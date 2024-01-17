#pragma once

#ifndef SNAKE_HIGHSCORESTATE_HXX
#define SNAKE_HIGHSCORESTATE_HXX

#include "GameState.hxx"
#include "AssetManager.hxx"

class HighScoreState final : public GameState {
public:
  HighScoreState();

  void on_event(GameStateManager& gsm, SDL_Event const& event) override;

  void render(SDLRenderer& renderer) override;

private:
  Asset<TTF_Font*> font_;

  void render_heading(SDLRenderer& renderer, int width, SDL_Color const& color);
};

#endif // SNAKE_HIGHSCORESTATE_HXX
