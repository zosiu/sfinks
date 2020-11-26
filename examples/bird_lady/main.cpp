#include <iostream>

#include <bird_lady/player.hpp>

using namespace bird_lady;

auto main() -> int {
  Player p;
  p.acquire_card(CardHandle::egg);
  p.acquire_card(CardHandle::egg);
  p.lose_card(CardHandle::egg);
  p.spend_card(CardHandle::egg, 1);
  std::cout << p.number_of_available_cards(CardHandle::egg) << std::endl;
}
