#pragma once

#ifndef SNAKE_LINEINPUT_HXX
#define SNAKE_LINEINPUT_HXX

#include <chrono>

#include <boost/noncopyable.hpp>

#include "../AssetManager.hxx"

class LineInput final : private boost::noncopyable {
public:
  static int constexpr MIN_WIDTH = 16;
  static int constexpr MIN_HEIGHT = 15;

  LineInput(int x, int y, int w, int h, std::string value = "");

  void on_event(SDL_Event const& evt);

  void update(std::chrono::milliseconds delta_time);

  void render(SDLRenderer& renderer);

  void move(int x, int y);

  void resize(int w, int h);

  [[nodiscard]] SDL_Rect get_bounding_box() const;

  // this does not remove the focus from other inputs!
  void set_focus(bool focus);

  [[nodiscard]] bool has_focus() const;

  void set_visible(bool visible);

  [[nodiscard]] bool is_visible() const;

  void set_value(std::string value);

  [[nodiscard]]char const* value() const;

private:
  std::string value_;
  int x_, y_, w_, h_;
  bool focus_;
  bool visible_;

  Asset<SDL_Texture*> texture_;
  Asset<TTF_Font*> font_;

  std::chrono::milliseconds blink_timer_{0};
};

#endif // SNAKE_LINEINPUT_HXX
