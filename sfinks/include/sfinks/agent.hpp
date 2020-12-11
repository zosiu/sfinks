#pragma once

#include <list>
#include <string>
#include <unordered_map>
#include <vector>

#include <cereal/access.hpp>
#include <cereal/cereal.hpp>

#include <sfinks/action_result.hpp>

namespace sfinks {

class Agent {
public:
  static constexpr double default_exploration_rate = 0.4;

  Agent(bool greedy = false);
  void reset();
  void set_exploration_rate(double exploration_rate);
  void set_greedy(bool greedy);
  template <typename ActionId>
  auto choose_action(const std::vector<ActionResult<ActionId>> &action_results) -> ActionId;
  void process_reward(double reward);
  [[nodiscard]] auto exploration_rate() const -> double;
  [[nodiscard]] auto greedy() const -> bool;
  [[nodiscard]] auto number_of_states_seen() const -> size_t;

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

  double _exploration_rate = default_exploration_rate;
  bool _greedy = false;
  std::list<std::string> _states_seen;

  struct StateInfo {
    double value = 0;
    size_t times_seen = 0;

    template <class Archive>
    void serialize(Archive &archive) {
      archive(CEREAL_NVP(value), CEREAL_NVP(times_seen));
    }
  };
  std::unordered_map<std::string, StateInfo> _state_values;
};

} // namespace sfinks

#include "agent.tpp"
