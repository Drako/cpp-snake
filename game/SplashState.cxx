#include "SplashState.hxx"
#include "AssetManager.hxx"
#include "GameStateManager.hxx"

using namespace std::chrono_literals;

void SplashState::on_event(GameStateManager& gsm, SDL_Event const& evt)
{
  switch (evt.type) {
  default:
    break;
  case SDL_KEYUP:
    switch (evt.key.keysym.scancode) {
    default:
      break;
    case SDL_SCANCODE_SPACE:
      [[fallthrough]];
    case SDL_SCANCODE_RETURN:
      [[fallthrough]];
    case SDL_SCANCODE_ESCAPE:
      gsm.replace_state(GameStates::MainMenu);
      break;
    }
    break;
  case SDL_CONTROLLERBUTTONUP:
    switch (evt.cbutton.button) {
    default:
      break;
    case SDL_CONTROLLER_BUTTON_A:
      [[fallthrough]];
    case SDL_CONTROLLER_BUTTON_B:
      [[fallthrough]];
    case SDL_CONTROLLER_BUTTON_START:
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
    SDL_GetRendererOutputSize(renderer, &screen_w, &screen_h);
    int logo_w, logo_h;
    SDL_QueryTexture(logo_, nullptr, nullptr, &logo_w, &logo_h);

    float const logo_aspect = static_cast<float>(logo_w)/static_cast<float>(logo_h);
    float const screen_aspect = static_cast<float>(screen_w-20)/static_cast<float>(screen_h-20);

    int put_w, put_h;
    if (logo_aspect>screen_aspect) {
      put_w = (screen_w-20);
      put_h = static_cast<int>(static_cast<float>(put_w)/logo_aspect);
    }
    else {
      put_h = screen_h-20;
      put_w = static_cast<int>(static_cast<float>(put_h)*logo_aspect);
    }

    SDL_Rect const logo_rect = {
        .x = (screen_w-put_w)/2,
        .y = (screen_h-put_h)/2,
        .w = put_w,
        .h = put_h
    };
    SDL_RenderCopy(renderer, logo_, nullptr, &logo_rect);
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