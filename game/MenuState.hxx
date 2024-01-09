#pragma once

#ifndef SNAKE_MENUSTATE_HXX
#define SNAKE_MENUSTATE_HXX

#include "GameState.hxx"
#include "ui/Button.hxx"

class MenuState final : public GameState {
public:
  void on_enter(GameStateManager& gsm) override;

  void update(GameStateManager& gsm, std::chrono::milliseconds delta_time) override;

  void render(SDLRenderer& renderer) override;

private:
  static int constexpr BUTTON_HEIGHT = 80;
  static int constexpr BUTTON_WIDTH = 300;

  Button new_game_button_{"New game", 0, 0, BUTTON_WIDTH, BUTTON_HEIGHT, UiColor::Green};
  Button continue_button_{"Continue", 0, 0, BUTTON_WIDTH, BUTTON_HEIGHT};
  Button quit_button_{"Quit", 0, 0, BUTTON_WIDTH, BUTTON_HEIGHT};
  bool escape_pressed_{false};
};

#endif // SNAKE_MENUSTATE_HXX
