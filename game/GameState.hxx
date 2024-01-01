#pragma once

#ifndef SNAKE_GAMESTATE_HXX
#define SNAKE_GAMESTATE_HXX

#include <chrono>

class GameStateManager;

class SDLRenderer;

class GameState {
public:
  virtual ~GameState() noexcept = default;

  virtual void on_enter() { }

  virtual void on_leave() { }

  virtual void update(GameStateManager& gsm, std::chrono::milliseconds delta_time) = 0;

  virtual void render(SDLRenderer& renderer) = 0;
};

#endif // SNAKE_GAMESTATE_HXX
