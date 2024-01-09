#pragma once

#ifndef SNAKE_BUTTON_HXX
#define SNAKE_BUTTON_HXX

#include <SDL.h>
#include <SDL_ttf.h>

#include <functional>
#include <string>

#include "../AssetManager.hxx"

class SDLRenderer;

class Button final {
public:
  Button(std::string title, int x, int y, int w, int h);

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

  [[nodiscard]] SDL_Rect get_bounding_box() const;

private:
  std::string title_;
  int x_, y_, w_, h_;
  bool pressed_;
  bool visible_;
  std::function<void()> on_click_;

  Asset<SDL_Texture*> up_;
  Asset<SDL_Texture*> down_;
  Asset<TTF_Font*> font_;
};

#endif // SNAKE_BUTTON_HXX
