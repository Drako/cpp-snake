#pragma once

#ifndef SNAKE_TRANSLATIONMANAGER_HXX
#define SNAKE_TRANSLATIONMANAGER_HXX

#include <array>
#include <filesystem>
#include <string>
#include <unordered_map>

enum class SupportedLanguage {
  English = 0,
  German,

  NUM_SUPPORTED_LANGUAGES,
};

class TranslationManager final {
public:
  static TranslationManager& instance();

  void load(std::filesystem::path const& filename);

  [[nodiscard]] std::string get_translation(std::string const& key) const;

private:
  TranslationManager() noexcept;

  SupportedLanguage current_language_;

  std::unordered_map<
      std::string,
      std::array<std::string, static_cast<std::size_t>(SupportedLanguage::NUM_SUPPORTED_LANGUAGES)>
  > translations_;
};

#endif // SNAKE_TRANSLATIONMANAGER_HXX
