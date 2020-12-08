#pragma once

#include <algorithm>
#include <chrono>
#include <cmath>
#include <fstream>
#include <iostream>
#include <limits>
#include <random>
#include <sstream>
#include <stdexcept>

#include <cereal/archives/json.hpp>
#include <cereal/archives/portable_binary.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/tuple.hpp>
#include <cereal/types/vector.hpp>

#include "engine.hpp"

namespace sfinks {

template <typename PlayerId, typename ActionId, typename ResourceId>
Engine<PlayerId, ActionId, ResourceId>::Engine(Game<PlayerId, ActionId, ResourceId> *game, const std::string &game_id,
                                               DataFormat data_format)
    : _game(game), _agents_data_file_name(game_id + "_state_values"), _agents_data_file_format(data_format),
      _history_file_name(game_id + "_history") {
  for (const auto &player_id : _game->player_ids()) {
    _agents.emplace(player_id, Agent());
  }

  load_agents_data();
  // for (const auto &[player_id, agent] : _agents)
  // std::cout << agent.number_of_states_seen() << std::endl;
}

template <typename PlayerId, typename ActionId, typename ResourceId>
void Engine<PlayerId, ActionId, ResourceId>::learn(size_t number_of_games_to_play) {
  for (auto &[player_id, agent] : _agents)
    agent.set_exploration_rate(LearningParams::default_exploration_rate);

  for (size_t i = 0; i < number_of_games_to_play; i++)
    play_a_single_game(false);

  dump_agents_data();
}

template <typename PlayerId, typename ActionId, typename ResourceId>
void Engine<PlayerId, ActionId, ResourceId>::play_according_to_policy(size_t number_of_games_to_play) {
  for (auto &[player_id, agent] : _agents) {
    agent.set_greedy(false);
    agent.set_exploration_rate(0.0);
  }
  _history.clear();

  for (size_t i = 0; i < number_of_games_to_play; i++)
    play_a_single_game(true);

  // dump_agents_data();
  dump_history("according_to_policy");
}

template <typename PlayerId, typename ActionId, typename ResourceId>
void Engine<PlayerId, ActionId, ResourceId>::play_random(size_t number_of_games_to_play) {
  for (auto &[player_id, agent] : _agents) {
    agent.set_greedy(false);
    agent.set_exploration_rate(1.0);
  }
  _history.clear();

  for (size_t i = 0; i < number_of_games_to_play; i++)
    play_a_single_game(true);

  // dump_agents_data();
  dump_history("random");
}

template <typename PlayerId, typename ActionId, typename ResourceId>
void Engine<PlayerId, ActionId, ResourceId>::play_against_random_opponents(const PlayerId &as_player,
                                                                           size_t number_of_games_to_play) {
  for (auto &[player_id, agent] : _agents) {
    agent.set_greedy(false);
    agent.set_exploration_rate(as_player == player_id ? 0.0 : 1.0);
  }
  _history.clear();

  for (size_t i = 0; i < number_of_games_to_play; i++)
    play_a_single_game(true);

  dump_history("against_random");
}

template <typename PlayerId, typename ActionId, typename ResourceId>
void Engine<PlayerId, ActionId, ResourceId>::play_against_greedy_opponents(const PlayerId &as_player,
                                                                           size_t number_of_games_to_play) {
  for (auto &[player_id, agent] : _agents) {
    agent.set_exploration_rate(0.0);
    agent.set_greedy(as_player != player_id);
  }
  _history.clear();

  for (size_t i = 0; i < number_of_games_to_play; i++)
    play_a_single_game(true);

  dump_history("against_greedy");
}

template <typename PlayerId, typename ActionId, typename ResourceId>
auto Engine<PlayerId, ActionId, ResourceId>::file_handle(const std::string &name, DataFormat format) -> std::string {
  switch (format) {
  case DataFormat::json:
    return name + ".json";
  case DataFormat::binary:
    return name + ".sfnks";
  default:
    std::throw_with_nested(std::invalid_argument("unknown format"));
  }
}

template <typename PlayerId, typename ActionId, typename ResourceId>
void Engine<PlayerId, ActionId, ResourceId>::dump_agents_data_json(const std::string &file_name,
                                                                   const std::vector<const Agent *> &agents) {
  std::ofstream outfile(file_handle(file_name, DataFormat::json));
  cereal::JSONOutputArchive archive(outfile);
  for (const auto agent : agents)
    archive(*agent);
}

template <typename PlayerId, typename ActionId, typename ResourceId>
void Engine<PlayerId, ActionId, ResourceId>::dump_agents_data_binary(const std::string &file_name,
                                                                     const std::vector<const Agent *> &agents) {
  std::ofstream outfile(file_handle(file_name, DataFormat::binary), std::ios::binary);
  cereal::PortableBinaryOutputArchive archive(outfile);
  for (const auto agent : agents)
    archive(*agent);
}

template <typename PlayerId, typename ActionId, typename ResourceId>
auto Engine<PlayerId, ActionId, ResourceId>::agents_in_random_order() const -> std::vector<const Agent *> {
  std::vector<const Agent *> agents;
  for (const auto &[player_id, agent] : _agents)
    agents.push_back(&agent);

  std::random_device rng_device;
  std::mt19937 rng{rng_device()};
  std::shuffle(agents.begin(), agents.end(), rng);

  return agents;
}

template <typename PlayerId, typename ActionId, typename ResourceId>
void Engine<PlayerId, ActionId, ResourceId>::load_agents_data_json() {
  std::ifstream infile(file_handle(_agents_data_file_name, DataFormat::json));
  if (infile.fail()) {
    // TODO: log warning
  } else {
    cereal::JSONInputArchive iarchive(infile);
    for (auto &[player_id, agent] : _agents)
      iarchive(agent);
  }
}

template <typename PlayerId, typename ActionId, typename ResourceId>
void Engine<PlayerId, ActionId, ResourceId>::load_agents_data_binary() {
  std::ifstream infile(file_handle(_agents_data_file_name, DataFormat::binary), std::ios::binary);
  if (infile.fail()) {
    // TODO: log warning
  } else {
    cereal::PortableBinaryInputArchive iarchive(infile);
    for (auto &[player_id, agent] : _agents)
      iarchive(agent);
  }
}

template <typename PlayerId, typename ActionId, typename ResourceId>
void Engine<PlayerId, ActionId, ResourceId>::load_agents_data() {
  switch (_agents_data_file_format) {
  case DataFormat::json:
    load_agents_data_json();
    break;
  case DataFormat::binary:
    load_agents_data_binary();
    break;
  }
}

template <typename PlayerId, typename ActionId, typename ResourceId>
void Engine<PlayerId, ActionId, ResourceId>::dump_agents_data() const {
  switch (_agents_data_file_format) {
  case DataFormat::json:
    dump_agents_data_json(_agents_data_file_name, agents_in_random_order());
    break;
  case DataFormat::binary:
    dump_agents_data_binary(_agents_data_file_name, agents_in_random_order());
    break;
  }
}

template <typename PlayerId, typename ActionId, typename ResourceId>
void Engine<PlayerId, ActionId, ResourceId>::dump_history(const std::string &mode) const {
  std::stringstream ss;
  ss << _history_file_name << "_" << mode << "_";
  ss << std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch())
            .count();
  std::ofstream outfile(file_handle(ss.str(), DataFormat::json));
  cereal::JSONOutputArchive archive(outfile);
  archive(cereal::make_nvp("history", _history));
}

