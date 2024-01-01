#include "SDL.hxx"

#include <cassert>
#include <sstream>

SDLError::SDLError(std::string_view const context, std::source_location location)
    :std::runtime_error{(std::ostringstream{}
    << context << " at "
    << location.file_name() << ":" << location.line() << ":" << location.column()
    << " - " << SDL_GetError()).str()}
{
}

SDL* SDL::instance_ = nullptr;

SDL::SDL(std::uint32_t const flags)
{
  assert(instance_==nullptr);
  if (SDL_Init(flags)!=0) {
    throw SDLError("Failed to initialize SDL");
  }
  SDL_Log("Initialized SDL successfully.");
  instance_ = this;
}

SDL::~SDL() noexcept
{
  assert(instance_!=nullptr);
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


