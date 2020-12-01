#include <chrono>
#include <iostream>
#include <string>

#include <colors/game.hpp>
#include <sfinks/engine.hpp>

auto main() -> int {
  constexpr size_t num_of_players = 6;
  colors::Game game(num_of_players);
  sfinks::Engine engine(&game, "colors_" + std::to_string(num_of_players) + "p", sfinks::DataFormat::binary);

  constexpr int num_of_learn_games = 200'000;
  auto before_learn = std::chrono::high_resolution_clock::now();
  engine.learn(num_of_learn_games);
  auto after_learn = std::chrono::high_resolution_clock::now();
  auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(after_learn - before_learn).count();
  std::cout << "learning " << num_of_learn_games << " games took " << duration << " milliseconds" << std::endl;

  constexpr int num_of_play_games = 10'000;
  for (size_t i = 0; i < num_of_players; i++) {
    engine.play_against_random_opponents(i, num_of_play_games);
    engine.play_against_greedy_opponents(i, num_of_play_games);
  }
  engine.play_according_to_policy(num_of_play_games);
}
