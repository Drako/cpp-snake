#include "SDLWindow.hxx"

#include <algorithm>

SDLWindow::SDLWindow(std::string_view title, int x, int y, int w, int h, std::uint32_t flags)
{
  SDL::require(SDL_INIT_VIDEO);
  window_ = SDL_CreateWindow(title.data(), x, y, w, h, flags);
  if (window_==nullptr) {
    throw SDLError("Failed to create window.");
  }
  SDL_Log("Created window successfully.");
}

SDLWindow::~SDLWindow() noexcept
{
  destroy();
}

void SDLWindow::destroy() noexcept
{
  if (window_!=nullptr) {
    SDL_DestroyWindow(window_);
    SDL_Log("Destroyed window successfully.");
    window_ = nullptr;
  }
}

SDLWindow::SDLWindow(SDLWindow&& src) noexcept
    :window_{src.window_}
{
  src.window_ = nullptr;
}

SDLWindow& SDLWindow::operator=(SDLWindow&& src) noexcept
{
  destroy();
  std::swap(window_, src.window_);
  return *this;
}
