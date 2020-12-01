#pragma once

#include <tuple>
#include <unordered_map>
#include <vector>

#include <cereal/cereal.hpp>

namespace sfinks {

class ResourceData;

struct AgentScore {
  double result;
  bool greedy;
  double exploration_rate;

  template <class Archive>
  void serialize(Archive &archive) {
    archive(CEREAL_NVP(result), CEREAL_NVP(greedy), CEREAL_NVP(exploration_rate));
  }
};

template <typename PlayerId, typename ActionId, typename ResourceId>
struct GameHistory {
  std::vector<std::tuple<PlayerId, ActionId>> actions = {};
  std::unordered_map<PlayerId, std::unordered_map<ResourceId, ResourceData>> state_by_players = {};
  std::unordered_map<PlayerId, AgentScore> results = {};

  template <class Archive>
  void serialize(Archive &archive) {
    archive(CEREAL_NVP(results), CEREAL_NVP(actions), CEREAL_NVP(state_by_players));
  }
};

} // namespace sfinks
