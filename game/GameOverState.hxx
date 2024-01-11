#pragma once

#ifndef SNAKE_GAMEOVERSTATE_HXX
#define SNAKE_GAMEOVERSTATE_HXX

#include "AssetManager.hxx"
#include "GameState.hxx"
#include "ui/Button.hxx"
#include "ui/LineInput.hxx"

class GameOverState final : public GameState {
public:
  GameOverState();

  void on_enter(GameStateManager& gsm) override;

  void on_leave() override;

  void on_event(GameStateManager& gsm, SDL_Event const& evt) override;

  void update(GameStateManager& gsm, std::chrono::milliseconds delta_time) override;

  void render(SDLRenderer& renderer) override;

private:
  Button ok_button_{"OK", 0, 0, 800, 80, UiColor::Green};
  LineInput name_input_{0, 0, 800, 80};

  Asset<TTF_Font*> font_;
};

#endif // SNAKE_GAMEOVERSTATE_HXX
