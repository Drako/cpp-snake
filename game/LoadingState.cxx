#include "LoadingState.hxx"

#include "../SDLRenderer.hxx"

void LoadingState::update(GameStateManager& gsm, std::chrono::milliseconds const delta_time)
{
  (void)gsm;
  (void)delta_time;
}

void LoadingState::render(SDLRenderer& renderer) {
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
  SDL_RenderClear(renderer);
  SDL_RenderPresent(renderer);
}
