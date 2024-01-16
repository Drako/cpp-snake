#pragma once

#ifndef SNAKE_ASSETMANAGER_HXX
#define SNAKE_ASSETMANAGER_HXX

#include "../SDLRenderer.hxx"

#include <atomic>
#include <concepts>
#include <filesystem>
#include <thread>
#include <unordered_map>

#include <SDL_ttf.h>

#include <boost/noncopyable.hpp>

class AssetManager final : private boost::noncopyable {

public:
  explicit AssetManager(SDLRenderer& renderer);

  ~AssetManager();

  static AssetManager& instance() noexcept;

  float get_progress() const;

  SDL_Texture* get_texture_asset(std::string const& filepath);

  SDL_Surface* get_surface_asset(std::string const& filepath);

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

template<typename T>
concept Pointer = std::is_pointer_v<T>;

template<Pointer T>
struct AssetTraits;

template<>
struct AssetTraits<SDL_Texture*> final {
  using type = SDL_Texture*;

  static type get(std::string const& name)
  {
    return AssetManager::instance().get_texture_asset(name);
  }
};

template<>
struct AssetTraits<TTF_Font*> final {
  using type = TTF_Font*;

  static type get(std::string const& name)
  {
    return AssetManager::instance().get_font_asset(name);
  }
};

template<typename T>
class Asset final {
  using traits = AssetTraits<T>;

public:
  explicit Asset(std::string name)
      :name_{std::move(name)},
       asset_{nullptr}
  {
  }

  Asset(Asset const&) = default;

  Asset& operator=(Asset const&) = default;

  operator T&() // NOLINT(*-explicit-constructor)
  {
    evaluate();
    return asset_;
  }

  operator T const&() const // NOLINT(*-explicit-constructor)
  {
    evaluate();
    return asset_;
  }

private:
  void evaluate() const
  {
    if (asset_==nullptr) {
      asset_ = traits::get(name_);
    }
  }

  std::string name_;
  mutable T asset_;
};

#endif // SNAKE_ASSETMANAGER_HXX
