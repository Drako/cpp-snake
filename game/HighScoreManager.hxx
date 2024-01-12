#pragma once

#ifndef SNAKE_HIGHSCOREMANAGER_HXX
#define SNAKE_HIGHSCOREMANAGER_HXX

#include <boost/noncopyable.hpp>

#include <boost/serialization/string.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/version.hpp>

#include <optional>
#include <string>
#include <vector>

struct Score final {
  std::string player_name_;
  unsigned points_;

  template<typename Archive>
  void serialize(Archive& archive, unsigned const version)
  {
    (void) version;
    archive & player_name_;
    archive & points_;
  }
};
BOOST_CLASS_VERSION(Score, 0);

struct HighScores final {
  std::vector<Score> scores_;

  template<typename Archive>
  void serialize(Archive& archive, unsigned const version)
  {
    (void) version;
    archive & scores_;
  }
};
BOOST_CLASS_VERSION(HighScores, 0);

class HighScoreManager final : private boost::noncopyable {
public:
  static int constexpr MAX_SCORES = 10;

  void set_new_score(unsigned score);

  [[nodiscard]] bool has_new_score() const;

  [[nodiscard]] unsigned get_new_score() const;

  void provide_name_for_new_score(std::string const& name);

  static HighScoreManager& instance();

  [[nodiscard]] std::vector<Score> const& get_scores() const;

private:
  HighScoreManager();

  void load();

  void save() const;

  std::string data_dir_;

  HighScores high_score_;
  std::optional<unsigned> new_score_;
};

#endif // SNAKE_HIGHSCOREMANAGER_HXX
