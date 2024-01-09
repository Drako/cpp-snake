#include "Button.hxx"

#include "../../SDLRenderer.hxx"

#include <cassert>

namespace {
  SDL_Rect const TEXTURE_RECTS_UP[] = {
      {.x = 0, .y = 0, .w = 6, .h = 5}, // UPPER_LEFT
      {.x = 6, .y = 0, .w = 37, .h = 5}, // UPPER_EDGE
      {.x = 43, .y = 0, .w = 6, .h = 5}, // UPPER_RIGHT
      {.x = 0, .y = 5, .w = 6, .h = 35}, // LEFT_EDGE
      {.x = 6, .y = 5, .w = 37, .h = 35}, // CENTER
      {.x = 43, .y = 5, .w = 6, .h = 35}, // RIGHT_EDGE
      {.x = 0, .y = 40, .w = 6, .h = 9}, // LOWER_LEFT
      {.x = 6, .y = 40, .w = 37, .h = 9}, // LOWER_EDGE
      {.x = 43, .y = 40, .w = 6, .h = 9}, // LOWER_RIGHT
  };

  SDL_Rect const TEXTURE_RECTS_DOWN[] = {
      {.x = 0, .y = 0, .w = 6, .h = 5}, // UPPER_LEFT
      {.x = 6, .y = 0, .w = 37, .h = 5}, // UPPER_EDGE
      {.x = 43, .y = 0, .w = 6, .h = 5}, // UPPER_RIGHT
      {.x = 0, .y = 5, .w = 6, .h = 35}, // LEFT_EDGE
      {.x = 6, .y = 5, .w = 37, .h = 35}, // CENTER
      {.x = 43, .y = 5, .w = 6, .h = 35}, // RIGHT_EDGE
      {.x = 0, .y = 40, .w = 6, .h = 5}, // LOWER_LEFT
      {.x = 6, .y = 40, .w = 37, .h = 5}, // LOWER_EDGE
      {.x = 43, .y = 40, .w = 6, .h = 5}, // LOWER_RIGHT
  };

  SDL_Rect calculate_text_rect(SDL_Rect const& area, SDL_Texture* texture)
  {
    int text_w, text_h;
    SDL_QueryTexture(texture, nullptr, nullptr, &text_w, &text_h);

    float const text_aspect = static_cast<float>(text_w)/static_cast<float>(text_h);
    float const area_aspect = static_cast<float>(area.w)/static_cast<float>(area.h);

    int put_w, put_h;
    if (text_aspect>area_aspect) {
      put_w = area.w;
      put_h = static_cast<int>(static_cast<float>(put_w)/text_aspect);
    }
    else {
      put_h = area.h;
      put_w = static_cast<int>(static_cast<float>(put_h)*text_aspect);
    }

    return {
        .x = area.x+(area.w-put_w)/2,
        .y = area.y+(area.h-put_h)/2,
        .w = put_w,
        .h = put_h,
    };
  }
}

Button::Button(std::string title, int x, int y, int w, int h)
    :title_{std::move(title)}, x_{x}, y_{y}, w_{w}, h_{h}, pressed_{false},
     visible_{true},
     up_{"blue_button_up.png"},
     down_{"blue_button_down.png"},
     font_{"kenney_pixel.ttf"}
{
  assert(w_>=12);
  assert(h_>=14);

  on_click_ = [] { };
}

void Button::set_pressed(bool const pressed)
{
  pressed_ = pressed;
}

bool Button::is_pressed() const
{
  return pressed_;
}

void Button::render(SDLRenderer& renderer)
{
  if (!visible_)
    return;

  auto const text = TTF_RenderUTF8_Solid(font_, title_.c_str(), {0, 0, 0, SDL_ALPHA_OPAQUE});
  auto const text_ure = SDL_CreateTextureFromSurface(renderer, text);
  SDL_FreeSurface(text);

  SDL_Texture* texture;
  SDL_Rect const* texture_rects;
  SDL_Rect target_rects[9];
  if (pressed_) {
    texture = down_;
    texture_rects = ::TEXTURE_RECTS_DOWN;

    target_rects[0] = {.x = x_, .y = y_+4, .w = 6, .h = 5};
    target_rects[1] = {.x = x_+6, .y = y_+4, .w = w_-12, .h = 5};
    target_rects[2] = {.x = x_+w_-6, .y = y_+4, .w = 6, .h = 5};
    target_rects[3] = {.x = x_, .y = y_+9, .w = 6, .h = h_-14};
    target_rects[4] = {.x = x_+6, .y = y_+9, .w = w_-12, .h = h_-14};
    target_rects[5] = {.x = x_+w_-6, .y = y_+9, .w = 6, .h = h_-14};
    target_rects[6] = {.x = x_, .y = y_+h_-5, .w = 6, .h = 5};
    target_rects[7] = {.x = x_+6, .y = y_+h_-5, .w = w_-12, .h = 5};
    target_rects[8] = {.x = x_+w_-6, .y = y_+h_-5, .w = 6, .h = 5};
  }
  else {
    texture = up_;
    texture_rects = ::TEXTURE_RECTS_UP;

    target_rects[0] = {.x = x_, .y = y_, .w = 6, .h = 5};
    target_rects[1] = {.x = x_+6, .y = y_, .w = w_-12, .h = 5};
    target_rects[2] = {.x = x_+w_-6, .y = y_, .w = 6, .h = 5};
    target_rects[3] = {.x = x_, .y = y_+5, .w = 6, .h = h_-14};
    target_rects[4] = {.x = x_+6, .y = y_+5, .w = w_-12, .h = h_-14};
    target_rects[5] = {.x = x_+w_-6, .y = y_+5, .w = 6, .h = h_-14};
    target_rects[6] = {.x = x_, .y = y_+h_-9, .w = 6, .h = 9};
    target_rects[7] = {.x = x_+6, .y = y_+h_-9, .w = w_-12, .h = 9};
    target_rects[8] = {.x = x_+w_-6, .y = y_+h_-9, .w = 6, .h = 9};
  }

  for (int n = 0; n<9; ++n)
    SDL_RenderCopy(renderer, texture, texture_rects+n, target_rects+n);

  SDL_Rect const text_rect = ::calculate_text_rect(target_rects[4], text_ure);
  SDL_RenderCopy(renderer, text_ure, nullptr, &text_rect);

  SDL_DestroyTexture(text_ure);
}

void Button::update()
{
  if (!visible_)
    return;

  int mouse_x, mouse_y;
  auto const mouse_button = SDL_GetMouseState(&mouse_x, &mouse_y);

  if ((mouse_x>=x_ && mouse_x<=(x_+w_) && mouse_y>=y_ && mouse_y<=(y_+h_))) {
    if (mouse_button & SDL_BUTTON(SDL_BUTTON_LEFT)) {
      pressed_ = true;
    }
    else {
      if (pressed_) {
        trigger();
      }
      pressed_ = false;
    }
  }
  else {
    pressed_ = false;
  }
}

void Button::trigger()
{
  on_click_();
}

void Button::set_on_click(std::function<void()> handler)
{
  on_click_ = std::move(handler);
}

void Button::move(int const x, int const y)
{
  x_ = x;
  y_ = y;
}

void Button::resize(int const w, int const h)
{
  w_ = w;
  h_ = h;
}

SDL_Rect Button::get_bounding_box() const
{
  return {x_, y_, w_, h_};
}

void Button::set_visible(bool const visible)
{
  visible_ = visible;
}

bool Button::is_visible() const
{
  return visible_;
}
