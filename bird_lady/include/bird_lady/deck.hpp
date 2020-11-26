#pragma once

#include <deque>
#include <unordered_map>
#include <vector>

#include <bird_lady/card_handle.hpp>

namespace bird_lady {

class Deck {
public:
  Deck(int number_of_players);

  [[nodiscard]] auto size() const -> size_t;
  [[nodiscard]] auto max_card_count(CardHandle card) const -> size_t;

  [[nodiscard]] auto draw(size_t n) -> std::vector<CardHandle>;
  void put_on_top(const std::vector<CardHandle> &cards);

  [[nodiscard]] auto initial_contents() const -> const std::unordered_map<CardHandle, size_t> &;

  void reset();

private:
  [[nodiscard]] auto draw_top() -> CardHandle;
  void shuffle();

  std::unordered_map<CardHandle, size_t> _card_counts;
  std::deque<CardHandle> _cards = {};
};

} // namespace bird_lady
