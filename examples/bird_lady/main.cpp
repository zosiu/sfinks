#include <iostream>

#include <bird_lady/game.hpp>
#include <sfinks/engine.hpp>

using namespace bird_lady;

auto main() -> int {
  // Game game_2p(2);
  // sfinks::Engine engine2p(&game_2p, "bird_lady_2p");
  Game game_3p(3);
  sfinks::Engine engine3p(&game_3p, "bird_lady_3p");
  // Game game_4p(4);
  // sfinks::Engine engine4p(&game_4p, "bird_lady_4p");

  constexpr int num_of_play_games = 1;
  // engine2p.play(num_of_play_games);
  engine3p.play(num_of_play_games);
  // engine4p.play(num_of_play_games);
}
