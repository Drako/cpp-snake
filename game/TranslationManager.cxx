#include "TranslationManager.hxx"

#include <SDL.h>

#include <fstream>
#include <optional>
#include <regex>

namespace {
  std::optional<SupportedLanguage> language_from_string(std::string const& lang)
  {
    static std::unordered_map<std::string, SupportedLanguage> const MAPPING{
        {"en", SupportedLanguage::English},
        {"de", SupportedLanguage::German},
    };

    auto const it = MAPPING.find(lang);
    if (it==MAPPING.end())
      return {};
    return it->second;
  }

  SupportedLanguage get_preferred_language()
  {
    auto const locales = SDL_GetPreferredLocales();
    for (auto const* locale = locales; locale->language!=nullptr; ++locale) {
      auto const lang = language_from_string(locale->language);
      if (lang.has_value()) {
        SDL_free(locales);
        return lang.value();
      }
    }
    SDL_free(locales);
    return SupportedLanguage::English;
  }

  std::regex const KEY_REGEX = std::regex{R"(^\[([^\]]*)\]$)"};
  std::regex const VALUE_REGEX = std::regex{R"(^(en|de) = (.+)$)"};
}

TranslationManager::TranslationManager() noexcept
    :current_language_{::get_preferred_language()}
{
}

TranslationManager& TranslationManager::instance()
{
  static TranslationManager tm;
  return tm;
}

std::string TranslationManager::get_translation(std::string const& key) const
{
  auto const it = translations_.find(key);
  if (it==translations_.end())
    return key;

  auto const lang_idx = static_cast<std::size_t>(current_language_);
  if (it->second[lang_idx].empty())
    return key;

  return it->second[lang_idx];
}

void TranslationManager::load(std::filesystem::path const& filename)
{
  std::ifstream input{filename};

  std::string current_key{};
  for (std::string line; std::getline(input, line);) {
    if (std::smatch key_match; std::regex_match(line, key_match, ::KEY_REGEX)) {
      current_key = key_match.str(1);
    }
    else if (std::smatch value_match; std::regex_match(line, value_match, ::VALUE_REGEX)) {
      // hacky, but right now the regex already makes sure we alway get a language back
      auto const lang = ::language_from_string(value_match.str(1)).value();
      auto const value = value_match.str(2);

      if (current_key.empty()) {
        SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "Translations without key not loaded! (%s)", value.c_str());
      }
      else {
        translations_[current_key][static_cast<std::size_t>(lang)] = value;
      }
    }
    else if (!line.empty()) {
      SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "Translation line with unsupported format: %s", line.c_str());
    }
  }
}
