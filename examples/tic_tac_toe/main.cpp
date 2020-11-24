#include <chrono>
#include <iostream>

#include <sfinks/engine.hpp>
#include <tic_tac_toe/game.hpp>

auto main() -> int {
  tic_tac_toe::Game ttt;
  sfinks::Engine engine(&ttt, "tic_tac_toe", sfinks::DataFormat::binary);

  constexpr int num_of_learn_games = 1000;
  auto before_learn = std::chrono::high_resolution_clock::now();
  engine.learn(num_of_learn_games);
  auto after_learn = std::chrono::high_resolution_clock::now();
  auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(after_learn - before_learn).count();
  std::cout << "learning " << num_of_learn_games << " games took " << duration << " milliseconds" << std::endl;

  constexpr int num_of_play_games = 10;
  engine.play(num_of_play_games);
}
