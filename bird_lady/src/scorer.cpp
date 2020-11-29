#include <bird_lady/birds_scorer.hpp>
#include <bird_lady/consts.hpp>
#include <bird_lady/player.hpp>

#include <bird_lady/scorer.hpp>

namespace bird_lady {

auto Scorer::score(const Player& player) -> PlayerScoreDetails {
  auto [birds_score, happy_birds, leftover_food] = score_from_birds(player);
  return {birds_score, score_from_aviaries(player, happy_birds), score_from_eggs(player)};
}

auto Scorer::score_from_birds(const Player& player) -> BirdsScoreDetails {
  return BirdsScorer(player).score();
}

auto Scorer::score_from_aviaries(const Player& player, int number_of_happy_birds) -> int {
  switch (player.number_of_available_cards(CardHandle::aviary)) {
  case 0:
    return consts::no_aviary_penalty;
  case 1:
    return number_of_happy_birds;
  default:
    return number_of_happy_birds * 2;
  }
}

auto Scorer::score_from_eggs(const Player& player) -> int {
  return (player.number_of_available_cards(CardHandle::egg) / 2) * consts::egg_reward_per_pair;
}

} // namespace bird_lady
