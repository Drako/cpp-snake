#include "MenuState.hxx"
#include "GameStateManager.hxx"
#include "../SDLRenderer.hxx"

void MenuState::on_enter(GameStateManager& gsm)
{
  new_game_button_.set_on_click([&gsm] {
    if (gsm.parent()!=nullptr)
      gsm.pop_state();
    gsm.replace_state(GameStates::Game);
  });

  continue_button_.set_on_click([&gsm] {
    gsm.pop_state();
  });

  quit_button_.set_on_click([&gsm] {
    while (gsm.current()!=nullptr) {
      gsm.pop_state();
    }
  });
}

void MenuState::on_event(GameStateManager& gsm, SDL_Event const& evt)
{
  if (evt.type==SDL_KEYUP && evt.key.keysym.scancode==SDL_SCANCODE_ESCAPE) {
    gsm.pop_state();
  }
}

void MenuState::update(GameStateManager& gsm, std::chrono::milliseconds delta_time)
{
  (void) delta_time;

  continue_button_.set_visible(gsm.parent()!=nullptr);

  new_game_button_.update();
  continue_button_.update();
  quit_button_.update();
}

void MenuState::render(SDLRenderer& renderer)
{
  int screen_w, screen_h;
  SDL_GetRendererOutputSize(renderer, &screen_w, &screen_h);

  int const button_count = continue_button_.is_visible() ? 3 : 2;

  int const x = (screen_w-BUTTON_WIDTH)/2;
  int const y = (screen_h-(button_count*BUTTON_HEIGHT+(button_count-1)*20))/2;

  new_game_button_.move(x, y);
  continue_button_.move(x, y+BUTTON_HEIGHT+20);
  quit_button_.move(x, y+(button_count-1)*(BUTTON_HEIGHT+20));

  SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
  SDL_RenderClear(renderer);

  new_game_button_.render(renderer);
  continue_button_.render(renderer);
  quit_button_.render(renderer);

  SDL_RenderPresent(renderer);
}
