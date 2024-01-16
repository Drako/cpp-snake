#pragma once

#ifndef SNAKE_SDLWINDOW_HXX
#define SNAKE_SDLWINDOW_HXX

#include "SDL.hxx"

#include <string_view>

#include "NonCopyable.hxx"

class SDLWindow final : private NonCopyable {
public:
  SDLWindow(std::string_view title, int x, int y, int w, int h, std::uint32_t flags = 0u);

  ~SDLWindow() noexcept;

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
