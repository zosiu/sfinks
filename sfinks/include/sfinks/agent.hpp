#pragma once

#include <list>
#include <string>
#include <unordered_map>
#include <vector>

#include <cereal/access.hpp>

namespace sfinks {

struct LearningParams {
  static constexpr double default_exploration_rate = 0.4;
  static constexpr double default_learning_rate = 0.2;
  static constexpr double default_decay_gamma = 0.9;

  double exploration_rate = default_exploration_rate;
  double learning_rate = default_learning_rate;
  double decay_gamma = default_decay_gamma;
};

class Agent {
public:
  Agent(LearningParams learning_params = LearningParams());
  void reset();
  void set_exploration_rate(double exploration_rate);
  template <typename ActionId>
  auto choose_action(const std::vector<std::pair<ActionId, std::string>> &actions_with_resulting_states) -> ActionId;
  void process_reward(double reward);

private:
  friend class cereal::access;
  template <class Archive>
  void serialize(Archive &archive);

  [[nodiscard]] auto state_value(const std::string &state) const -> double;
  template <typename ActionId>
  [[nodiscard]] auto
  random_action(const std::vector<std::pair<ActionId, std::string>> &actions_with_resulting_states) const
      -> std::pair<ActionId, std::string>;
  template <typename ActionId>
  [[nodiscard]] auto
  policy_action(const std::vector<std::pair<ActionId, std::string>> &actions_with_resulting_states) const
      -> std::pair<ActionId, std::string>;

  LearningParams _learning_params;
  std::list<std::string> _states_seen;
  std::unordered_map<std::string, double> _state_values;
};

} // namespace sfinks

#include "agent.tpp"
