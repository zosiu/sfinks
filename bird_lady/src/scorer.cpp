#include <bird_lady/birds_scorer.hpp>
#include <bird_lady/consts.hpp>
#include <bird_lady/player.hpp>

#include <bird_lady/scorer.hpp>

namespace bird_lady {

Scorer::Scorer(const Player *player, int most_toys) : _player(player), _most_toys(most_toys) {}

auto Scorer::score() const -> PlayerScoreDetails {
  auto [birds_score, happy_birds, leftover_food] = score_from_birds();
  return {birds_score, score_from_toys(), score_from_aviaries(happy_birds), score_from_eggs()};
}

auto Scorer::score_from_birds() const -> BirdsScoreDetails {
  BirdsScorer birds_scorer(*_player);
  return birds_scorer.score();
}

auto Scorer::score_from_toys() const -> int {
  int number_of_toys = _player->number_of_available_cards(CardHandle::toy);
  if (number_of_toys == 0)
    return consts::no_toys_penalty;
  if (number_of_toys == _most_toys)
    return consts::most_toys_reward;

  return 0;
}

auto Scorer::score_from_aviaries(int number_of_happy_birds) const -> int {
  switch (_player->number_of_available_cards(CardHandle::aviary)) {
  case 0:
    return 0;
  case 1:
    return consts::single_aviary_penalty;
  case 2:
    [[fallthrough]];
  case 3:
    return number_of_happy_birds;
  default:
    return number_of_happy_birds * 2;
  }
}

auto Scorer::score_from_eggs() const -> int {
  return (_player->number_of_available_cards(CardHandle::egg) / 2) * consts::egg_reward_per_pair;
}

} // namespace bird_lady
