#pragma once

#ifndef SNAKE_ASSETMANAGER_HXX
#define SNAKE_ASSETMANAGER_HXX

#include "../SDLRenderer.hxx"

#include <atomic>
#include <filesystem>
#include <thread>
#include <unordered_map>

#include <SDL_ttf.h>

class AssetManager final {

public:
  explicit AssetManager(SDLRenderer& renderer);

  AssetManager(AssetManager const&) = delete;

  AssetManager& operator=(AssetManager const&) = delete;

  ~AssetManager();

  static AssetManager& instance() noexcept;

  float get_progress() const;

  SDL_Texture* get_texture_asset(std::string const& filepath);

  TTF_Font* get_font_asset(std::string const& filepath);

private:
  void load_assets(std::filesystem::path const& asset_directory);

  static AssetManager* instance_;

  std::atomic<size_t> assets_loaded_{0u};
  std::atomic<size_t> total_assets_{0u};
  std::thread loading_thread_;

  SDLRenderer& renderer_;

  std::unordered_map<std::string, SDL_Surface*> surface_assets_;
  std::unordered_map<std::string, SDL_Texture*> texture_assets_;
  std::unordered_map<std::string, TTF_Font*> font_assets_;
};

#endif // SNAKE_ASSETMANAGER_HXX
