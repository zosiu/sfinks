#include <stdexcept>

#include <bird_lady/consts.hpp>

#include <bird_lady/board.hpp>

namespace bird_lady {

Board::Board() : _cards(CardHandle::none, consts::board_size * consts::board_size) {}

auto Board::slice(size_t slice_index) const -> std::vector<CardHandle> {
  check_slice_index(slice_index);
  const std::valarray<CardHandle> slice = _cards[slice_for(slice_index)];
  return std::vector<CardHandle>(std::begin(slice), std::end(slice));
}

auto Board::replace(size_t slice_index, const std::vector<CardHandle> &cards) -> std::vector<CardHandle> {
  check_slice_index(slice_index);

  const std::valarray<CardHandle> old_slice = _cards[slice_for(slice_index)];
  _cards[slice_for(slice_index)] = std::valarray(cards.data(), cards.size());
  return std::vector<CardHandle>(std::begin(old_slice), std::end(old_slice));
}

auto Board::take(size_t slice_index) -> std::vector<CardHandle> {
  check_slice_availability(slice_index);
  return replace(slice_index, std::vector<CardHandle>(consts::board_size, CardHandle::none));
}

auto Board::available_slices() const -> std::vector<size_t> {
  std::vector<size_t> slice_indices;

  for (size_t i = 0; i < consts::board_size * 2; i++)
    if (is_slice_available(i))
      slice_indices.emplace_back(i);

  return slice_indices;
}

auto Board::is_slice_available(size_t slice_index) const -> bool {
  check_slice_index(slice_index);

  std::valarray<CardHandle> sl = _cards[slice_for(slice_index)];
  return std::none_of(std::begin(sl), std::end(sl), [](CardHandle card) { return card == CardHandle::none; });
}

void Board::reset() { _cards[true] = CardHandle::none; }

auto Board::slice_for(size_t slice_index) const -> std::slice {
  if (slice_index >= consts::board_size)
    return std::slice((slice_index % consts::board_size) * consts::board_size, consts::board_size, 1);
  else
    return std::slice(slice_index, consts::board_size, consts::board_size);
}

void Board::check_slice_index(size_t slice_index) const {
  if (slice_index >= 2 * consts::board_size)
    std::throw_with_nested(std::invalid_argument("no such slice"));
}

void Board::check_slice_availability(size_t slice_index) const {
  if (!is_slice_available(slice_index))
    std::throw_with_nested(std::invalid_argument("no such slice"));
}

} // namespace bird_lady
