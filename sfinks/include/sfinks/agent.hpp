#pragma once

#include <list>
#include <string>
#include <unordered_map>
#include <vector>

#include <sfinks/action_result.hpp>

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
  Agent(bool greedy = false, LearningParams learning_params = LearningParams());
  void reset();
  void set_exploration_rate(double exploration_rate);
  void set_greedy(bool greedy);
  template <typename ActionId>
  auto choose_action(const std::vector<ActionResult<ActionId>> &action_results) -> ActionId;
  void process_reward(double reward);
  [[nodiscard]] auto exploration_rate() const -> double;
  [[nodiscard]] auto greedy() const -> bool;

private:
  friend class cereal::access;
  template <class Archive>
  void serialize(Archive &archive);

  [[nodiscard]] auto state_value(const std::string &state) const -> double;
  template <typename ActionId>
  [[nodiscard]] auto random_action(const std::vector<ActionResult<ActionId>> &action_results) const
      -> ActionResult<ActionId>;
  template <typename ActionId>
  [[nodiscard]] auto policy_action(const std::vector<ActionResult<ActionId>> &action_results) const
      -> ActionResult<ActionId>;
  template <typename ActionId>
  [[nodiscard]] auto greedy_action(const std::vector<ActionResult<ActionId>> &action_results) const
      -> ActionResult<ActionId>;

  bool _greedy;
  LearningParams _learning_params;
  std::list<std::string> _states_seen;
  std::unordered_map<std::string, double> _state_values;
};

} // namespace sfinks

#include "agent.tpp"
