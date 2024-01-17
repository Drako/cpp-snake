#include "PlayingState.hxx"

#include "HighScoreManager.hxx"
#include "GameStateManager.hxx"
#include "TranslationManager.hxx"

#include <algorithm>
#include <cfenv>
#include <cmath>
#include <iterator>
#include <random>

namespace {
  SDL_Point head_position(SDL_FPoint const& position)
  {
#ifndef _MSC_VER
#pragma STDC FENV_ACCESS ON
#endif // !_MSC_VER
    std::fesetround(FE_TONEAREST);
    return {
        .x = static_cast<int>(std::nearbyint(position.x)),
        .y = static_cast<int>(std::nearbyint(position.y)),
    };
  }

  unsigned points_for_target(SDL_Point const& target)
  {
    // normally there is just 1 point per target hit
    SDL_Rect const inner_field{
        .x=2,
        .y=2,
        .w=PlayingState::CELLS_X-4,
        .h=PlayingState::CELLS_Y-4,
    };
    if (SDL_PointInRect(&target, &inner_field)) {
      return 1;
    }

    // targets with a distance of 1 to the wall give double points
    SDL_Rect const outer_field{
        .x=1,
        .y=1,
        .w=PlayingState::CELLS_X-2,
        .h=PlayingState::CELLS_Y-2,
    };
    if (SDL_PointInRect(&target, &outer_field)) {
      return 2;
    }

    // corners give 8 points
    if ((target.x==0 || target.x==PlayingState::CELLS_X-1) && (target.y==0 || target.y==PlayingState::CELLS_Y-1)) {
      return 8;
    }

    // edges give 4 points
    return 4;
  }
}

static bool operator==(SDL_Point const& lhs, SDL_Point const& rhs)
{
  return lhs.x==rhs.x && lhs.y==rhs.y;
}

static bool operator!=(SDL_Point const& lhs, SDL_Point const& rhs)
{
  return lhs.x!=rhs.x || lhs.y!=rhs.y;
}

PlayingState::PlayingState()
    :generator_{std::random_device{}()}, font_{"kenney_pixel.ttf"}
{
}

void PlayingState::on_enter(GameStateManager& gsm)
{
  (void) gsm;

  length_ = 10u;

  std::uniform_int_distribution<int> distribution_direction{0, 3};
  direction_ = static_cast<Direction>(distribution_direction(generator_));

  place_head();
  place_target();

  tail_.clear();

  speed_ = START_SPEED;
}

void PlayingState::on_event(GameStateManager& gsm, SDL_Event const& evt)
{
  if (evt.type==SDL_KEYUP) {
    auto const scancode = evt.key.keysym.scancode;
    if (scancode==SDL_SCANCODE_ESCAPE || scancode==SDL_SCANCODE_PAUSE)
      gsm.push_state(GameStates::MainMenu);
  }
}

void PlayingState::update(GameStateManager& gsm, std::chrono::milliseconds const delta_time)
{
  handle_direction_change();
  fps_ = static_cast<int>(1000.0/static_cast<double>(delta_time.count()));

  auto const distance = speed_*static_cast<float>(delta_time.count());
  if (distance>MAX_DISTANCE) {
    SDL_Log("Snake would move a distance of %f. Game might have been stuck. Skipping cycle.", distance);
    return;
  }

  auto const direction = new_direction_.value_or(direction_);

  auto const MAX_X = static_cast<float>(CELLS_X-1);
  auto const MAX_Y = static_cast<float>(CELLS_Y-1);
  SDL_FPoint new_head = head_;
  switch (direction) {
  case Direction::Up:
    new_head.y -= distance;
    if (new_head.y<0.0f && !deadly_wall_) {
      new_head.y += MAX_Y;
    }
    break;
  case Direction::Down:
    new_head.y += distance;
    if (new_head.y>=MAX_Y && !deadly_wall_) {
      new_head.y -= MAX_Y;
    }
    break;
  case Direction::Left:
    new_head.x -= distance;
    if (new_head.x<0.0f && !deadly_wall_) {
      new_head.x += MAX_X;
    }
    break;
  case Direction::Right:
    new_head.x += distance;
    if (new_head.x>=MAX_X && !deadly_wall_) {
      new_head.x -= MAX_X;
    }
    break;
  }

  auto const old_pos = ::head_position(head_);
  auto const new_pos = ::head_position(new_head);
  if (old_pos!=new_pos) {
    if (new_direction_.has_value()) {
      direction_ = new_direction_.value();
      new_direction_.reset();
    }

    if (detect_death(new_pos)) {
      HighScoreManager::instance().set_new_score(length_);
      gsm.replace_state(GameStates::GameOver);
    }

    if (target_.contains(new_pos)) {
      target_.erase(new_pos);
      auto const growth = std::min(points_for_target(new_pos), static_cast<unsigned>(CELLS_X*CELLS_Y)-length_);
      length_ += growth;
      speed_ = std::min(MAX_SPEED, speed_*std::pow(ACCELERATION, static_cast<float>(growth)));
      if (!place_target()) {
        // technically the player finished the game at this point
        HighScoreManager::instance().set_new_score(length_);
        gsm.replace_state(GameStates::GameOver);
      }
    }

    tail_.push_front(old_pos);
    if (tail_.size()+1>length_)
      tail_.pop_back();
  }

  head_ = new_head;
}

