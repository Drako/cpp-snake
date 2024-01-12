#include "AssetManager.hxx"

#include <cassert>
#include <filesystem>

#include <SDL_image.h>

AssetManager* AssetManager::instance_ = nullptr;

AssetManager::AssetManager(SDLRenderer& renderer)
    :renderer_{renderer}
{
  namespace fs = std::filesystem;

  assert(instance_==nullptr);

  auto const current_path = fs::current_path();
  auto const asset_directory =
      fs::exists(current_path/"assets")
      ? current_path/"assets"
      : fs::exists(current_path.parent_path()/"assets")
        ? current_path.parent_path()/"assets"
        : fs::path{};
  if (asset_directory.empty()) {
    throw std::runtime_error("Assets directory not found.");
  }

  total_assets_ = std::distance(fs::directory_iterator(asset_directory),
      fs::directory_iterator());

  loading_thread_ = std::thread{&AssetManager::load_assets, this, asset_directory};
  instance_ = this;
}

AssetManager::~AssetManager()
{
  assert(instance_!=nullptr);
  if (loading_thread_.joinable()) {
    loading_thread_.join();
  }
  for (auto const& kv: surface_assets_) {
    SDL_FreeSurface(kv.second);
  }
  for (auto const& kv: texture_assets_) {
    SDL_DestroyTexture(kv.second);
    SDL_Log("Unloaded texture %s successfully.", kv.first.c_str());
  }
  for (auto const& kv: font_assets_) {
    TTF_CloseFont(kv.second);
    SDL_Log("Unloaded font %s successfully.", kv.first.c_str());
  }
  instance_ = nullptr;
}

void AssetManager::load_assets(std::filesystem::path const& asset_directory)
{
  for (auto const& entry: std::filesystem::directory_iterator(asset_directory)) {
    auto const path = entry.path().string();
    auto const ext = entry.path().extension();
    auto const filename = entry.path().filename().string();
    if (ext==".png" || ext==".jpg") {
      auto const surface = IMG_Load(path.c_str());
      if (surface==nullptr) {
        throw SDLError{"Failed to load texture "+path+"."};
      }
      surface_assets_[filename] = surface;
      SDL_Log("Loaded texture %s successfully.", filename.c_str());
    }
    else if (ext==".ttf") {
      auto const font = TTF_OpenFont(path.c_str(), 42);
      if (font==nullptr) {
        throw SDLError{"Failed to load font "+path+"."};
      }
      font_assets_[filename] = font;
      SDL_Log("Loaded font %s successfully.", filename.c_str());
    }
    ++assets_loaded_;
  }
}

float AssetManager::get_progress() const
{
  return static_cast<float>(assets_loaded_)/static_cast<float>(total_assets_);
}

SDL_Texture* AssetManager::get_texture_asset(std::string const& filepath)
{
  auto const it = texture_assets_.find(filepath);
  if (it==std::end(texture_assets_)) {
    auto const surf_it = surface_assets_.find(filepath);
    if (surf_it==std::end(surface_assets_))
      return nullptr;

    auto const texture = SDL_CreateTextureFromSurface(renderer_, surf_it->second);
    texture_assets_[filepath] = texture;
    SDL_FreeSurface(surf_it->second);
    surface_assets_.erase(surf_it);

    return texture;
  }
  return it->second;
}

TTF_Font* AssetManager::get_font_asset(std::string const& filepath)
{
  return font_assets_[filepath];
}

AssetManager& AssetManager::instance() noexcept
{
  assert(instance_!=nullptr);
  return *instance_;
}
