#include "GameStateManager.hxx"

#include <cassert>

GameStateManager::GameStateManager()
    :states_{{GameStates::Loading}}
{
  loading_.on_enter(*this);
}

GameStateManager::~GameStateManager()
{
  while (!states_.empty()) {
    pop_state();
  }
}

GameState* GameStateManager::enum_to_state(GameStates const state)
{
  switch (state) {
  default:
    // this should no longer be reachable as all states are handled
    assert(false);
    return nullptr;
  case GameStates::Loading:
    return &loading_;
  case GameStates::Splash:
    return &splash_;
  case GameStates::MainMenu:
    return &menu_;
  case GameStates::Game:
    return &game_;
  case GameStates::GameOver:
    return &game_over_;
  case GameStates::HighScores:
    return &high_score_;
  case GameStates::Credits:
    return &credits_;
  }
}

GameState* GameStateManager::current()
{
  if (states_.empty())
    return nullptr;

  return enum_to_state(states_.top());
}

GameState* GameStateManager::parent()
{
  if (states_.size()<2)
    return nullptr;

  auto const current = states_.top();
  states_.pop();
  auto const parent = states_.top();
  states_.push(current);

  return enum_to_state(parent);
}

void GameStateManager::push_state(GameStates const new_state)
{
  states_.push(new_state);
  current()->on_enter(*this);
}

void GameStateManager::pop_state()
{
  auto const state = current();
  if (state!=nullptr) {
    state->on_leave();
    states_.pop();
  }
}

void GameStateManager::replace_state(GameStates const new_state)
{
  pop_state();
  push_state(new_state);
}
