#include "SDL.hxx"

#include <cassert>
#include <sstream>

#include <SDL_image.h>
#include <SDL_ttf.h>

namespace {
  std::string build_error_message(std::string_view const message, std::source_location const location)
  {
    std::ostringstream strm;
    strm << location.file_name() << ":" << location.line() << ":" << location.column();
    strm << " - ";
    strm << message << " (" << SDL_GetError() << ")";
    return strm.str();
  }
}

SDLError::SDLError(std::string_view const message, std::source_location const location)
    :std::runtime_error{::build_error_message(message, location)}
{
}

SDL* SDL::instance_ = nullptr;

SDL::SDL(std::uint32_t const flags)
{
  assert(instance_==nullptr);
  if (SDL_Init(flags)!=0) {
    throw SDLError{"Failed to initialize SDL."};
  }
  SDL_Log("Initialized SDL successfully.");

  auto const img_flags = IMG_INIT_PNG | IMG_INIT_JPG;
  if (IMG_Init(img_flags)!=img_flags) {
    throw SDLError{"Failed to initialize SDL_image."};
  }
  SDL_Log("Initialized SDL_image successfully.");

  if (TTF_Init()!=0) {
    throw SDLError{"Failed to initialize SDL_ttf."};
  }
  SDL_Log("Initialized SDL_ttf successfully.");

  instance_ = this;
}

SDL::~SDL() noexcept
{
  assert(instance_!=nullptr);

  TTF_Quit();
  SDL_Log("Shut down SDL_ttf successfully.");

  IMG_Quit();
  SDL_Log("Shut down SDL_image successfully.");

  SDL_Quit();
  SDL_Log("Shut down SDL successfully.");
  instance_ = nullptr;
}

SDL& SDL::instance() noexcept
{
  assert(instance_!=nullptr);
  return *instance_;
}

SDL& SDL::require(std::uint32_t const flags) noexcept
{
  assert((SDL_WasInit(flags) | SDL_INIT_NOPARACHUTE)==(flags | SDL_INIT_NOPARACHUTE));
  return instance();
}


