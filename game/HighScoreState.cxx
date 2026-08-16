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
  if (event.type==SDL_EVENT_KEY_UP) {
    switch (event.key.scancode) {
    default:
      break;
    case SDL_SCANCODE_ESCAPE:
      [[fallthrough]];
    case SDL_SCANCODE_SPACE:
      [[fallthrough]];
    case SDL_SCANCODE_RETURN:
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

void HighScoreState::render(SDLRenderer& renderer)
{
  TTF_Font* const font = font_;

  SDL_Rect viewport;
  SDL_GetRenderViewport(renderer, &viewport);
  int width = viewport.w;

  SDL_Color const color = {255, 255, 255, SDL_ALPHA_OPAQUE};

  render_heading(renderer, width, color);

  auto const scores = HighScoreManager::instance().get_scores();
  for (auto n = scores.size(); n--;) {
    auto const current_height = 180+static_cast<int>(n)*50;
    auto const& score = scores[n];

    std::string const text = std::to_string(n+1)+" - "+score.player_name_+": "+std::to_string(score.points_);
    SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), text.length(), color);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

    SDL_FRect textRect;
    textRect.w = surface->w;
    textRect.h = surface->h;
    textRect.x = (width-textRect.w)/2;
    textRect.y = current_height;

    SDL_RenderTexture(renderer, texture, nullptr, &textRect);

    SDL_DestroySurface(surface);
    SDL_DestroyTexture(texture);
  }
}

void HighScoreState::render_heading(SDLRenderer& renderer, int const width, SDL_Color const& color)
{
  std::string txt = TranslationManager::instance().get_translation("High Scores");
  SDL_Surface* headingSurface = TTF_RenderText_Solid(font_,
      txt.c_str(), txt.length(), color);
  SDL_Texture* headingTexture = SDL_CreateTextureFromSurface(renderer, headingSurface);

  SDL_FRect heading_rect;
  heading_rect.w = headingSurface->w*2;
  heading_rect.h = headingSurface->h*2;
  heading_rect.x = (width-heading_rect.w)/2;
  heading_rect.y = 80;

  SDL_RenderTexture(renderer, headingTexture, nullptr, &heading_rect);

  SDL_DestroySurface(headingSurface);
  SDL_DestroyTexture(headingTexture);
}