void PlayingState::handle_direction_change()
{
  // this is not done in the event handler as we don't want to wait for KEYUP to re-fire in certain situations
  auto const keyboard = SDL_GetKeyboardState(nullptr);

  if (new_direction_.has_value())
    return;

  if (direction_==Direction::Left || direction_==Direction::Right) {
    if (keyboard[SDL_SCANCODE_UP] || keyboard[SDL_SCANCODE_W]) {
      new_direction_ = Direction::Up;
    }

    else if (keyboard[SDL_SCANCODE_DOWN] || keyboard[SDL_SCANCODE_S]) {
      new_direction_ = Direction::Down;
    }
  }
  else {
    if (keyboard[SDL_SCANCODE_LEFT] || keyboard[SDL_SCANCODE_A]) {
      new_direction_ = Direction::Left;
    }

    else if (keyboard[SDL_SCANCODE_RIGHT] || keyboard[SDL_SCANCODE_D]) {
      new_direction_ = Direction::Right;
    }
  }
}

void PlayingState::render(SDLRenderer& renderer)
{
  render_game(renderer);
}

void PlayingState::render_ui(SDLRenderer& renderer, SDL_Rect const& playing_field)
{
  auto const& tm = TranslationManager::instance();

  auto const score_text = tm.get_translation("Score")+": "+std::to_string(length_);
  SDL_Surface* text_surface = TTF_RenderUTF8_Solid(font_, score_text.c_str(), {255, 255, 255, SDL_ALPHA_OPAQUE});
  SDL_Texture* text = SDL_CreateTextureFromSurface(renderer, text_surface);
  SDL_FreeSurface(text_surface);
  int text_width, text_height;
  SDL_QueryTexture(text, nullptr, nullptr, &text_width, &text_height);
  SDL_Rect render_quad = {playing_field.x, 10, text_width, text_height};
  SDL_RenderCopy(renderer, text, nullptr, &render_quad);
  SDL_DestroyTexture(text);

  auto const fps_text = tm.get_translation("Frames per second")+": "+std::to_string(fps_);
  text_surface = TTF_RenderUTF8_Solid(font_, fps_text.c_str(), {255, 255, 255, SDL_ALPHA_OPAQUE});
  text = SDL_CreateTextureFromSurface(renderer, text_surface);
  SDL_FreeSurface(text_surface);
  SDL_QueryTexture(text, nullptr, nullptr, &text_width, &text_height);
  render_quad = {playing_field.x+playing_field.w-text_width, 10, text_width, text_height};
  SDL_RenderCopy(renderer, text, nullptr, &render_quad);
  SDL_DestroyTexture(text);

  if (deadly_wall_)
    SDL_SetRenderDrawColor(renderer, 249, 95, 0, SDL_ALPHA_OPAQUE);
  else
    SDL_SetRenderDrawColor(renderer, 255, 204, 0, SDL_ALPHA_OPAQUE);
  SDL_RenderDrawRect(renderer, &playing_field);
}

