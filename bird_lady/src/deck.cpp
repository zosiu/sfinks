#include <algorithm>
#include <random>
#include <stdexcept>

#include <bird_lady/consts.hpp>

#include <bird_lady/deck.hpp>

namespace bird_lady {

Deck::Deck() { reset(); }

auto Deck::size() const -> size_t { return _cards.size(); }

auto Deck::draw_for_board() -> BoardSlice {
  if (_cards.size() < Board::size)
    std::throw_with_nested(std::invalid_argument("not enough cards to draw from"));
  return {draw_top(), draw_top()};
}

void Deck::put_on_top(CardHandle card) { _cards.push_front(card); }

void Deck::reset() {
  _cards.clear();
  for (const auto &[card, count] : consts::card_counts)
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
