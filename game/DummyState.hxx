#pragma once

#ifndef SNAKE_DUMMYSTATE_HXX
#define SNAKE_DUMMYSTATE_HXX

#include "GameState.hxx"

class DummyState final : public GameState {
public:
  void update(GameStateManager& gsm, std::chrono::milliseconds delta_time) override;

  void render(SDLRenderer& renderer) override;
};

#endif // SNAKE_DUMMYSTATE_HXX
