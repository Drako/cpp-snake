#pragma once

#ifndef SNAKE_DUMMYSTATE_HXX
#define SNAKE_DUMMYSTATE_HXX

#include "GameState.hxx"
#include "ui/Button.hxx"

class DummyState final : public GameState {
public:
  void update(GameStateManager& gsm, std::chrono::milliseconds delta_time) override;

  void render(SDLRenderer& renderer) override;

private:
  std::chrono::milliseconds time_in_state_{0};
  Button quit_button_{"Quit", 15, 15, 800, 80};
};

#endif // SNAKE_DUMMYSTATE_HXX
