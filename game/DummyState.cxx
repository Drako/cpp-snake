#include "DummyState.hxx"

#include "../SDLRenderer.hxx"

void DummyState::update(GameStateManager& gsm, std::chrono::milliseconds delta_time)
{
  (void) gsm;
  time_in_state_ += delta_time;
  test_button_.set_pressed((time_in_state_.count() % 4'000) >= 2'000);
}

void DummyState::render(SDLRenderer& renderer)
{
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
  SDL_RenderClear(renderer);

  test_button_.render(renderer);

  SDL_RenderPresent(renderer);
}
