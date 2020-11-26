#pragma once

#include <unordered_map>

#include <bird_lady/consts.hpp>

namespace bird_lady {

class Player {
public:
  [[nodiscard]] auto number_of_available_cards(CardHandle handle) const -> int;

  void acquire_card(CardHandle handle);
  void lose_card(CardHandle handle);

  void spend_card(CardHandle handle, int quantity);
  void unspend_card(CardHandle handle, int quantity);

  void reset();

private:
  std::unordered_map<CardHandle, std::pair<int, int>> _cards = {};
};

} // namespace bird_lady
