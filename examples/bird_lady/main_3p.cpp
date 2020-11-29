#include <chrono>
#include <iostream>

#include <bird_lady/game.hpp>
#include <sfinks/engine.hpp>

using namespace bird_lady;

auto main() -> int {
  bird_lady::Game game(3);
  sfinks::Engine engine(&game, "bird_lady_3p_binary_out", sfinks::DataFormat::binary);

  constexpr int num_of_learn_games = 100'000;
  auto before_learn = std::chrono::high_resolution_clock::now();
  engine.learn(num_of_learn_games);
  auto after_learn = std::chrono::high_resolution_clock::now();
  auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(after_learn - before_learn).count();
  std::cout << "learning " << num_of_learn_games << " games took " << duration << " milliseconds" << std::endl;

  constexpr int num_of_play_games = 1000;
  engine.play(num_of_play_games);
}
