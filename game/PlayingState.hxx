#pragma once

#ifndef SNAKE_PLAYINGSTATE_HXX
#define SNAKE_PLAYINGSTATE_HXX

#include "GameState.hxx"
#include "AssetManager.hxx"

#include <deque>
#include <optional>
#include <random>

enum class Direction {
  Up,
  Right,
  Down,
  Left,
};

class PlayingState final : public GameState {
public:
  static int constexpr CELLS_X = 64;
  static int constexpr CELLS_Y = 48;
  static float constexpr ACCELERATION = 1.05f;
  static float constexpr MAX_DISTANCE = 1.0f; // one cell per cycle
  static float constexpr START_SPEED = 0.005f; // 5 cell per second
  static float constexpr MAX_SPEED = 0.01f; // 20 cells per second

  PlayingState();

  void on_enter(GameStateManager& gsm) override;

  void on_event(GameStateManager& gsm, SDL_Event const& evt) override;

  void update(GameStateManager& gsm, std::chrono::milliseconds delta_time) override;

  void render(SDLRenderer& renderer) override;

  void render_game(SDLRenderer& renderer, bool is_current_state = true);

private:

  void place_head();

  bool place_target();

  void render_ui(SDLRenderer& renderer, SDL_Rect const& playing_field);

  void render_target(SDLRenderer& renderer, SDL_Rect const& playing_field);

  void render_snake(SDLRenderer& renderer, SDL_Rect const& playing_field);

  bool detect_death(SDL_Point const& position);

  void handle_direction_change();

  std::mt19937 generator_;
  std::uniform_int_distribution<int> distribution_position_x_{0, CELLS_X-1};
  std::uniform_int_distribution<int> distribution_position_y_{0, CELLS_Y-1};

  SDL_Point target_{};
  unsigned length_{0u};
  Direction direction_{Direction::Left};
  std::optional<Direction> new_direction_{};
  SDL_FPoint head_{};
  std::deque<SDL_Point> tail_;
  float speed_{0.001f};
  int fps_{0};

  Asset<TTF_Font*> font_;
};

#endif // SNAKE_PLAYINGSTATE_HXX
