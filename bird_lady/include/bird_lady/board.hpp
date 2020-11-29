#pragma once

#include <array>

#include <bird_lady/card_handle.hpp>

namespace bird_lady {

struct BoardSlice {
  CardHandle first;
  CardHandle second;
};

class Board {
public:
  static constexpr int size = 2;
  static constexpr auto slice_ids = std::array<size_t, size * size>{0, 1, 2, 3};

  [[nodiscard]] auto slice(size_t slice_index) const -> BoardSlice;
  auto replace(size_t slice_index, BoardSlice cards) -> BoardSlice;
  auto take(size_t slice_index) -> BoardSlice;

  void reset();

private:
  void write_slice(size_t slice_index, BoardSlice slice);
  void check_slice_index(size_t slice_index) const;

  CardHandle _card_0 = CardHandle::none;
  CardHandle _card_1 = CardHandle::none;
  CardHandle _card_2 = CardHandle::none;
  CardHandle _card_3 = CardHandle::none;
};

} // namespace bird_lady
