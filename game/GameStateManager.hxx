#pragma once

#ifndef SNAKE_GAMESTATEMANAGER_HXX
#define SNAKE_GAMESTATEMANAGER_HXX

#include "LoadingState.hxx"
#include "SplashState.hxx"
#include "MenuState.hxx"
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

  GameStateManager(GameStateManager const&) = delete;

  GameStateManager& operator=(GameStateManager) = delete;

  GameState* current();

  GameState* parent();

  void push_state(GameStates new_state);

  void pop_state();

  void replace_state(GameStates new_state);

private:
  std::stack<GameStates> states_;

  GameState* enum_to_state(GameStates state);

  LoadingState loading_;
  SplashState splash_;
  MenuState menu_;
  DummyState dummy_;
};

#endif // SNAKE_GAMESTATEMANAGER_HXX
