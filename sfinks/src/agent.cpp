#include <sfinks/agent.hpp>

namespace sfinks {

Agent::Agent(LearningParams learning_params) : _learning_params(std::move(learning_params)) {}

void Agent::reset() { _states_seen.clear(); }

void Agent::set_exploration_rate(double exploration_rate) { _learning_params.exploration_rate = exploration_rate; }

void Agent::process_reward(double reward) {
  for (const auto &state : _states_seen) {
    _state_values.try_emplace(state, 0.0);

    auto [exploration_rate, learning_rate, decay_gamma] = _learning_params;
    _state_values.at(state) += learning_rate * (decay_gamma * reward - _state_values.at(state));
    reward = _state_values.at(state);
  }
}

auto Agent::state_value(const std::string &state) const -> double {
  const auto it = _state_values.find(state);
  return it == _state_values.end() ? 0.0 : it->second;
}

} // namespace sfinks
