#include <chrono>
#include <iostream>
#include <string>

#include <colors/game.hpp>
#include <sfinks/engine.hpp>

auto main() -> int {
  constexpr size_t num_of_players = 4;
  colors::Game game(num_of_players);
  sfinks::Engine engine(&game, "colors_" + std::to_string(num_of_players) + "p", sfinks::DataFormat::binary);

  constexpr int num_of_learn_games = 40'000'000;
  constexpr int learn_batch_size = 200'000;
  for (int i = 0; i < num_of_learn_games; i += learn_batch_size) {
    auto before_learn = std::chrono::high_resolution_clock::now();
    engine.learn(learn_batch_size);
    auto after_learn = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(after_learn - before_learn).count();
    std::cout << "learning " << learn_batch_size << " games took " << duration << " milliseconds" << std::endl;
  }

  constexpr int num_of_play_games = 10'000;
  engine.play_random(num_of_play_games);
  engine.play_according_to_policy(num_of_play_games);
  for (const auto player_id : game.player_ids()) {
    engine.play_against_random_opponents(player_id, num_of_play_games);
    engine.play_against_greedy_opponents(player_id, num_of_play_games);
  }
}
