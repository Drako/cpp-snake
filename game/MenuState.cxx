#include "MenuState.hxx"
#include "PlayingState.hxx"
#include "GameStateManager.hxx"
#include "TranslationManager.hxx"
#include "../SDLRenderer.hxx"

#include <array>

void MenuState::on_enter(GameStateManager& gsm)
{
  active_button_ = 0;

  auto const& tm = TranslationManager::instance();
  new_game_button_.set_title(tm.get_translation("New game"));
  continue_button_.set_title(tm.get_translation("Continue"));
  high_score_button_.set_title(tm.get_translation("High Scores"));
  credits_button_.set_title(tm.get_translation("Credits"));
  quit_button_.set_title(tm.get_translation("Quit"));

  game_.reset();
  if (auto const parent = gsm.parent(); parent!=nullptr) {
    continue_button_.set_visible(true);
    if (auto const game = dynamic_cast<PlayingState*>(parent); game!=nullptr) {
      game_ = game;
      active_button_ = 1;
    }
  }
  else {
    continue_button_.set_visible(false);
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
    case SDL_SCANCODE_UP:
      SDL_ShowCursor(SDL_DISABLE);
      --active_button_;
      if (active_button_<0)
        active_button_ = 4;
      else if (active_button_==1 && !continue_button_.is_visible())
        active_button_ = 0;
      break;
    case SDL_SCANCODE_DOWN:
      SDL_ShowCursor(SDL_DISABLE);
      ++active_button_;
      if (active_button_>4)
        active_button_ = 0;
      else if (active_button_==1 && !continue_button_.is_visible())
        active_button_ = 2;
      break;
    case SDL_SCANCODE_RETURN:
      switch (active_button_) {
      case 0:
        new_game_button_.trigger();
        break;
      case 1:
        continue_button_.trigger();
        break;
      case 2:
        high_score_button_.trigger();
        break;
      case 3:
        credits_button_.trigger();
        break;
      case 4:
        quit_button_.trigger();
        break;
      }
      break;
    }
  }
  else if (evt.type==SDL_MOUSEMOTION) {
    SDL_ShowCursor(SDL_ENABLE);

    auto const x = evt.motion.x;
    auto const y = evt.motion.y;

    std::array<Button*, 5> buttons{&new_game_button_, nullptr, &high_score_button_, &credits_button_, &quit_button_};
    if (continue_button_.is_visible())
      buttons[1] = &continue_button_;

    for (std::size_t n = 0; n<buttons.size(); ++n) {
      if (buttons[n]==nullptr)
        continue;

      auto const box = buttons[n]->get_bounding_box();
      if (x>=box.x && x<=box.x+box.w && y>=box.y && y<=box.y+box.h) {
        active_button_ = static_cast<int>(n);
        break;
      }
    }
  }
}

void MenuState::update(GameStateManager& gsm, std::chrono::milliseconds const delta_time)
{
  (void) delta_time;

  auto adjust_button_size = [this, idx = 0](Button& btn) mutable {
    if (idx++==active_button_) {
      btn.resize(BUTTON_WIDTH, BUTTON_HEIGHT);
    }
    else {
      btn.resize(BUTTON_WIDTH-ACTIVE_SIZE_DIFF, BUTTON_HEIGHT-ACTIVE_SIZE_DIFF);
    }
  };

  for (auto const btn: {&new_game_button_, &continue_button_, &high_score_button_, &credits_button_, &quit_button_}) {
    adjust_button_size(*btn);
    btn->update();
  }
}

void MenuState::render(SDLRenderer& renderer)
{
  int screen_w, screen_h;
  SDL_GetRendererOutputSize(renderer, &screen_w, &screen_h);

  int const button_count = continue_button_.is_visible() ? 5 : 4;

  int const x = (screen_w-BUTTON_WIDTH)/2;
  int y = (screen_h-(button_count*BUTTON_HEIGHT+(button_count-1)*20))/2;

  auto const adjust_button_position = [](Button& btn, int x, int y) {
    auto const box = btn.get_bounding_box();
    auto const dw = BUTTON_WIDTH-box.w;
    auto const dh = BUTTON_HEIGHT-box.h;
    btn.move(x+dw/2, y+dh/2);
  };

  adjust_button_position(new_game_button_, x, y);
  if (continue_button_.is_visible()) {
    adjust_button_position(continue_button_, x, y += BUTTON_HEIGHT+20);
  }
  adjust_button_position(high_score_button_, x, y += BUTTON_HEIGHT+20);
  adjust_button_position(credits_button_, x, y += BUTTON_HEIGHT+20);
  adjust_button_position(quit_button_, x, y+BUTTON_HEIGHT+20);

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
