#include "GameStateManager.hxx"

GameStateManager::GameStateManager()
    :states_{{GameStates::Loading}}
{
  loading_.on_enter();
}

GameStateManager::~GameStateManager()
{
  while (!states_.empty()) {
    pop_state();
  }
}

GameState* GameStateManager::current()
{
  if (states_.empty())
    return nullptr;

  switch (states_.top()) {
  default:
    return nullptr; // TODO: handle all game states
  case GameStates::Loading:
    return &loading_;
  }
}

void GameStateManager::push_state(GameStates const new_state)
{
  states_.push(new_state);
  current()->on_enter();
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
