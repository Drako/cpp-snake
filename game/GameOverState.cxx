#include "GameOverState.hxx"
#include "GameStateManager.hxx"
#include "TranslationManager.hxx"
#include "HighScoreManager.hxx"
#include "../SDLRenderer.hxx"

#include <regex>

namespace {
  std::regex const PLACEHOLDER_REGEX{R"(\{\})"};
}

GameOverState::GameOverState()
    :font_{"kenney_pixel.ttf"}
{
}

void GameOverState::on_enter(GameStateManager& gsm)
{
  SDL_ShowCursor(SDL_ENABLE);

  ok_button_.set_title(TranslationManager::instance().get_translation("OK"));
  if (HighScoreManager::instance().has_new_score()) {
    name_input_.set_focus(true);
    name_input_.set_value("Anon");
    name_input_.set_visible(true);
  }
  else {
    name_input_.set_focus(false);
    name_input_.set_value("");
    name_input_.set_visible(false);
  }

  ok_button_.set_on_click([&gsm, this] {
    HighScoreManager::instance().provide_name_for_new_score(name_input_.value());
    gsm.replace_state(GameStates::MainMenu);
  });
}

void GameOverState::on_leave()
{
  SDL_StopTextInput();
  SDL_ShowCursor(SDL_DISABLE);
}

void GameOverState::on_event(GameStateManager& gsm, SDL_Event const& evt)
{
  name_input_.on_event(evt);

  if (evt.type==SDL_KEYUP) {
    switch (evt.key.keysym.scancode) {
    default:
      break;
    case SDL_SCANCODE_RETURN:
      HighScoreManager::instance().provide_name_for_new_score(name_input_.value());
      [[fallthrough]];
    case SDL_SCANCODE_ESCAPE:
      gsm.replace_state(GameStates::MainMenu);
      break;
    }
  } else if (evt.type==SDL_CONTROLLERBUTTONUP) {
    switch (evt.cbutton.which) {
    default:
      break;
    // TODO: for full controller support, the input must provide an onscreen keyboard
    // in that case the controller buttons etc. used to interact with this state will need adjustment
    case SDL_CONTROLLER_BUTTON_A:
      HighScoreManager::instance().provide_name_for_new_score(name_input_.value());
      [[fallthrough]];
    case SDL_CONTROLLER_BUTTON_B:
      [[fallthrough]];
    case SDL_CONTROLLER_BUTTON_START:
      gsm.replace_state(GameStates::MainMenu);
      break;
    }
  }
}

void GameOverState::update(GameStateManager& gsm, std::chrono::milliseconds const delta_time)
{
  ok_button_.update();
  name_input_.update(delta_time);
}

void GameOverState::render(SDLRenderer& renderer)
{
  auto const& tm = TranslationManager::instance();

  int width, height;
  SDL_GetRendererOutputSize(renderer, &width, &height);

  int base_y;
  auto const& hsm = HighScoreManager::instance();
  if (hsm.has_new_score()) {
    auto score_text = (tm.get_translation("Congratulations, you made it to the top 10!")
        +"\n"+tm.get_translation("You reached {} points!")+"\n"+tm.get_translation("Please enter your name:"));
    score_text = std::regex_replace(score_text, PLACEHOLDER_REGEX, std::to_string(hsm.get_new_score()));
    SDL_Surface* text_surface = TTF_RenderUTF8_Solid_Wrapped(font_, score_text.c_str(),
        {255, 255, 255, SDL_ALPHA_OPAQUE}, 0);
    SDL_Texture* text = SDL_CreateTextureFromSurface(renderer, text_surface);
    SDL_FreeSurface(text_surface);
    int text_width, text_height;
    SDL_QueryTexture(text, nullptr, nullptr, &text_width, &text_height);
    // some up-scaling
    text_width *= 2;
    text_height *= 2;

    base_y = (height-(text_height+name_input_.get_bounding_box().h+ok_button_.get_bounding_box().h+30))/2;

    SDL_Rect render_quad = {(width-text_width)/2, base_y, text_width, text_height};
    SDL_RenderCopy(renderer, text, nullptr, &render_quad);
    SDL_DestroyTexture(text);

    name_input_.move((width-name_input_.get_bounding_box().w)/2, base_y += (text_height+20));
    name_input_.render(renderer);
    base_y += name_input_.get_bounding_box().h+10;
  }
  else {
    SDL_Surface* text_surface = TTF_RenderUTF8_Solid(font_, tm.get_translation("Game over!").c_str(),
        {255, 255, 255, SDL_ALPHA_OPAQUE});
    SDL_Texture* text = SDL_CreateTextureFromSurface(renderer, text_surface);
    SDL_FreeSurface(text_surface);
    int text_width, text_height;
    SDL_QueryTexture(text, nullptr, nullptr, &text_width, &text_height);
    // some up-scaling
    text_width *= 2;
    text_height *= 2;

    base_y = (height-(text_height+ok_button_.get_bounding_box().h+20))/2;

    SDL_Rect render_quad = {(width-text_width)/2, base_y, text_width, text_height};
    SDL_RenderCopy(renderer, text, nullptr, &render_quad);
    SDL_DestroyTexture(text);

    base_y += text_height+20;
  }

  ok_button_.move((width-ok_button_.get_bounding_box().w)/2, base_y);
  ok_button_.render(renderer);
}
