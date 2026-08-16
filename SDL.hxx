#pragma once

#ifndef SNAKE_SDL_HXX
#define SNAKE_SDL_HXX

#include <SDL3/SDL.h>

#include <cstdint>
#include <source_location>
#include <stdexcept>
#include <string_view>
#include <unordered_map>
#include <vector>

#include "NonCopyable.hxx"

/**
 * @class SDLError
 * @brief Represents an SDL error.
 *
 * This class is a subclass of std::runtime_error and represents an error that occurred
 * during an SDL operation. It provides additional information such as the context where
 * the error occurred and the source location.
 */
class SDLError final : public std::runtime_error {
public:
  explicit SDLError(
      std::string_view message,
      std::source_location location = std::source_location::current()
  );
};

/**
 * @class SDL
 * @brief The SDL class is used to initialize and clean up the SDL library.
 *
 * This class provides a convenient way to initialize SDL and clean up resources when they are no longer needed.
 * Only one instance of this class can exist at a time.
 */
class SDL final : private NonCopyable {
public:
  explicit SDL(std::uint32_t flags = SDL_INIT_GAMEPAD|SDL_INIT_VIDEO|SDL_INIT_EVENTS);

  ~SDL() noexcept;

  static SDL& instance() noexcept;

  static SDL& require(std::uint32_t flags) noexcept;

  void add_controller(SDL_JoystickID which);

  void remove_controller(SDL_JoystickID which);

  [[nodiscard]] std::unordered_map<SDL_JoystickID, SDL_Gamepad*> const& get_controllers() const;

private:
  static SDL* instance_;

  std::unordered_map<SDL_JoystickID, SDL_Gamepad*> controllers_;
};

#endif // SNAKE_SDL_HXX
