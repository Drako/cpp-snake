#pragma once

#ifndef SNAKE_SPLASHSTATE_HXX
#define SNAKE_SPLASHSTATE_HXX

#include "GameState.hxx"

#include <SDL.h>

class SplashState final : public GameState {
public:
  void on_enter(GameStateManager& gsm) override;

  void on_event(GameStateManager& gsm, SDL_Event const& evt) override;

  void update(GameStateManager& gsm, std::chrono::milliseconds delta_time) override;

  void render(SDLRenderer& renderer) override;

private:
  std::chrono::milliseconds time_in_state_{0};
  SDL_Texture* logo_{nullptr};
};

#endif // SNAKE_SPLASHSTATE_HXX
