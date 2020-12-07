#pragma once

#include <sfinks/agent.hpp>
#include <sfinks/game.hpp>
#include <sfinks/game_history.hpp>

namespace sfinks {

enum class DataFormat { json, binary };

template <typename PlayerId, typename ActionId, typename ResourceId>
class Engine {
public:
  Engine(Game<PlayerId, ActionId, ResourceId> *game, const std::string &game_id,
         DataFormat data_format = DataFormat::json);
  void learn(size_t number_of_games_to_play);
  void play_according_to_policy(size_t number_of_games_to_play);
  void play_random(size_t number_of_games_to_play);
  void play_against_random_opponents(const PlayerId &as_player, size_t number_of_games_to_play);
  void play_against_greedy_opponents(const PlayerId &as_player, size_t number_of_games_to_play);

private:
  // TODO: these should be somewhere else
  static auto file_handle(const std::string &name, DataFormat format) -> std::string;
  static void dump_agents_data_json(const std::string &file_name, const std::vector<const Agent *> &agents);
  static void dump_agents_data_binary(const std::string &file_name, const std::vector<const Agent *> &agents);

  [[nodiscard]] auto agents_in_random_order() const -> std::vector<const Agent *>;
  inline void load_agents_data_json();
  inline void load_agents_data_binary();
  void load_agents_data();
  void dump_agents_data() const;
  void dump_history(const std::string &mode) const;
  inline void play_a_single_game(bool record_history);

  Game<PlayerId, ActionId, ResourceId> *_game;
  std::string _agents_data_file_name;
  DataFormat _agents_data_file_format;
  std::string _history_file_name;
  std::unordered_map<PlayerId, Agent> _agents;
  std::vector<GameHistory<PlayerId, ActionId, ResourceId>> _history;
};

} // namespace sfinks

#include "engine.tpp"
