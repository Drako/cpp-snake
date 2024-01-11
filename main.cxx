#include "SDL.hxx"
#include "SDLWindow.hxx"
#include "SDLRenderer.hxx"

#include "game/AssetManager.hxx"
#include "game/GameStateManager.hxx"

#include <cstdlib>

void main_loop(SDLRenderer& renderer)
{
  using namespace std::chrono;

  AssetManager am{renderer};
  GameStateManager gsm{};

  auto start = high_resolution_clock::now();
  for (;;) {
    SDL_Event evt;
    while (SDL_PollEvent(&evt)!=0) {
      if (evt.type==SDL_QUIT)
        return;

      if (auto const state = gsm.current(); state!=nullptr)
        state->on_event(gsm, evt);
      else
        return;
    }

    auto const end = high_resolution_clock::now();
    auto const delta_time = duration_cast<milliseconds>(end-start);
    start = end;

    if (auto const state = gsm.current(); state != nullptr)
      state->update(gsm, delta_time);
    else
      return;

    if (auto const state = gsm.current(); state != nullptr)
      state->render(renderer);
    else
      return;
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
  SDLRenderer renderer{window};

  main_loop(renderer);
  return EXIT_SUCCESS;
} catch (std::exception const& ex) {
  SDL_LogError(SDL_LOG_CATEGORY_ERROR, "%s", ex.what());
  return EXIT_FAILURE;
}
