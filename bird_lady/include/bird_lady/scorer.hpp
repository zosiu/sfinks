#pragma once

namespace bird_lady {

class Player;
struct BirdsScoreDetails;

struct PlayerScoreDetails {
  int score_from_birds = 0;
  int score_from_aviaries = 0;
  int score_from_eggs = 0;
};

class Scorer {
public:
  [[nodiscard]] static auto score(const Player& player) -> PlayerScoreDetails;

private:
  [[nodiscard]] inline static auto score_from_birds(const Player& player) -> BirdsScoreDetails;
  [[nodiscard]] inline static auto score_from_aviaries(const Player& player, int number_of_happy_birds) -> int;
  [[nodiscard]] inline static auto score_from_eggs(const Player& player) -> int;
};

} // namespace bird_lady
