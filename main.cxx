#include "SDL.hxx"
#include "SDLWindow.hxx"

#include <cstdlib>

int main(int argc, char** argv) try
{
  SDL sdl{};
  SDLWindow window{
      "Snake",
      SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
      0, 0,
      SDL_WINDOW_FULLSCREEN_DESKTOP | SDL_WINDOW_BORDERLESS
  };

  return EXIT_SUCCESS;
} catch (std::exception const& ex) {
  SDL_LogError(SDL_LOG_CATEGORY_ERROR, "%s", ex.what());
  return EXIT_FAILURE;
}
