#pragma once

#include <unordered_map>
#include <vector>

namespace sfinks {

class ResourceData;

template <typename PlayerId, typename ActionId, typename ResourceId>
class Game {
public:
  [[nodiscard]] virtual auto is_over() const -> bool = 0;

  [[nodiscard]] virtual auto player_ids() const -> std::vector<PlayerId> = 0;
  [[nodiscard]] virtual auto current_player_id() const -> PlayerId = 0;
  [[nodiscard]] virtual auto player_score(const PlayerId &player_id) const -> double = 0;

  [[nodiscard]] virtual auto available_actions() const -> std::vector<ActionId> = 0;

  [[nodiscard]] virtual auto resource_ids() const -> std::vector<ResourceId> = 0;
  [[nodiscard]] virtual auto resource_count(const ResourceId &resource_id) const -> size_t = 0;
  [[nodiscard]] virtual auto resource_count_for_player(const ResourceId &resource_id, const PlayerId &player_id) const
      -> size_t = 0;

  virtual void reset() = 0;
  virtual void perform_action(const ActionId &action_id, const PlayerId &player_id) = 0;
  virtual void undo_action(const ActionId &action_id, const PlayerId &player_id) = 0;

  using State = std::unordered_map<ResourceId, ResourceData>;
  [[nodiscard]] auto state_from_the_point_of_view_of(const PlayerId &player_id) const -> State;
  [[nodiscard]] auto state_after_action(const ActionId &action_id, const PlayerId &player_id) -> State;
  [[nodiscard]] auto available_actions_with_results() -> std::vector<std::pair<ActionId, std::string>>;
};

} // namespace sfinks

#include "game.tpp"
