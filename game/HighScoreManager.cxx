#include "HighScoreManager.hxx"

#include <SDL.h>

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <numeric>

namespace fs = std::filesystem;

HighScoreManager::HighScoreManager()
{
  data_dir_ = SDL_GetPrefPath("draconic-bytes", "snake");
  fs::create_directories(data_dir_);

  load();
}

void HighScoreManager::load()
{
  auto const filename = fs::path{data_dir_}/"highscore.dat";
  std::ifstream input{filename, std::ios::binary};
  if (!input) {
    SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "Missing high score file: %s", filename.c_str());
    return;
  }

  std::size_t count;
  input.read(std::bit_cast<char*>(&count), sizeof(std::size_t));
  high_score_.scores_.resize(count);
  for (std::size_t n = 0; n<count; ++n) {
    std::size_t len;
    input.read(std::bit_cast<char*>(&len), sizeof(std::size_t));
    high_score_.scores_[n].player_name_.resize(len);
    input.read(high_score_.scores_[n].player_name_.data(), static_cast<std::streamsize>(len));
    input.read(std::bit_cast<char*>(&(high_score_.scores_[n].points_)), sizeof(unsigned));
  }
  std::ranges::stable_sort(high_score_.scores_, std::greater<>{}, &Score::points_);

  if (high_score_.scores_.empty()) {
    SDL_Log("No existing high scores.");
  }
  else {
    SDL_Log("Loaded high scores:");
    for (auto const& score: high_score_.scores_) {
      SDL_Log(" - %s: %u", score.player_name_.c_str(), score.points_);
    }
  }
}

void HighScoreManager::save() const
{
  auto const filename = fs::path{data_dir_}/"highscore.dat";
  std::ofstream output{filename, std::ios::binary};
  if (!output) {
    SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Failed saving high scores to file %s", filename.c_str());
    return;
  }

  auto const count = high_score_.scores_.size();
  output.write(std::bit_cast<char const*>(&count), sizeof(std::size_t));
  for (auto const& score: high_score_.scores_) {
    auto const len = score.player_name_.length();
    output.write(std::bit_cast<char const*>(&len), sizeof(std::size_t));
    output.write(score.player_name_.data(), static_cast<std::streamsize>(score.player_name_.length()));
    output.write(std::bit_cast<char const*>(&score.points_), sizeof(unsigned));
  }
  if (high_score_.scores_.empty()) {
    SDL_Log("No high scores saved.");
  }
  else {
    SDL_Log("Saved high scores:");
    for (auto const& score: high_score_.scores_) {
      SDL_Log(" - %s: %u", score.player_name_.c_str(), score.points_);
    }
  }
}

void HighScoreManager::set_new_score(unsigned int score)
{
  if (high_score_.scores_.size()<MAX_SCORES || high_score_.scores_.back().points_<score) {
    new_score_ = score;
  }
  else {
    new_score_.reset();
  }
}

bool HighScoreManager::has_new_score() const
{
  return new_score_.has_value();
}

unsigned HighScoreManager::get_new_score() const
{
  return new_score_.value_or(0u);
}

void HighScoreManager::provide_name_for_new_score(std::string const& name)
{
  if (new_score_.has_value()) {
    high_score_.scores_.push_back({name, new_score_.value()});
    new_score_.reset();
    std::ranges::stable_sort(high_score_.scores_, std::greater<>{}, &Score::points_);
    if (high_score_.scores_.size()>MAX_SCORES) {
      high_score_.scores_.resize(MAX_SCORES);
    }
    save();
  }
}

HighScoreManager& HighScoreManager::instance()
{
  static HighScoreManager manager;
  return manager;
}

std::vector<Score> const& HighScoreManager::get_scores() const
{
  return high_score_.scores_;
}
