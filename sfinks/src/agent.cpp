#include <sfinks/agent.hpp>

namespace sfinks {

Agent::Agent(bool greedy) : _greedy(greedy) {}

void Agent::reset() { _states_seen.clear(); }

void Agent::set_exploration_rate(double exploration_rate) { _exploration_rate = exploration_rate; }

void Agent::set_greedy(bool greedy) { _greedy = greedy; }

void Agent::process_reward(double reward) {
  for (const auto &state : _states_seen) {
    const auto &[it, insertion_result] = _state_values.try_emplace(state);
    auto &[state_value, times_seen] = it->second;
    times_seen += 1;
    state_value += (1.0 / times_seen) * (reward - state_value);
    reward = state_value;
  }
}

auto Agent::state_value(const std::string &state) const -> double {
  const auto it = _state_values.find(state);
  return it == _state_values.end() ? 0.0 : it->second.value;
}

auto Agent::exploration_rate() const -> double { return _exploration_rate; }

auto Agent::greedy() const -> bool { return _greedy; }

auto Agent::number_of_states_seen() const -> size_t { return _state_values.size(); }

} // namespace sfinks
