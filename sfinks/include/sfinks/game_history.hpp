#pragma once

#include <tuple>
#include <unordered_map>
#include <vector>

#include <sfinks/resource_data.hpp>

namespace sfinks {

template <typename PlayerId, typename ActionId, typename ResourceId>
struct GameHistory {
  std::vector<std::tuple<PlayerId, ActionId>> actions = {};
  std::unordered_map<PlayerId, std::unordered_map<ResourceId, ResourceData>> state_by_players = {};
  std::unordered_map<PlayerId, double> results = {};

  template <class Archive>
  void serialize(Archive &archive) {
    archive(CEREAL_NVP(results), CEREAL_NVP(actions), CEREAL_NVP(state_by_players));
  }
};

} // namespace sfinks
