#pragma once

#include <deque>
#include <vector>

#include <bird_lady/board.hpp>
#include <bird_lady/card_handle.hpp>

namespace bird_lady {

class Deck {
public:
  Deck();

  [[nodiscard]] auto size() const -> size_t;

  [[nodiscard]] auto draw_for_board() -> BoardSlice;
  void put_on_top(CardHandle card);

  void reset();

private:
  [[nodiscard]] auto draw_top() -> CardHandle;
  void shuffle();

  std::deque<CardHandle> _cards = {};
};

} // namespace bird_lady
