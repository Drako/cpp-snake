#include "DummyState.hxx"
#include "GameStateManager.hxx"

#include "../SDLRenderer.hxx"

void DummyState::update(GameStateManager& gsm, std::chrono::milliseconds delta_time)
{
  time_in_state_ += delta_time;
  quit_button_.set_on_click([&gsm] {
    gsm.pop_state();
  });
  quit_button_.update();
}

void DummyState::render(SDLRenderer& renderer)
{
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
  SDL_RenderClear(renderer);

  quit_button_.render(renderer);

  SDL_RenderPresent(renderer);
}
