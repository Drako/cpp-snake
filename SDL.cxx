#include "SDL.hxx"

#include <algorithm>
#include <cassert>
#include <sstream>

#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>

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
  if (!SDL_Init(flags)) {
    throw SDLError{"Failed to initialize SDL."};
  }
  SDL_Log("Initialized SDL successfully.");

  if (!TTF_Init()) {
    throw SDLError{"Failed to initialize SDL_ttf."};
  }
  SDL_Log("Initialized SDL_ttf successfully.");

  int num_joysticks = 0;
  SDL_JoystickID* ids = SDL_GetJoysticks(&num_joysticks);
  for (auto id = ids; id < ids + num_joysticks; ++id) {
    add_controller(*id);
  }

  instance_ = this;
}

SDL::~SDL() noexcept
{
  assert(instance_!=nullptr);

  for (auto const& [id, controller]: controllers_) {
    SDL_Log("Closing controller %s.", SDL_GetGamepadName(controller));
    SDL_CloseGamepad(controller);
  }

  TTF_Quit();
  SDL_Log("Shut down SDL_ttf successfully.");

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
  assert(SDL_WasInit(flags)==flags);
  return instance();
}

void SDL::add_controller(SDL_JoystickID const which)
{
  auto const controller = SDL_OpenGamepad(which);
  controllers_[which] = controller;
  SDL_Log("Opened controller %s.", SDL_GetGamepadName(controller));
}

void SDL::remove_controller(SDL_JoystickID const which)
{
  auto const controller = controllers_[which];
  SDL_Log("Closing controller %s.", SDL_GetGamepadName(controller));
  SDL_CloseGamepad(controller);
  controllers_.erase(which);
}

std::unordered_map<SDL_JoystickID, SDL_Gamepad*> const& SDL::get_controllers() const
{
  return controllers_;
}


