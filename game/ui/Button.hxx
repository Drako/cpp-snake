#pragma once

#ifndef SNAKE_BUTTON_HXX
#define SNAKE_BUTTON_HXX

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

#include <functional>
#include <string>

#include "../../NonCopyable.hxx"

#include "UiColor.hxx"
#include "../AssetManager.hxx"

class Button final : private NonCopyable {
public:
  static float constexpr MIN_WIDTH = 12;
  static float constexpr MIN_HEIGHT = 14;

  Button(float x, float y, float w, float h, UiColor color = UiColor::Grey);

  void set_title(std::string const& title);

  [[nodiscard]] std::string const& title() const;

  void set_pressed(bool pressed);

  [[nodiscard]] bool is_pressed() const;

  void set_visible(bool visible);

  [[nodiscard]] bool is_visible() const;

  void update();

  void render(SDLRenderer& renderer);

  void trigger();

  void set_on_click(std::function<void()> handler);

  void move(int x, int y);

  void resize(int w, int h);

  [[nodiscard]] SDL_FRect get_bounding_box() const;

private:
  std::string title_{};
  float x_, y_, w_, h_;
  bool pressed_;
  bool visible_;
  std::function<void()> on_click_;

  Asset<SDL_Texture*> up_;
  Asset<SDL_Texture*> down_;
  Asset<TTF_Font*> font_;
};

#endif // SNAKE_BUTTON_HXX
