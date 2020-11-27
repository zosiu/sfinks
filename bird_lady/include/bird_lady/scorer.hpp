#pragma once

namespace bird_lady {

class Player;
struct BirdsScoreDetails;

struct PlayerScoreDetails {
  int score_from_birds = 0;
  int score_from_toys = 0;
  int score_from_aviaries = 0;
  int score_from_eggs = 0;
};

class Scorer {
public:
  Scorer(const Player *player, int most_toys);
  [[nodiscard]] auto score() const -> PlayerScoreDetails;

private:
  [[nodiscard]] auto score_from_birds() const -> BirdsScoreDetails;
  [[nodiscard]] auto score_from_toys() const -> int;
  [[nodiscard]] auto score_from_aviaries(int number_of_happy_birds) const -> int;
  [[nodiscard]] auto score_from_eggs() const -> int;

  const Player *_player;
  int _most_toys;
};

} // namespace bird_lady