bool PlayingState::place_target()
{
  std::unordered_set<SDL_Point> field;
  field.reserve(CELLS_X*CELLS_Y);
  for (int x = 0; x<CELLS_X; ++x) {
    for (int y = 0; y<CELLS_Y; ++y) {
      field.insert({x, y});
    }
  }

  field.erase(::head_position(head_));
  for (auto const& particle: tail_) {
    field.erase(particle);
  }
  for (auto const& target: target_) {
    field.erase(target);
  }

  if (field.empty()) {
    return false;
  }

  auto const wanted_targets = distribution_num_targets(generator_);
  std::ranges::sample(field, std::insert_iterator{target_, target_.end()},
      std::max(static_cast<long long>(wanted_targets-target_.size()), 0ll), generator_);
  deadly_wall_ = distribution_deadly_wall(generator_)!=0;

  return true;
}

void PlayingState::place_head()
{
  do {
    head_.x = static_cast<float>(distribution_position_x_(generator_));
  }
  while (head_.x<10.0f || head_.x>static_cast<float>(CELLS_X-10));

  do {
    head_.y = static_cast<float>(distribution_position_y_(generator_));
  }
  while (head_.y<10.0f || head_.y>static_cast<float>(CELLS_Y-10));
}

void PlayingState::render_target(SDLRenderer& renderer, SDL_Rect const& playing_field)
{
  auto const ratio = playing_field.w/static_cast<double>(CELLS_X);

  SDL_SetRenderDrawColor(renderer, 76, 208, 45, SDL_ALPHA_OPAQUE);
  for (auto const& target: target_) {
    SDL_Rect const target_rect{
        .x = static_cast<int>(playing_field.x+ratio*target.x),
        .y = static_cast<int>(playing_field.y+ratio*target.y),
        .w = static_cast<int>(ratio),
        .h = static_cast<int>(ratio),
    };

    SDL_RenderFillRect(renderer, &target_rect);
  }
}

void PlayingState::render_snake(SDLRenderer& renderer, SDL_Rect const& playing_field)
{
  auto const ratio = playing_field.w/static_cast<double>(CELLS_X);
  auto const render_dot = [ratio, playing_field, &renderer](SDL_Point const& position, double const size_factor) {
    int const base_x = static_cast<int>(playing_field.x+ratio*position.x);
    int const base_y = static_cast<int>(playing_field.y+ratio*position.y);
    int const size = std::max(1, static_cast<int>(ratio*size_factor));
    int const padding = (static_cast<int>(ratio)-size) >> 1;
    SDL_Rect const target_rect{
        .x = base_x+padding,
        .y = base_y+padding,
        .w = size,
        .h = size,
    };

    SDL_RenderFillRect(renderer, &target_rect);
  };

  SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
  double size = 1.0;
  double const decay = 1.0/static_cast<double>(tail_.size()+1);
  for (auto const& particle: tail_) {
    size = std::max(0.0, size-decay);
    render_dot(particle, size);
  }
  SDL_SetRenderDrawColor(renderer, 0, 170, 231, SDL_ALPHA_OPAQUE);
  render_dot(::head_position(head_), 1.0);
}

bool PlayingState::detect_death(SDL_Point const& position)
{
  // collision with wall
  if (position.x<0 || position.x>=CELLS_X || position.y<0 || position.y>=CELLS_Y)
    return true;

  // collision with self
  return (std::ranges::any_of(tail_, [&position](SDL_Point const& particle) {
    return position==particle;
  }));
}

void PlayingState::render_game(SDLRenderer& renderer, bool is_current_state)
{
  int width, height;
  SDL_GetRendererOutputSize(renderer, &width, &height);

  SDL_Rect playing_field;
  double const ratio = static_cast<double>(CELLS_X)/CELLS_Y;

  if (width<height*ratio) {
    playing_field.w = width-20;
    playing_field.h = static_cast<int>(playing_field.w/ratio);
  }
  else {
    playing_field.h = height-70;
    playing_field.w = static_cast<int>(playing_field.h*ratio);
  }

  playing_field.x = (width-playing_field.w)/2;
  playing_field.y = 50;

  if (is_current_state) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);
  }

  render_snake(renderer, playing_field);
  render_target(renderer, playing_field);
  render_ui(renderer, playing_field);

  if (is_current_state)
    SDL_RenderPresent(renderer);
}
