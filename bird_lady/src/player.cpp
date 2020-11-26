#include <stdexcept>

#include <bird_lady/player.hpp>

namespace bird_lady {

auto Player::number_of_available_cards(CardHandle handle) const -> int {
  const auto it = _cards.find(handle);
  return it == _cards.end() ? 0 : it->second.first;
}

void Player::acquire_card(CardHandle handle) {
  _cards.try_emplace(handle, std::make_pair(0, 0));
  _cards.at(handle).first += 1;
}

void Player::lose_card(CardHandle handle) {
  auto &[available, spent] = _cards.at(handle);
  if (available == 0)
    std::throw_with_nested(std::invalid_argument("not enough available cards"));

  available -= 1;
}

void Player::spend_card(CardHandle handle, int quantity) {
  auto &[available, spent] = _cards.at(handle);
  if (quantity > available)
    std::throw_with_nested(std::invalid_argument("not enough available cards"));
  available -= quantity;
  spent += quantity;
}

void Player::unspend_card(CardHandle handle, int quantity) {
  auto &[available, spent] = _cards.at(handle);
  if (quantity > spent)
    std::throw_with_nested(std::invalid_argument("not enough spent cards"));
  available += quantity;
  spent -= quantity;
}

void Player::reset() { _cards.clear(); }

} // namespace bird_lady
