#pragma once

#ifndef SNAKE_LOADINGSTATE_HXX
#define SNAKE_LOADINGSTATE_HXX

#include "GameState.hxx"

class LoadingState final : public GameState {
public:
  void on_enter(GameStateManager& gsm) override;

  void update(GameStateManager& gsm, std::chrono::milliseconds delta_time) override;

  void render(SDLRenderer& renderer) override;
};

#endif // SNAKE_LOADINGSTATE_HXX
