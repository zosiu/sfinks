#include <chrono>
#include <iostream>

#include <bird_lady/game.hpp>
#include <sfinks/engine.hpp>

using namespace bird_lady;

auto main() -> int {
  bird_lady::Game game_4p(4);
  sfinks::Engine engine(&game_4p, "bird_lady_4p");

  constexpr int num_of_learn_games = 10'000;
  auto before_learn = std::chrono::high_resolution_clock::now();
  engine.learn(num_of_learn_games);
  auto after_learn = std::chrono::high_resolution_clock::now();
  auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(after_learn - before_learn).count();
  std::cout << "learning " << num_of_learn_games << " games took " << duration << " milliseconds" << std::endl;

  constexpr int num_of_play_games = 100;
  engine.play(num_of_play_games);
}
