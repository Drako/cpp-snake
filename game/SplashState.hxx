#pragma once

#ifndef SNAKE_SPLASHSTATE_HXX
#define SNAKE_SPLASHSTATE_HXX

#include "GameState.hxx"

#include <SDL.h>

class SplashState final : public GameState {
public:
  void on_enter() override;

  void on_leave() override;

  void update(GameStateManager& gsm, std::chrono::milliseconds delta_time) override;

  void render(SDLRenderer& renderer) override;

private:
  std::chrono::milliseconds time_in_state_;
  SDL_Texture* logo_;
};

#endif // SNAKE_SPLASHSTATE_HXX
