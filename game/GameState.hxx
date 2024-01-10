#pragma once

#ifndef SNAKE_GAMESTATE_HXX
#define SNAKE_GAMESTATE_HXX

#include <chrono>

#include <SDL.h>

class GameStateManager;

class SDLRenderer;

class GameState {
public:
  virtual ~GameState() noexcept = default;

  virtual void on_enter(GameStateManager& gsm) { (void) gsm; }

  virtual void on_leave() { }

  virtual void on_event(GameStateManager& gsm, SDL_Event const& event)
  {
    (void) gsm;
    (void) event;
  }

  virtual void update(GameStateManager& gsm, std::chrono::milliseconds delta_time) = 0;

  virtual void render(SDLRenderer& renderer) = 0;
};

#endif // SNAKE_GAMESTATE_HXX
