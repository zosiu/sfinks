#include <sfinks/agent.hpp>

namespace sfinks {

Agent::Agent(bool greedy, LearningParams learning_params) : _greedy(greedy), _learning_params(learning_params) {}

void Agent::reset() { _states_seen.clear(); }

void Agent::set_exploration_rate(double exploration_rate) { _learning_params.exploration_rate = exploration_rate; }

void Agent::set_greedy(bool greedy) { _greedy = greedy; }

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

auto Agent::exploration_rate() const -> double { return _learning_params.exploration_rate; }

auto Agent::greedy() const -> bool { return _greedy; }

auto Agent::number_of_states_seen() const -> size_t { return _state_values.size(); }

} // namespace sfinks
