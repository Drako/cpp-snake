#include "SDL.hxx"
#include "SDLWindow.hxx"

#include <cstdlib>

void main_loop(SDLWindow& window)
{
  for (;;) {
    SDL_Event evt;
    while (SDL_PollEvent(&evt)!=0) {
      if (evt.type==SDL_QUIT) {
        return;
      }
    }

    // Game logic, render... (Implement your logic here if needed.)

    SDL_Delay(16);
  }
}

int main(int argc, char** argv) try
{
  SDL sdl{};
  SDLWindow window{
      "Snake",
      SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
      0, 0,
      SDL_WINDOW_FULLSCREEN_DESKTOP | SDL_WINDOW_BORDERLESS
  };

  main_loop(window);
  return EXIT_SUCCESS;
} catch (std::exception const& ex) {
  SDL_LogError(SDL_LOG_CATEGORY_ERROR, "%s", ex.what());
  return EXIT_FAILURE;
}