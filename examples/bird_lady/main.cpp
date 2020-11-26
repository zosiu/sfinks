#include <iostream>

#include <bird_lady/game.hpp>
#include <sfinks/engine.hpp>

using namespace bird_lady;

auto main() -> int {
  Game game_2p(2);
  sfinks::Engine engine(&game_2p, "bird_lady_2p");

  constexpr int num_of_play_games = 1;
  engine.play(num_of_play_games);
}
