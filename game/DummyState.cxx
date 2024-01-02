#include "DummyState.hxx"

#include "../SDLRenderer.hxx"

void DummyState::update(GameStateManager& gsm, std::chrono::milliseconds delta_time)
{
  (void) gsm;
  (void) delta_time;
}

void DummyState::render(SDLRenderer& renderer)
{
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
  SDL_RenderClear(renderer);
  SDL_RenderPresent(renderer);
}
