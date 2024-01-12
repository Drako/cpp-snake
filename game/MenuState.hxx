#pragma once

#ifndef SNAKE_MENUSTATE_HXX
#define SNAKE_MENUSTATE_HXX

#include "GameState.hxx"
#include "ui/Button.hxx"

#include <optional>

class PlayingState;

class MenuState final : public GameState {
public:
  void on_enter(GameStateManager& gsm) override;

  void on_leave() override;

  void on_event(GameStateManager& gsm, SDL_Event const& evt) override;

  void update(GameStateManager& gsm, std::chrono::milliseconds delta_time) override;

  void render(SDLRenderer& renderer) override;

private:
  static int constexpr ACTIVE_SIZE_DIFF = 8;

  static int constexpr BUTTON_HEIGHT = 80;
  static int constexpr BUTTON_WIDTH = 350;

  Button new_game_button_{"New game", 0, 0, BUTTON_WIDTH, BUTTON_HEIGHT, UiColor::Green};
  Button continue_button_{"Continue", 0, 0, BUTTON_WIDTH, BUTTON_HEIGHT, UiColor::Blue};
  Button high_score_button_{"High Scores", 0, 0, BUTTON_WIDTH, BUTTON_HEIGHT};
  Button credits_button_{"Credits", 0, 0, BUTTON_WIDTH, BUTTON_HEIGHT};
  Button quit_button_{"Quit", 0, 0, BUTTON_WIDTH, BUTTON_HEIGHT, UiColor::Red};

  std::optional<PlayingState*> game_{};

  int active_button_{0};
};

#endif // SNAKE_MENUSTATE_HXX
