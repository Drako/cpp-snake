#pragma once

#ifndef SNAKE_SDLWINDOW_HXX
#define SNAKE_SDLWINDOW_HXX

#include "SDL.hxx"

#include <string_view>

class SDLWindow final {
public:
  SDLWindow(std::string_view title, int x, int y, int w, int h, std::uint32_t flags = 0u);

  ~SDLWindow();

  SDLWindow(SDLWindow const&) = delete;

  SDLWindow& operator=(SDLWindow const&) = delete;

  SDLWindow(SDLWindow&& src) noexcept;

  SDLWindow& operator=(SDLWindow&& src) noexcept;

  void destroy() noexcept;

  operator SDL_Window*() const noexcept // NOLINT(*-explicit-constructor)
  {
    return window_;
  }

private:
  SDL_Window* window_;
};

#endif // SNAKE_SDLWINDOW_HXX
