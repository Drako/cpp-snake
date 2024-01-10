#pragma once

#ifndef SNAKE_DUMMYSTATE_HXX
#define SNAKE_DUMMYSTATE_HXX

#include "GameState.hxx"
#include "ui/Button.hxx"
#include "ui/LineInput.hxx"

class DummyState final : public GameState {
public:
  void on_enter(GameStateManager& gsm) override;

  void on_leave() override;

  void on_event(GameStateManager& gsm, SDL_Event const& evt) override;

  void update(GameStateManager& gsm, std::chrono::milliseconds delta_time) override;

  void render(SDLRenderer& renderer) override;

private:
  Button pause_button_{"Pause", 15, 15, 800, 80};
  LineInput name_input_{15, 200, 800, 80};
};

#endif // SNAKE_DUMMYSTATE_HXX
