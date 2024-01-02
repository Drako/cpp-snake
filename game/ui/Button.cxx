#include "Button.hxx"

#include "../AssetManager.hxx"

#include "../../SDLRenderer.hxx"

#include <cassert>

namespace {
  SDL_Rect const UPPER_LEFT{
      .x = 0,
      .y = 0,
      .w = 6,
      .h = 5,
  };

  SDL_Rect const UPPER_EDGE{
      .x = 6,
      .y = 0,
      .w = 37,
      .h = 5,
  };

  SDL_Rect const UPPER_RIGHT{
      .x = 43,
      .y = 0,
      .w = 6,
      .h = 5,
  };

  SDL_Rect const LEFT_EDGE{
      .x = 0,
      .y = 5,
      .w = 6,
      .h = 35,
  };

  SDL_Rect const CENTER{
      .x = 6,
      .y = 5,
      .w = 37,
      .h = 35,
  };

  SDL_Rect const RIGHT_EDGE{
      .x = 43,
      .y = 5,
      .w = 6,
      .h = 35,
  };

  SDL_Rect const LOWER_LEFT_UP{
      .x = 0,
      .y = 40,
      .w = 6,
      .h = 9,
  };

  SDL_Rect const LOWER_EDGE_UP{
      .x = 6,
      .y = 40,
      .w = 37,
      .h = 9,
  };

  SDL_Rect const LOWER_RIGHT_UP{
      .x = 43,
      .y = 40,
      .w = 6,
      .h = 9,
  };

  SDL_Rect const LOWER_LEFT_DOWN{
      .x = 0,
      .y = 40,
      .w = 6,
      .h = 5,
  };

  SDL_Rect const LOWER_EDGE_DOWN{
      .x = 6,
      .y = 40,
      .w = 37,
      .h = 5,
  };

  SDL_Rect const LOWER_RIGHT_DOWN{
      .x = 43,
      .y = 40,
      .w = 6,
      .h = 5,
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
    :title_{std::move(title)}, x_{x}, y_{y}, w_{w}, h_{h}, pressed_{false}
{
  assert(w_>=12);
  assert(h_>=14);

  on_click_ = [] { };

  up_ = std::async(std::launch::deferred, [] {
    return AssetManager::instance().get_texture_asset("blue_button_up.png");
  });
  down_ = std::async(std::launch::deferred, [] {
    return AssetManager::instance().get_texture_asset("blue_button_down.png");
  });
  font_ = std::async(std::launch::deferred, [] {
    return AssetManager::instance().get_font_asset("kenney_pixel.ttf");
  });
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
  auto const text = TTF_RenderUTF8_Solid(font_.get(), title_.c_str(), {0, 0, 0, SDL_ALPHA_OPAQUE});
  auto const text_ure = SDL_CreateTextureFromSurface(renderer, text);
  SDL_FreeSurface(text);

  if (pressed_) {
    auto const down = down_.get();

    SDL_Rect const upper_left{
        .x = x_,
        .y = y_+4,
        .w = 6,
        .h = 5,
    };
    SDL_RenderCopy(renderer, down, &::UPPER_LEFT, &upper_left);

    SDL_Rect const upper_edge{
        .x = x_+6,
        .y = y_+4,
        .w = w_-12,
        .h = 5,
    };
    SDL_RenderCopy(renderer, down, &::UPPER_EDGE, &upper_edge);

    SDL_Rect const upper_right{
        .x = x_+w_-6,
        .y = y_+4,
        .w = 6,
        .h = 5,
    };
    SDL_RenderCopy(renderer, down, &::UPPER_RIGHT, &upper_right);

    SDL_Rect const left_edge{
        .x = x_,
        .y = y_+9,
        .w = 6,
        .h = h_-14,
    };
    SDL_RenderCopy(renderer, down, &::LEFT_EDGE, &left_edge);

    SDL_Rect const center{
        .x = x_+6,
        .y = y_+9,
        .w = w_-12,
        .h = h_-14,
    };
    SDL_RenderCopy(renderer, down, &::CENTER, &center);

    SDL_Rect const text_rect = ::calculate_text_rect(center, text_ure);
    SDL_RenderCopy(renderer, text_ure, nullptr, &text_rect);

    SDL_Rect const right_edge{
        .x = x_+w_-6,
        .y = y_+9,
        .w = 6,
        .h = h_-14,
    };
    SDL_RenderCopy(renderer, down, &::RIGHT_EDGE, &right_edge);

    SDL_Rect const lower_left{
        .x = x_,
        .y = y_+h_-5,
        .w = 6,
        .h = 5,
    };
    SDL_RenderCopy(renderer, down, &::LOWER_LEFT_DOWN, &lower_left);

    SDL_Rect const lower_edge{
        .x = x_+6,
        .y = y_+h_-5,
        .w = w_-12,
        .h = 5,
    };
    SDL_RenderCopy(renderer, down, &::LOWER_EDGE_DOWN, &lower_edge);

    SDL_Rect const lower_right{
        .x = x_+w_-6,
        .y = y_+h_-5,
        .w = 6,
        .h = 5,
    };
    SDL_RenderCopy(renderer, down, &::LOWER_RIGHT_DOWN, &lower_right);
  }
  else {
    auto const up = up_.get();

    SDL_Rect const upper_left{
        .x = x_,
        .y = y_,
        .w = 6,
        .h = 5,
    };
    SDL_RenderCopy(renderer, up, &::UPPER_LEFT, &upper_left);

    SDL_Rect const upper_edge{
        .x = x_+6,
        .y = y_,
        .w = w_-12,
        .h = 5,
    };
    SDL_RenderCopy(renderer, up, &::UPPER_EDGE, &upper_edge);

    SDL_Rect const upper_right{
        .x = x_+w_-6,
        .y = y_,
        .w = 6,
        .h = 5,
    };
    SDL_RenderCopy(renderer, up, &::UPPER_RIGHT, &upper_right);

    SDL_Rect const left_edge{
        .x = x_,
        .y = y_+5,
        .w = 6,
        .h = h_-14,
    };
    SDL_RenderCopy(renderer, up, &::LEFT_EDGE, &left_edge);

    SDL_Rect const center{
        .x = x_+6,
        .y = y_+5,
        .w = w_-12,
        .h = h_-14,
    };
    SDL_RenderCopy(renderer, up, &::CENTER, &center);

    SDL_Rect const text_rect = ::calculate_text_rect(center, text_ure);
    SDL_RenderCopy(renderer, text_ure, nullptr, &text_rect);

    SDL_Rect const right_edge{
        .x = x_+w_-6,
        .y = y_+5,
        .w = 6,
        .h = h_-14,
    };
    SDL_RenderCopy(renderer, up, &::RIGHT_EDGE, &right_edge);

    SDL_Rect const lower_left{
        .x = x_,
        .y = y_+h_-9,
        .w = 6,
        .h = 9,
    };
    SDL_RenderCopy(renderer, up, &::LOWER_LEFT_UP, &lower_left);

    SDL_Rect const lower_edge{
        .x = x_+6,
        .y = y_+h_-9,
        .w = w_-12,
        .h = 9,
    };
    SDL_RenderCopy(renderer, up, &::LOWER_EDGE_UP, &lower_edge);

    SDL_Rect const lower_right{
        .x = x_+w_-6,
        .y = y_+h_-9,
        .w = 6,
        .h = 9,
    };
    SDL_RenderCopy(renderer, up, &::LOWER_RIGHT_UP, &lower_right);
  }

  SDL_DestroyTexture(text_ure);
}

void Button::update()
{
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
