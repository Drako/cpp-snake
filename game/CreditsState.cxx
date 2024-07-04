#include "CreditsState.hxx"

#include "GameStateManager.hxx"

#include <type_traits>

CreditsState::CreditsState()
    :font_{"kenney_pixel.ttf"}, jetbrains_ai_logo_{"jetbrains-ai-logo.png"}, kenney_logo_{"kenney-logo.png"},
     sdl_logo_{"SDL_logo.png"}
{
}

void CreditsState::on_enter(GameStateManager& gsm)
{
  TTF_Font* const font = font_;

  scroll_y_ = 0.0;
  done_ = false;
  scroll_items_ = {
      "Copyright © 2024, Felix Bytow <drako@drako.guru>",
      External{jetbrains_ai_logo_, "with some help from JetBrains AI Assistant"},
      External{kenney_logo_, "Font & UI Pack from kenney.nl"},
      sdl_logo_,
  };

  int summed_size = -ITEM_PADDING;
  for (auto const& item: scroll_items_) {
    summed_size += std::visit([font]<typename T>(T const& it) {
      int w, h = 0;
      if constexpr (std::is_same_v<T, char const*>) {
        TTF_SizeUTF8(font, it, &w, &h);
      }
      else if constexpr (std::is_same_v<T, External>) {
        int logo_h, text_h;
        TTF_SizeUTF8(font, it.text_, &w, &text_h);
        SDL_QueryTexture(it.texture_, nullptr, nullptr, &w, &logo_h);
        h = logo_h+INNER_ITEM_PADDING+text_h;
      }
      else if constexpr (std::is_same_v<T, SDL_Texture*>) {
        SDL_QueryTexture(it, nullptr, nullptr, &w, &h);
      }
      return h;
    }, item);
    summed_size += ITEM_PADDING;
  }
  scroll_size_ = static_cast<double>(summed_size);
}

void CreditsState::on_event(GameStateManager& gsm, SDL_Event const& event)
{
  if (event.type==SDL_KEYUP) {
    switch (event.key.keysym.scancode) {
    default:
      break;
    case SDL_SCANCODE_ESCAPE:
      [[fallthrough]];
    case SDL_SCANCODE_RETURN:
      [[fallthrough]];
    case SDL_SCANCODE_SPACE:
      gsm.pop_state();
      break;
    }
  }
  else if (event.type==SDL_CONTROLLERBUTTONUP) {
    switch (event.cbutton.which) {
    default:
      break;
    case SDL_CONTROLLER_BUTTON_A:
      [[fallthrough]];
    case SDL_CONTROLLER_BUTTON_B:
      [[fallthrough]];
    case SDL_CONTROLLER_BUTTON_START:
      gsm.pop_state();
      break;
    }
  }
}

void CreditsState::update(GameStateManager& gsm, std::chrono::milliseconds const delta_time)
{
  if (done_) {
    gsm.pop_state();
  }

  scroll_y_ += 0.05*static_cast<double>(delta_time.count());
}

void CreditsState::render(SDLRenderer& renderer)
{
  TTF_Font* const font = font_;

  int window_width = 0, window_height = 0;
  SDL_GetRendererOutputSize(renderer, &window_width, &window_height);

  int y = window_height-static_cast<int>(scroll_y_);
  if (y<=-static_cast<int>(scroll_size_)) {
    // everything is now outside the screen at the top
    done_ = true;
  }

  for (auto const& item: scroll_items_) {
    std::visit([&renderer, &y, window_width, font]<typename T>(T const& it) {
      int h = 0;
      if constexpr (std::is_same_v<T, char const*>) {
        auto const surface = TTF_RenderUTF8_Solid(font, it, {255, 255, 255, SDL_ALPHA_OPAQUE});
        auto const texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);

        int w;
        SDL_QueryTexture(texture, nullptr, nullptr, &w, &h);
        SDL_Rect const rect{.x = (window_width-w)/2, .y = y, .w = w, .h = h};
        SDL_RenderCopy(renderer, texture, nullptr, &rect);

        SDL_DestroyTexture(texture);
      }
      else if constexpr (std::is_same_v<T, External>) {
        int w, logo_h, text_h;

        SDL_QueryTexture(it.texture_, nullptr, nullptr, &w, &logo_h);
        SDL_Rect const logo_rect{.x = (window_width-w)/2, .y = y, .w = w, .h = logo_h};
        SDL_RenderCopy(renderer, it.texture_, nullptr, &logo_rect);

        auto const surface = TTF_RenderUTF8_Solid(font, it.text_, {255, 255, 255, SDL_ALPHA_OPAQUE});
        auto const texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);

        SDL_QueryTexture(texture, nullptr, nullptr, &w, &text_h);
        SDL_Rect const text_rect{.x = (window_width-w)/2, .y = y+logo_h+INNER_ITEM_PADDING, .w = w, .h = text_h};
        SDL_RenderCopy(renderer, texture, nullptr, &text_rect);

        SDL_DestroyTexture(texture);

        h = logo_h+INNER_ITEM_PADDING+text_h;
      }
      else if constexpr (std::is_same_v<T, SDL_Texture*>) {
        int w;
        SDL_QueryTexture(it, nullptr, nullptr, &w, &h);
        SDL_Rect const rect{.x = (window_width-w)/2, .y = y, .w = w, .h = h};
        SDL_RenderCopy(renderer, it, nullptr, &rect);
      }

      y += h+ITEM_PADDING;
    }, item);
  }
}
