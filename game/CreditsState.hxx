#pragma once

#ifndef SNAKE_CREDITSSTATE_HXX
#define SNAKE_CREDITSSTATE_HXX

#include "GameState.hxx"

#include "AssetManager.hxx"

#include <variant>
#include <vector>

struct External final {
  SDL_Texture* texture_;
  char const* text_;
};

class CreditsState final : public GameState {
public:
  static int constexpr ITEM_PADDING = 80;
  static int constexpr INNER_ITEM_PADDING = 10;

  CreditsState();

  void on_enter(GameStateManager& gsm) override;

  void on_event(GameStateManager& gsm, SDL_Event const& event) override;

  void update(GameStateManager& gsm, std::chrono::milliseconds delta_time) override;

  void render(SDLRenderer& renderer) override;

private:
  Asset<TTF_Font*> font_;

  Asset<SDL_Texture*> jetbrains_ai_logo_;
  Asset<SDL_Texture*> kenney_logo_;
  Asset<SDL_Texture*> sdl_logo_;
  Asset<SDL_Texture*> boost_logo_;

  std::vector<std::variant<char const*, External, SDL_Texture*>> scroll_items_{};

  double scroll_y_{0.0};
  double scroll_size_{0.0};
  // this is a hack because we detect being done in render() and use that info in update()
  bool done_{false};
};

#endif // SNAKE_CREDITSSTATE_HXX
