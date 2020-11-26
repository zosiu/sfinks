#include <algorithm>
#include <random>
#include <stdexcept>

#include <bird_lady/consts.hpp>

#include <bird_lady/deck.hpp>

namespace bird_lady {

Deck::Deck(int number_of_players) : _card_counts(card_counts_by_player_number.at(number_of_players)) { reset(); }

auto Deck::size() const -> size_t { return _cards.size(); }

auto Deck::max_card_count(CardHandle card) const -> size_t {
  const auto it = _card_counts.find(card);
  return it == _card_counts.end() ? 0 : it->second;
}

auto Deck::draw(size_t n) -> std::vector<CardHandle> {
  if (n > _cards.size())
    std::throw_with_nested(std::invalid_argument("not enough cards to draw from"));
  std::vector<CardHandle> drawn_cards;

  for (size_t i = 0; i < n; i++)
    drawn_cards.emplace_back(draw_top());

  return drawn_cards;
}

void Deck::put_on_top(const std::vector<CardHandle> &cards) {
  for (auto it = cards.rbegin(); it != cards.rend(); ++it)
    _cards.push_front(*it);
}

void Deck::reset() {
  _cards.clear();
  for (const auto &[card, count] : _card_counts)
    _cards.insert(_cards.end(), count, card);
  shuffle();
}

auto Deck::draw_top() -> CardHandle {
  const CardHandle top_card = _cards.front();
  _cards.pop_front();
  return top_card;
}

void Deck::shuffle() {
  std::random_device rng_device;
  std::mt19937 rng{rng_device()};
  std::shuffle(_cards.begin(), _cards.end(), rng);
}

} // namespace bird_lady
