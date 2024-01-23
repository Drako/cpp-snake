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
  static int constexpr ACTIVE_SIZE_DIFF = 32;

  static int constexpr BUTTON_HEIGHT = 80;
  static int constexpr BUTTON_WIDTH = 350;

  void handle_key_up(GameStateManager& gsm, SDL_Scancode scancode);

  void handle_key_down(SDL_Scancode scancode);

  void handle_controller_button_up(GameStateManager& gsm, std::uint8_t button);

  void handle_controller_button_down(std::uint8_t button);

  void handle_mouse_movement(int x, int y);

  void handle_controller_axis_motion(std::uint8_t axis, std::int16_t value);

  void select_previous_button();

  void select_next_button();

  void trigger_active_button();

  Button new_game_button_{0, 0, BUTTON_WIDTH, BUTTON_HEIGHT, UiColor::Green};
  Button continue_button_{0, 0, BUTTON_WIDTH, BUTTON_HEIGHT, UiColor::Blue};
  Button high_score_button_{0, 0, BUTTON_WIDTH, BUTTON_HEIGHT};
  Button credits_button_{0, 0, BUTTON_WIDTH, BUTTON_HEIGHT};
  Button quit_button_{0, 0, BUTTON_WIDTH, BUTTON_HEIGHT, UiColor::Red};

  std::optional<PlayingState*> game_{};

  int active_button_{0};
  int last_controller_direction_{0};
};

#endif // SNAKE_MENUSTATE_HXX
