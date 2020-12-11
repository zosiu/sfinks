#pragma once

#include <numeric>
#include <random>
#include <stdexcept>

#include <cereal/types/unordered_map.hpp>

#include "agent.hpp"

namespace sfinks {

template <typename ActionId>
auto Agent::choose_action(const std::vector<ActionResult<ActionId>> &action_results) -> ActionId {
  if (action_results.empty())
    std::throw_with_nested(std::invalid_argument("no actions to choose from"));

  if (_greedy)
    return greedy_action(action_results).action_id;

  std::random_device rng_device;
  std::mt19937 rng{rng_device()};
  std::uniform_real_distribution<> uniform(0.0, 1.0);

  const auto &[action_id, state, score] =
      uniform(rng) < _exploration_rate ? random_action(action_results) : policy_action(action_results);
  _states_seen.push_front(state);
  return action_id;
}

template <typename ActionId>
auto Agent::policy_action(const std::vector<ActionResult<ActionId>> &action_results) const -> ActionResult<ActionId> {
  return *std::max_element(action_results.begin(), action_results.end(), [this](const auto &lhs, const auto &rhs) {
    return state_value(lhs.state) < state_value(rhs.state);
  });
}

template <typename ActionId>
auto Agent::random_action(const std::vector<ActionResult<ActionId>> &action_results) const -> ActionResult<ActionId> {
  auto it = action_results.begin();
  std::random_device rng_device;
  std::mt19937 rng{rng_device()};
  std::uniform_int_distribution<> uniform(0, action_results.size() - 1);
  std::advance(it, uniform(rng));
  return *it;
}

template <typename ActionId>
auto Agent::greedy_action(const std::vector<ActionResult<ActionId>> &action_results) const -> ActionResult<ActionId> {
  return *std::max_element(action_results.begin(), action_results.end(),
                           [this](const auto &lhs, const auto &rhs) { return lhs.score < rhs.score; });
}

template <class Archive>
void Agent::serialize(Archive &archive) {
  archive(_state_values);
}

} // namespace sfinks
