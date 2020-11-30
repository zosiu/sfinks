#include <chrono>
#include <iostream>
#include <string>

#include <bird_lady/game.hpp>
#include <sfinks/engine.hpp>

auto main() -> int {
  constexpr size_t num_of_players = 4;
  bird_lady::Game game(num_of_players);
  sfinks::Engine engine(&game, "bird_lady_" + std::to_string(num_of_players) + "p_binary_out",
                        sfinks::DataFormat::binary);

  constexpr int num_of_learn_games = 100'000;
  auto before_learn = std::chrono::high_resolution_clock::now();
  engine.learn(num_of_learn_games);
  auto after_learn = std::chrono::high_resolution_clock::now();
  auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(after_learn - before_learn).count();
  std::cout << "learning " << num_of_learn_games << " games took " << duration << " milliseconds" << std::endl;

  constexpr int num_of_play_games = 1000;
  engine.play(num_of_play_games);
}
