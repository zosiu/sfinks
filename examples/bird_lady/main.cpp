#include <iostream>

#include <bird_lady/game.hpp>

using namespace bird_lady;

auto main() -> int {
  Game game(4);
  for (auto player_id : game.player_ids())
    std::cout << player_id << std::endl;
}
