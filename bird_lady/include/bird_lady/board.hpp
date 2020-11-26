#pragma once

#include <valarray>
#include <vector>

#include <bird_lady/card_handle.hpp>

namespace bird_lady {

class Board {
public:
  Board();

  [[nodiscard]] auto slice(size_t slice_index) const -> std::vector<CardHandle>;
  auto replace(size_t slice_index, const std::vector<CardHandle> &cards) -> std::vector<CardHandle>;
  auto take(size_t slice_index) -> std::vector<CardHandle>;

  [[nodiscard]] auto available_slices() const -> std::vector<size_t>;
  [[nodiscard]] auto is_slice_available(size_t slice_index) const -> bool;

  void reset();

private:
  [[nodiscard]] auto slice_for(size_t slice_index) const -> std::slice;
  void check_slice_index(size_t slice_index) const;
  void check_slice_availability(size_t slice_index) const;

  std::valarray<CardHandle> _cards;
};

} // namespace bird_lady
