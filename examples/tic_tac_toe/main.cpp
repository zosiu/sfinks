#include <chrono>
#include <iostream>

#include <sfinks/engine.hpp>
#include <tic_tac_toe/game.hpp>

auto main() -> int {
  tic_tac_toe::Game game;
  sfinks::Engine engine(&game, "tic_tac_toe");

  constexpr int num_of_learn_games = 1'000'000;
  auto before_learn = std::chrono::high_resolution_clock::now();
  engine.learn(num_of_learn_games);
  auto after_learn = std::chrono::high_resolution_clock::now();
  auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(after_learn - before_learn).count();
  std::cout << "learning " << num_of_learn_games << " games took " << duration << " milliseconds" << std::endl;

  constexpr int num_of_play_games = 10'000;
  for (const auto &player_id : game.player_ids())
    engine.play_against_random_opponents(player_id, num_of_play_games);

  engine.play_according_to_policy(num_of_play_games);
}
