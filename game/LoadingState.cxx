#include "LoadingState.hxx"
#include "AssetManager.hxx"
#include "GameStateManager.hxx"

void LoadingState::update(GameStateManager& gsm, std::chrono::milliseconds const delta_time)
{
  (void) delta_time;
  if (AssetManager::instance().get_progress()==1.0f) {
    gsm.replace_state(GameStates::Splash);
  }
}

void LoadingState::render(SDLRenderer& renderer)
{
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
  SDL_RenderClear(renderer);

  static int const BAR_MARGIN = 30;
  static int const BAR_HEIGHT = 50;

  int w, h;
  SDL_GetRendererOutputSize(renderer, &w, &h);
  float progress = AssetManager::instance().get_progress();

  SDL_Rect const outer_rect = {
      .x=BAR_MARGIN,
      .y=h-BAR_MARGIN-BAR_HEIGHT,
      .w=w-(BAR_MARGIN*2),
      .h=BAR_HEIGHT
  };
  SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
  SDL_RenderFillRect(renderer, &outer_rect);

  SDL_Rect const inner_rect = {
      .x=BAR_MARGIN+2,
      .y=h-BAR_MARGIN-BAR_HEIGHT+2,
      .w=w-(BAR_MARGIN*2)-4,
      .h=BAR_HEIGHT-4
  };
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
  SDL_RenderFillRect(renderer, &inner_rect);

  SDL_Rect const progress_rect = {
      .x=BAR_MARGIN+4,
      .y=h-BAR_MARGIN-BAR_HEIGHT+4,
      .w=static_cast<int>(std::lerp(0, w-(BAR_MARGIN*2)-8, progress)),
      .h=BAR_HEIGHT-8
  };
  SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
  SDL_RenderFillRect(renderer, &progress_rect);

  SDL_RenderPresent(renderer);
}