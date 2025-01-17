#pragma once

#include <sstream>

#include <sfinks/resource_data.hpp>

#include "game.hpp"

namespace sfinks {

template <typename PlayerId, typename ActionId, typename ResourceId>
auto encode_state(const typename Game<PlayerId, ActionId, ResourceId>::State &state) -> std::string {
  std::stringstream ss;
  for (const auto &[resource_id, resource_data] : state) {
    ss << resource_data.owned_by_player << "," << (resource_data.more_available ? 1 : 0) << "|";
  }
  return ss.str();
}

template <typename PlayerId, typename ActionId, typename ResourceId>
auto Game<PlayerId, ActionId, ResourceId>::state_from_the_point_of_view_of(const PlayerId &player_id) const
    -> Game::State {
  State state;
  for (const auto &resource_id : resource_ids()) {
    ResourceData resource_data;
    int others = 0;
    for (const auto &pid : player_ids())
      if (pid == player_id)
        resource_data.owned_by_player = resource_count_for_player(resource_id, pid);
      else
        others += resource_count_for_player(resource_id, pid);

    resource_data.more_available = resource_count(resource_id) - resource_data.owned_by_player - others > 0;

    state.emplace(resource_id, resource_data);
  }

  return state;
}

template <typename PlayerId, typename ActionId, typename ResourceId>
auto Game<PlayerId, ActionId, ResourceId>::action_result(const ActionId &action_id, const PlayerId &player_id,
                                                         bool include_score) -> ActionResult<ActionId> {
  perform_action(action_id, player_id);
  const State state_after_action = state_from_the_point_of_view_of(player_id);
  const double score_after_action = include_score ? player_score(player_id) : 0;
  undo_action(action_id, player_id);
  return {action_id, encode_state<PlayerId, ActionId, ResourceId>(state_after_action), score_after_action};
}

template <typename PlayerId, typename ActionId, typename ResourceId>
auto Game<PlayerId, ActionId, ResourceId>::available_actions_with_results(bool include_score)
    -> std::vector<ActionResult<ActionId>> {
  const auto actions = available_actions();
  std::vector<ActionResult<ActionId>> action_results;
  action_results.reserve(actions.size());
  const PlayerId current_player = current_player_id();
  for (const auto &action_id : actions)
    action_results.emplace_back(action_result(action_id, current_player, include_score));

  return action_results;
}

} // namespace sfinks