template <typename PlayerId, typename ActionId, typename ResourceId>
void Engine<PlayerId, ActionId, ResourceId>::play_a_single_game(bool record_history) {
  GameHistory<PlayerId, ActionId, ResourceId> *history =
      record_history ? &*_history.insert(_history.end(), GameHistory<PlayerId, ActionId, ResourceId>{}) : nullptr;

  _game->reset();
  for (auto &[player_id, agent] : _agents)
    agent.reset();

  while (!_game->is_over()) {
    PlayerId current_player = _game->current_player_id();
    Agent *current_agent = &_agents.at(current_player);

    ActionId choosen_action =
        current_agent->choose_action(_game->available_actions_with_results(current_agent->greedy()));
    _game->perform_action(choosen_action, current_player);

    if (history != nullptr)
      history->actions.emplace_back(std::tuple<PlayerId, ActionId>{current_player, choosen_action});
  }

  std::unordered_map<PlayerId, double> scores;
  for (auto &[player_id, agent] : _agents)
    scores.emplace(player_id, _game->player_score(player_id));
  double winning_score = std::max_element(scores.begin(), scores.end(), [](const auto &p1, const auto &p2) {
                           return p1.second < p2.second;
                         })->second;

  constexpr double winning_reward = 1.0;
  constexpr double losing_penalty = -0.1;
  for (auto &[player_id, agent] : _agents)
    agent.process_reward(std::fabs(scores[player_id] - winning_score) <= std::numeric_limits<double>::epsilon()
                             ? winning_reward
                             : losing_penalty);

  if (history != nullptr) {
    std::unordered_map<PlayerId, AgentScore> results;
    for (const auto &[player_id, score] : scores) {
      const Agent &agent = _agents.at(player_id);
      results[player_id] = {score, agent.greedy(), agent.exploration_rate()};
    }
    history->results = results;

    for (const auto &[player_id, agent] : _agents)
      history->state_by_players.emplace(player_id, _game->state_from_the_point_of_view_of(player_id));
  }
}

} // namespace sfinks
