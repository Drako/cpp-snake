#pragma once

#ifndef SNAKE_BUTTON_HXX
#define SNAKE_BUTTON_HXX

#include <SDL.h>
#include <SDL_ttf.h>

#include <functional>
#include <future>
#include <string>

class SDLRenderer;

class Button final {
public:
  Button(std::string title, int x, int y, int w, int h);

  void set_pressed(bool pressed);

  bool is_pressed() const;

  void update();

  void render(SDLRenderer& renderer);

  void trigger();

  void set_on_click(std::function<void()> handler);

private:
  std::string title_;
  int x_, y_, w_, h_;
  bool pressed_;
  std::function<void()> on_click_;

  std::shared_future<SDL_Texture*> up_;
  std::shared_future<SDL_Texture*> down_;
  std::shared_future<TTF_Font*> font_;
};

#endif // SNAKE_BUTTON_HXX
