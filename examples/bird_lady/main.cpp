#include <iostream>

#include <bird_lady/game.hpp>

using namespace bird_lady;

auto main() -> int {
  Game game(4);
  std::cout << game.resource_count(CardHandle::egg) << std::endl;
}
