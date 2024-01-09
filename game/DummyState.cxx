#include "DummyState.hxx"
#include "GameStateManager.hxx"

#include "../SDLRenderer.hxx"

void DummyState::on_enter(GameStateManager& gsm)
{
  pause_button_.set_on_click([&gsm] {
    gsm.push_state(GameStates::MainMenu);
  });
}

void DummyState::update(GameStateManager& gsm, std::chrono::milliseconds delta_time)
{
  pause_button_.update();
}

void DummyState::render(SDLRenderer& renderer)
{
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
  SDL_RenderClear(renderer);

  pause_button_.render(renderer);

  SDL_RenderPresent(renderer);
}
