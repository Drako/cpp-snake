#pragma once

#ifndef SNAKE_SDLRENDERER_HXX
#define SNAKE_SDLRENDERER_HXX

#include "SDLWindow.hxx"

#include <boost/noncopyable.hpp>

class SDLRenderer final : private boost::noncopyable {
public:
  explicit SDLRenderer(SDLWindow& window);

  ~SDLRenderer() noexcept;

  SDLRenderer(SDLRenderer&& src) noexcept;

  SDLRenderer& operator=(SDLRenderer&& src) noexcept;

  void destroy() noexcept;

  operator SDL_Renderer*() const noexcept // NOLINT(*-explicit-constructor)
  {
    return renderer_;
  }

private:
  SDL_Renderer* renderer_;
};

#endif // SNAKE_SDLRENDERER_HXX
