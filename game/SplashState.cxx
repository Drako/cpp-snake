#include "SplashState.hxx"
#include "AssetManager.hxx"
#include "GameStateManager.hxx"

using namespace std::chrono_literals;

void SplashState::on_event(GameStateManager& gsm, SDL_Event const& evt)
{
  switch (evt.type) {
  default:
    break;
  case SDL_EVENT_KEY_UP:
    switch (evt.key.key) {
    default:
      break;
    case SDLK_SPACE:
      [[fallthrough]];
    case SDLK_RETURN:
      [[fallthrough]];
    case SDLK_ESCAPE:
      gsm.replace_state(GameStates::MainMenu);
      break;
    }
    break;
  case SDL_EVENT_GAMEPAD_BUTTON_UP:
    switch (evt.gbutton.button) {
    default:
      break;
    case SDL_GAMEPAD_BUTTON_SOUTH:
      [[fallthrough]];
    case SDL_GAMEPAD_BUTTON_EAST:
      [[fallthrough]];
    case SDL_GAMEPAD_BUTTON_START:
      gsm.replace_state(GameStates::MainMenu);
      break;
    }
    break;
  }
}

void SplashState::update(GameStateManager& gsm, std::chrono::milliseconds delta_time)
{
  time_in_state_ += delta_time;
  if (time_in_state_>13'000ms)
    gsm.replace_state(GameStates::MainMenu);
}

void SplashState::render(SDLRenderer& renderer)
{
  if (time_in_state_>=2'000ms && time_in_state_<11'000ms) {
    if (time_in_state_<4'000ms) {
      auto const progress = static_cast<float>(time_in_state_.count()-2'000)/2'000.0f;
      auto const alpha = static_cast<int>(std::lerp(SDL_ALPHA_TRANSPARENT, SDL_ALPHA_OPAQUE, progress));
      SDL_SetTextureAlphaMod(logo_, alpha);
    }
    else if (time_in_state_>=4'000ms && time_in_state_<9'000ms) {
      SDL_SetTextureAlphaMod(logo_, SDL_ALPHA_OPAQUE);
    }
    else if (time_in_state_>=9'000ms) {
      auto const progress = static_cast<float>(time_in_state_.count()-9'000)/2'000.0f;
      auto const alpha = static_cast<int>(std::lerp(SDL_ALPHA_OPAQUE, SDL_ALPHA_TRANSPARENT, progress));
      SDL_SetTextureAlphaMod(logo_, alpha);
    }

    int screen_w, screen_h;
    SDL_GetCurrentRenderOutputSize(renderer, &screen_w, &screen_h);
    float logo_w, logo_h;
    SDL_GetTextureSize(logo_, &logo_w, &logo_h);

    float const logo_aspect = logo_w/logo_h;
    float const screen_aspect = static_cast<float>(screen_w-20)/static_cast<float>(screen_h-20);

    float put_w, put_h;
    if (logo_aspect>screen_aspect) {
      put_w = screen_w-20.f;
      put_h = put_w/logo_aspect;
    }
    else {
      put_h = screen_h-20.f;
      put_w = put_h*logo_aspect;
    }

    SDL_FRect const logo_rect = {
        .x = (screen_w-put_w)/2.f,
        .y = (screen_h-put_h)/2.f,
        .w = put_w,
        .h = put_h
    };
    SDL_RenderTexture(renderer, logo_, nullptr, &logo_rect);
  }
}

void SplashState::on_enter(GameStateManager& gsm)
{
  auto& am = AssetManager::instance();
  time_in_state_ = 0ms;
  logo_ = am.get_texture_asset("logo.jpg");
  SDL_SetTextureBlendMode(logo_, SDL_BLENDMODE_BLEND);

  SDL_SetWindowIcon(gsm.window(), am.get_surface_asset("snake-icon.png"));
}