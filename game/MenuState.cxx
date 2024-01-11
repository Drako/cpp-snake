#include "MenuState.hxx"
#include "PlayingState.hxx"
#include "GameStateManager.hxx"
#include "../SDLRenderer.hxx"

void MenuState::on_enter(GameStateManager& gsm)
{
  game_.reset();
  if (auto const parent = gsm.parent(); parent!=nullptr) {
    if (auto const game = dynamic_cast<PlayingState*>(parent); game!=nullptr) {
      game_ = game;
    }
  }

  new_game_button_.set_on_click([&gsm] {
    if (gsm.parent()!=nullptr)
      gsm.pop_state();
    gsm.replace_state(GameStates::Game);
  });

  continue_button_.set_on_click([&gsm] {
    gsm.pop_state();
  });

  high_score_button_.set_on_click([&gsm] {
    gsm.push_state(GameStates::HighScores);
  });

  credits_button_.set_on_click([&gsm] {
    gsm.push_state(GameStates::Credits);
  });

  quit_button_.set_on_click([&gsm] {
    while (gsm.current()!=nullptr) {
      gsm.pop_state();
    }
  });

  SDL_ShowCursor(SDL_ENABLE);
}

void MenuState::on_event(GameStateManager& gsm, SDL_Event const& evt)
{
  if (evt.type==SDL_KEYUP) {
    switch (evt.key.keysym.scancode) {
    default:
      break;
    case SDL_SCANCODE_PAUSE:
      if (gsm.parent()==nullptr)
        break;
      [[fallthrough]];
    case SDL_SCANCODE_ESCAPE:
      gsm.pop_state();
      break;
    }
  }
}

void MenuState::update(GameStateManager& gsm, std::chrono::milliseconds delta_time)
{
  (void) delta_time;

  continue_button_.set_visible(gsm.parent()!=nullptr);

  new_game_button_.update();
  continue_button_.update();
  high_score_button_.update();
  credits_button_.update();
  quit_button_.update();
}

void MenuState::render(SDLRenderer& renderer)
{
  int screen_w, screen_h;
  SDL_GetRendererOutputSize(renderer, &screen_w, &screen_h);

  int const button_count = continue_button_.is_visible() ? 5 : 4;

  int const x = (screen_w-BUTTON_WIDTH)/2;
  int y = (screen_h-(button_count*BUTTON_HEIGHT+(button_count-1)*20))/2;

  new_game_button_.move(x, y);
  if (continue_button_.is_visible()) {
    continue_button_.move(x, y += BUTTON_HEIGHT+20);
  }
  high_score_button_.move(x, y += BUTTON_HEIGHT+20);
  credits_button_.move(x, y += BUTTON_HEIGHT+20);
  quit_button_.move(x, y+BUTTON_HEIGHT+20);

  SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
  SDL_RenderClear(renderer);

  if (game_.has_value()) {
    game_.value()->render_game(renderer, false);
  }

  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 200);
  SDL_RenderFillRect(renderer, nullptr);

  new_game_button_.render(renderer);
  continue_button_.render(renderer);
  high_score_button_.render(renderer);
  credits_button_.render(renderer);
  quit_button_.render(renderer);

  SDL_RenderPresent(renderer);
}

void MenuState::on_leave()
{
  SDL_ShowCursor(SDL_DISABLE);
}
