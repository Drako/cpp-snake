#pragma once

#ifndef SNAKE_GAMESTATEMANAGER_HXX
#define SNAKE_GAMESTATEMANAGER_HXX

#include "LoadingState.hxx"
#include "SplashState.hxx"
#include "DummyState.hxx"

#include <stack>

enum class GameStates {
  Loading,
  Splash,
  MainMenu,
  Game,
  GameOver,
};

class GameStateManager final {
public:
  GameStateManager();

  ~GameStateManager();

  GameState* current();

  void push_state(GameStates new_state);

  void pop_state();

  void replace_state(GameStates new_state);

private:
  std::stack<GameStates> states_;

  LoadingState loading_;
  SplashState splash_;
  DummyState dummy_;
};

#endif // SNAKE_GAMESTATEMANAGER_HXX
