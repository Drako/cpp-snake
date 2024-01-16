#include "HighScoreState.hxx"

#include "HighScoreManager.hxx"
#include "GameStateManager.hxx"
#include "TranslationManager.hxx"
#include "../SDLRenderer.hxx"

HighScoreState::HighScoreState()
    :font_{"kenney_pixel.ttf"}
{
}

void HighScoreState::on_event(GameStateManager& gsm, SDL_Event const& event)
{
  if (event.type==SDL_KEYUP) {
    switch (event.key.keysym.scancode) {
    default:
      break;
    case SDL_SCANCODE_ESCAPE:
      [[fallthrough]];
    case SDL_SCANCODE_RETURN:
      gsm.pop_state();
      break;
    }
  }
}

void HighScoreState::render(SDLRenderer& renderer)
{
  TTF_Font* const font = font_;

  SDL_Rect viewport;
  SDL_RenderGetViewport(renderer, &viewport);
  int width = viewport.w;

  SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
  SDL_RenderClear(renderer);

  SDL_Color const color = {255, 255, 255, SDL_ALPHA_OPAQUE};

  int current_height = 80;
  render_heading(renderer, width, color, current_height);

  auto const scores = HighScoreManager::instance().get_scores();
  for (auto const& score: scores) {
    std::string text = score.player_name_+": "+std::to_string(score.points_);
    SDL_Surface* surface = TTF_RenderUTF8_Solid(font, text.c_str(), color);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

    SDL_Rect textRect;
    textRect.w = surface->w;
    textRect.h = surface->h;
    textRect.x = (width-textRect.w)/2;
    textRect.y = current_height;

    SDL_RenderCopy(renderer, texture, nullptr, &textRect);

    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);

    current_height += textRect.h;
  }

  SDL_RenderPresent(renderer);
}

void HighScoreState::render_heading(SDLRenderer& renderer, int const width, SDL_Color const& color, int& current_height)
{
  SDL_Surface* headingSurface = TTF_RenderUTF8_Solid(font_,
      TranslationManager::instance().get_translation("High Scores").c_str(), color);
  SDL_Texture* headingTexture = SDL_CreateTextureFromSurface(renderer, headingSurface);

  SDL_Rect heading_rect;
  heading_rect.w = headingSurface->w*2;
  heading_rect.h = headingSurface->h*2;
  heading_rect.x = (width-heading_rect.w)/2;
  heading_rect.y = current_height;

  SDL_RenderCopy(renderer, headingTexture, nullptr, &heading_rect);

  SDL_FreeSurface(headingSurface);
  SDL_DestroyTexture(headingTexture);

  current_height += heading_rect.h;
}
