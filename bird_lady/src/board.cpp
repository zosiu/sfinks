#include <stdexcept>

#include <bird_lady/board.hpp>

namespace bird_lady {

[[nodiscard]] auto Board::slice(size_t slice_index) const -> BoardSlice {
  switch (slice_index) {
  case 0:
    return {_card_0, _card_1};
  case 1:
    return {_card_2, _card_3};
  case 2:
    return {_card_0, _card_2};
  case 3:
    return {_card_1, _card_3};
  default:
    std::throw_with_nested(std::invalid_argument("no such slice"));
  }
}

auto Board::replace(size_t slice_index, BoardSlice cards) -> BoardSlice {
  BoardSlice old_slice = slice(slice_index);
  write_slice(slice_index, cards);
  return old_slice;
}

auto Board::take(size_t slice_index) -> BoardSlice {
  return replace(slice_index, {CardHandle::none, CardHandle::none});
}

void Board::reset() {
  _card_0 = CardHandle::none;
  _card_1 = CardHandle::none;
  _card_2 = CardHandle::none;
  _card_3 = CardHandle::none;
}

void Board::write_slice(size_t slice_index, BoardSlice slice) {
  const auto [first_card, second_card] = slice;
  switch (slice_index) {
  case 0:
    _card_0 = first_card;
    _card_1 = second_card;
    break;
  case 1:
    _card_2 = first_card;
    _card_3 = second_card;
    break;
  case 2:
    _card_0 = first_card;
    _card_2 = second_card;
    break;
  case 3:
    _card_1 = first_card;
    _card_3 = second_card;
    break;
  default:
    std::throw_with_nested(std::invalid_argument("no such slice"));
  }
}

} // namespace bird_lady
