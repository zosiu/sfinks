#pragma once

#include <numeric>
#include <random>
#include <stdexcept>

#include <cereal/types/unordered_map.hpp>

#include "agent.hpp"

namespace sfinks {

template <typename ActionId>
auto Agent::choose_action(const std::vector<std::pair<ActionId, std::string>> &actions_with_resulting_states)
    -> ActionId {
  if (actions_with_resulting_states.empty())
    std::throw_with_nested(std::invalid_argument("no actions to choose from"));

  std::random_device rng_device;
  std::mt19937 rng{rng_device()};
  std::uniform_real_distribution<> uniform(0.0, 1.0);

  auto [action_id, state] = uniform(rng) < _learning_params.exploration_rate
                                ? random_action(actions_with_resulting_states)
                                : policy_action(actions_with_resulting_states);
  _states_seen.push_front(state);
  return action_id;
}

template <typename ActionId>
auto Agent::policy_action(const std::vector<std::pair<ActionId, std::string>> &actions_with_resulting_states) const
    -> std::pair<ActionId, std::string> {
  return *std::max_element(
      actions_with_resulting_states.begin(), actions_with_resulting_states.end(),
      [this](const auto &lhs, const auto &rhs) { return state_value(lhs.second) < state_value(rhs.second); });
}

template <typename ActionId>
auto Agent::random_action(const std::vector<std::pair<ActionId, std::string>> &actions_with_resulting_states) const
    -> std::pair<ActionId, std::string> {
  auto it = actions_with_resulting_states.begin();
  std::random_device rng_device;
  std::mt19937 rng{rng_device()};
  std::uniform_int_distribution<> uniform(0, actions_with_resulting_states.size() - 1);
  std::advance(it, uniform(rng));
  return *it;
}

template <class Archive>
void Agent::serialize(Archive &archive) {
  archive(_state_values);
}

} // namespace sfinks
