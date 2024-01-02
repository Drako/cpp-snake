#include "SplashState.hxx"
#include "AssetManager.hxx"
#include "GameStateManager.hxx"

using namespace std::chrono_literals;

void SplashState::update(GameStateManager& gsm, std::chrono::milliseconds delta_time)
{
  time_in_state_ += delta_time;
  auto const key_state = SDL_GetKeyboardState(nullptr);
  if (time_in_state_ > 13'000ms || key_state[SDL_SCANCODE_SPACE] || key_state[SDL_SCANCODE_RETURN])
    gsm.replace_state(GameStates::MainMenu);
}

void SplashState::render(SDLRenderer& renderer)
{
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
  SDL_RenderClear(renderer);

  int w, h;
  SDL_GetRendererOutputSize(renderer, &w, &h);

  if (time_in_state_ >= 2'000ms && time_in_state_ < 4'000ms)
  {
    auto const progress = static_cast<float>(time_in_state_.count() - 2'000) / 2'000.0f;
    auto const alpha = static_cast<int>(std::lerp(SDL_ALPHA_TRANSPARENT, SDL_ALPHA_OPAQUE, progress));
    SDL_SetTextureAlphaMod(logo_, alpha);
    SDL_RenderCopy(renderer, logo_, nullptr, nullptr);
  }
  else if (time_in_state_ >= 4'000ms && time_in_state_ < 9'000ms)
  {
    SDL_SetTextureAlphaMod(logo_, SDL_ALPHA_OPAQUE);
    SDL_RenderCopy(renderer, logo_, nullptr, nullptr);
  }
  else if (time_in_state_ >= 9'000ms && time_in_state_ < 11'000ms)
  {
    auto const progress = static_cast<float>(time_in_state_.count() - 11'000) / 2'000.0f;
    auto const alpha = static_cast<int>(std::lerp(SDL_ALPHA_OPAQUE, SDL_ALPHA_TRANSPARENT, progress));
    SDL_SetTextureAlphaMod(logo_, alpha);
    SDL_RenderCopy(renderer, logo_, nullptr, nullptr);
  }

  SDL_RenderPresent(renderer);
}

void SplashState::on_enter()
{
  time_in_state_ = 0ms;
  logo_ = AssetManager::instance().get_texture_asset("logo.jpg");
  SDL_SetTextureBlendMode(logo_, SDL_BLENDMODE_BLEND);
}
