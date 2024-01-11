#include "PlayingState.hxx"

#include "GameStateManager.hxx"

#include <algorithm>
#include <cfenv>
#include <cmath>
#include <format>
#include <random>

namespace {
  SDL_Point head_position(SDL_FPoint const& position)
  {
#pragma STDC FENV_ACCESS ON
    std::fesetround(FE_TONEAREST);
    return {
        .x = static_cast<int>(std::nearbyint(position.x)),
        .y = static_cast<int>(std::nearbyint(position.y)),
    };
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

unsigned PlayingState::last_high_score_{0};

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
  if (evt.type==SDL_KEYUP && evt.key.keysym.scancode==SDL_SCANCODE_ESCAPE) {
    gsm.push_state(GameStates::MainMenu);
  }
  else if (evt.type==SDL_KEYDOWN) {
    switch (evt.key.keysym.scancode) {
    default:
      break;
    case SDL_SCANCODE_UP:
      if (direction_==Direction::Left || direction_==Direction::Right) {
        direction_ = Direction::Up;
      }
      break;
    case SDL_SCANCODE_DOWN:
      if (direction_==Direction::Left || direction_==Direction::Right) {
        direction_ = Direction::Down;
      }
      break;
    case SDL_SCANCODE_LEFT:
      if (direction_==Direction::Up || direction_==Direction::Down) {
        direction_ = Direction::Left;
      }
      break;
    case SDL_SCANCODE_RIGHT:
      if (direction_==Direction::Up || direction_==Direction::Down) {
        direction_ = Direction::Right;
      }
      break;
    }
  }
}

void PlayingState::update(GameStateManager& gsm, std::chrono::milliseconds delta_time)
{
  auto const distance = speed_*static_cast<float>(delta_time.count());
  if (distance>MAX_DISTANCE) {
    SDL_Log("Snake would move a distance of %f. Game might have been stuck. Skipping cycle.", distance);
    return;
  }

  SDL_FPoint new_head = head_;
  switch (direction_) {
  case Direction::Up:
    new_head.y -= distance;
    break;
  case Direction::Down:
    new_head.y += distance;
    break;
  case Direction::Left:
    new_head.x -= distance;
    break;
  case Direction::Right:
    new_head.x += distance;
    break;
  }

  auto const old_pos = ::head_position(head_);
  auto const new_pos = ::head_position(new_head);
  if (old_pos!=new_pos) {
    if (new_pos==target_) {
      ++length_;
      speed_ *= ACCELERATION;
      place_target();
    }

    if (detect_death(new_pos)) {
      last_high_score_ = length_;
      gsm.replace_state(GameStates::GameOver);
    }

    tail_.push_front(old_pos);
    if (tail_.size()+1>length_)
      tail_.pop_back();
  }

  head_ = new_head;
}

void PlayingState::render(SDLRenderer& renderer)
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

  SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
  SDL_RenderClear(renderer);

  render_ui(renderer, playing_field);
  render_target(renderer, playing_field);
  render_snake(renderer, playing_field);

  SDL_RenderPresent(renderer);
}

void PlayingState::render_ui(SDLRenderer& renderer, SDL_Rect const& playing_field)
{
  TTF_Font* const font = font_;

  auto const score_text = std::format("Score: {}", length_);
  SDL_Surface* text_surface = TTF_RenderText_Solid(font, score_text.c_str(), {255, 255, 255, SDL_ALPHA_OPAQUE});
  SDL_Texture* text = SDL_CreateTextureFromSurface(renderer, text_surface);
  SDL_FreeSurface(text_surface);
  int text_width, text_height;
  SDL_QueryTexture(text, nullptr, nullptr, &text_width, &text_height);
  SDL_Rect render_quad = {playing_field.x, 10, text_width, text_height};
  SDL_RenderCopy(renderer, text, nullptr, &render_quad);
  SDL_DestroyTexture(text);

  SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
  SDL_RenderDrawRect(renderer, &playing_field);
}

void PlayingState::place_target()
{
  target_.x = distribution_position_x_(generator_);
  target_.y = distribution_position_y_(generator_);
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
  SDL_Rect const target_rect{
      .x = static_cast<int>(playing_field.x+ratio*target_.x),
      .y = static_cast<int>(playing_field.y+ratio*target_.y),
      .w = static_cast<int>(ratio),
      .h = static_cast<int>(ratio),
  };

  SDL_SetRenderDrawColor(renderer, 0, 255, 0, SDL_ALPHA_OPAQUE);
  SDL_RenderFillRect(renderer, &target_rect);
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
  render_dot(::head_position(head_), 1.0);
  double size = 1.0;
  double const decay = 1.0/static_cast<double>(tail_.size()+1);
  for (auto const& particle: tail_) {
    size = std::max(0.0, size-decay);
    render_dot(particle, size);
  }
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

unsigned PlayingState::last_high_score()
{
  return last_high_score_;
}
