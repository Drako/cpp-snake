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
      int w, h;
      if constexpr (std::is_same_v<T, char const*>) {
        TTF_GetStringSize(font, it, 0, &w, &h);
      }
      else if constexpr (std::is_same_v<T, External>) {
        int text_h;
        float logo_w, logo_h;
        TTF_GetStringSize(font, it.text_, 0, &w, &text_h);
        SDL_GetTextureSize(it.texture_, &logo_w, &logo_h);
        h = logo_h+INNER_ITEM_PADDING+text_h;
      }
      else if constexpr (std::is_same_v<T, SDL_Texture*>) {
        float logo_w, logo_h;
        SDL_GetTextureSize(it, &logo_w, &logo_h);
        h = static_cast<int>(logo_h);
      }
      return h;
    }, item);
    summed_size += ITEM_PADDING;
  }
  scroll_size_ = static_cast<float>(summed_size);
}

void CreditsState::on_event(GameStateManager& gsm, SDL_Event const& event)
{
  if (event.type==SDL_EVENT_KEY_UP) {
    switch (event.key.key) {
    default:
      break;
    case SDLK_ESCAPE:
      [[fallthrough]];
    case SDLK_RETURN:
      [[fallthrough]];
    case SDLK_SPACE:
      gsm.pop_state();
      break;
    }
  }
  else if (event.type==SDL_EVENT_GAMEPAD_BUTTON_UP) {
    switch (event.gbutton.button) {
    default:
      break;
    case SDL_GAMEPAD_BUTTON_SOUTH:
      [[fallthrough]];
    case SDL_GAMEPAD_BUTTON_WEST:
      [[fallthrough]];
    case SDL_GAMEPAD_BUTTON_START:
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
  SDL_GetCurrentRenderOutputSize(renderer, &window_width, &window_height);

  float y = static_cast<float>(window_height)-scroll_y_;
  if (y<=-scroll_size_) {
    // everything is now outside the screen at the top
    done_ = true;
  }

  for (auto const& item: scroll_items_) {
    std::visit([&renderer, &y, window_width, font]<typename T>(T const& it) {
      float h = 0.f;
      float w = 0.f;
      if constexpr (std::is_same_v<T, char const*>) {
        auto const surface = TTF_RenderText_Solid(font, it, 0, {255, 255, 255, SDL_ALPHA_OPAQUE});
        auto const texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_DestroySurface(surface);

        SDL_GetTextureSize(texture, &w, &h);
        SDL_FRect const rect{.x = (window_width-w)/2, .y = y, .w = w, .h = h};
        SDL_RenderTexture(renderer, texture, nullptr, &rect);

        SDL_DestroyTexture(texture);
      }
      else if constexpr (std::is_same_v<T, External>) {
        float logo_h, text_h;

        SDL_GetTextureSize(it.texture_, &w, &logo_h);
        SDL_FRect const logo_rect{.x = (window_width-w)/2, .y = y, .w = w, .h = logo_h};
        SDL_RenderTexture(renderer, it.texture_, nullptr, &logo_rect);

        auto const surface = TTF_RenderText_Solid(font, it.text_, 0, {255, 255, 255, SDL_ALPHA_OPAQUE});
        auto const texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_DestroySurface(surface);

        SDL_GetTextureSize(texture, &w, &text_h);
        SDL_FRect const text_rect{.x = (window_width-w)/2, .y = y+logo_h+INNER_ITEM_PADDING, .w = w, .h = text_h};
        SDL_RenderTexture(renderer, texture, nullptr, &text_rect);

        SDL_DestroyTexture(texture);

        h = logo_h+INNER_ITEM_PADDING+text_h;
      }
      else if constexpr (std::is_same_v<T, SDL_Texture*>) {
        SDL_GetTextureSize(it, &w, &h);
        SDL_FRect const rect{.x = (window_width-w)/2, .y = y, .w = w, .h = h};
        SDL_RenderTexture(renderer, it, nullptr, &rect);
      }

      y += h+ITEM_PADDING;
    }, item);
  }
}
