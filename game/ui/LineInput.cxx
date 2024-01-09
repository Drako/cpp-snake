#include "LineInput.hxx"

#include <cassert>

namespace {
  SDL_Rect const TEXTURE_RECTS[] = {
      {.x = 0, .y = 0, .w = 8, .h = 8}, // UPPER_LEFT
      {.x = 8, .y = 0, .w = 179, .h = 8}, // UPPER_EDGE
      {.x = 187, .y = 0, .w = 8, .h = 8}, // UPPER_RIGHT
      {.x = 0, .y = 8, .w = 8, .h = 34}, // LEFT_EDGE
      {.x = 8, .y = 8, .w = 179, .h = 34}, // CENTER
      {.x = 187, .y = 8, .w = 8, .h = 34}, // RIGHT_EDGE
      {.x = 0, .y = 42, .w = 8, .h = 7}, // LOWER_LEFT
      {.x = 8, .y = 42, .w = 179, .h = 7}, // LOWER_EDGE
      {.x = 187, .y = 42, .w = 8, .h = 7}, // LOWER_RIGHT
  };

  SDL_Rect calculate_text_rect(SDL_Rect const& area, SDL_Texture* texture)
  {
    int text_w, text_h;
    SDL_QueryTexture(texture, nullptr, nullptr, &text_w, &text_h);

    int const w = std::min(text_w, area.w-LineInput::MIN_WIDTH-8);
    int const h = std::min(text_h, area.h-LineInput::MIN_HEIGHT-8);

    return {
        .x = area.x+12,
        .y = area.y+(area.h-h)/2,
        .w = area.w>=(w+LineInput::MIN_WIDTH+8) ? w : 0,
        .h = area.h>=(h+LineInput::MIN_HEIGHT+8) ? h : 0,
    };
  }
}

LineInput::LineInput(int const x, int const y, int const w, int const h, std::string value)
    :value_{std::move(value)}, x_{x}, y_{y}, w_{w}, h_{h}, focus_{false},
     texture_{"line_input.png"},
     font_{"kenney_pixel.ttf"}
{
  assert(w_>=MIN_WIDTH);
  assert(h_>=MIN_HEIGHT);
}

void LineInput::move(int const x, int const y)
{
  x_ = x;
  y_ = y;
}

void LineInput::resize(int const w, int const h)
{
  assert(w>=MIN_WIDTH);
  assert(h>=MIN_HEIGHT);

  w_ = w;
  h_ = h;
}

SDL_Rect LineInput::get_bounding_box() const
{
  return {x_, y_, w_, h_};
}

void LineInput::set_focus(bool const focus)
{
  focus_ = focus;
}

bool LineInput::has_focus() const
{
  return focus_;
}

void LineInput::update(std::chrono::milliseconds const delta_time)
{
  blink_timer_ = (blink_timer_+delta_time)%6'000u;
}

void LineInput::render(SDLRenderer& renderer)
{
  auto const text = TTF_RenderUTF8_Solid(font_, value_.c_str(), {0, 0, 0, SDL_ALPHA_OPAQUE});
  auto const text_ure = SDL_CreateTextureFromSurface(renderer, text);
  SDL_FreeSurface(text);

  SDL_Texture* const background = texture_;
  SDL_Rect const target_rects[9] = {
      {.x = x_, .y = y_, .w = 8, .h = 8},
      {.x = x_+8, .y = y_, .w = w_-16, .h = 8},
      {.x = x_+w_-8, .y = y_, .w = 8, .h = 8},
      {.x = x_, .y = y_+8, .w = 8, .h = h_-15},
      {.x = x_+8, .y = y_+8, .w = w_-16, .h = h_-15},
      {.x = x_+w_-8, .y = y_+8, .w = 8, .h = h_-15},
      {.x = x_, .y = y_+h_-7, .w = 8, .h = 7},
      {.x = x_+8, .y = y_+h_-7, .w = w_-16, .h = 7},
      {.x = x_+w_-8, .y = y_+h_-7, .w = 8, .h = 7},
  };

  for (int n = 0; n<9; ++n)
    SDL_RenderCopy(renderer, background, ::TEXTURE_RECTS+n, target_rects+n);
  auto const text_rect = ::calculate_text_rect(target_rects[4], text_ure);
  SDL_Rect const text_texture_rect{
      .x = 0, .y = 0, .w = text_rect.w, .h = text_rect.h,
  };
  SDL_RenderCopy(renderer, text_ure, &text_texture_rect, &text_rect);

  SDL_DestroyTexture(text_ure);
}