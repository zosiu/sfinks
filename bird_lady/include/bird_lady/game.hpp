#pragma once

#include <vector>

#include <sfinks/game.hpp>

#include <bird_lady/consts.hpp>
#include <bird_lady/player.hpp>

namespace bird_lady {

struct ActionId {};
using PlayerId = size_t;
using ResourceId = CardHandle;

class Game : sfinks::Game<PlayerId, ActionId, ResourceId> {
public:
  Game(int number_of_players);

  [[nodiscard]] auto is_over() const -> bool override;

  [[nodiscard]] auto player_ids() const -> std::vector<PlayerId> override;
  [[nodiscard]] auto current_player_id() const -> PlayerId override;
  [[nodiscard]] auto player_score(const PlayerId &player_id) const -> double override;

  [[nodiscard]] auto available_actions() const -> std::vector<ActionId> override;

  [[nodiscard]] auto resource_ids() const -> std::vector<ResourceId> override;
  [[nodiscard]] auto resource_count(const ResourceId &resource_id) const -> int override;
  [[nodiscard]] auto resource_count_for_player(const ResourceId &resource_id, const PlayerId &player_id) const
      -> int override;

  void reset() override;
  void perform_action(const ActionId &action_id, const PlayerId &player_id) override;
  void undo_action(const ActionId &action_id, const PlayerId &player_id) override;

private:
  [[nodiscard]] auto current_player() -> Player &;
  [[nodiscard]] auto player_by_id(PlayerId player_id) const -> const Player &;
  void switch_to_next_player();
  void switch_to_prev_player();

  size_t _current_player_id = 0;
  std::vector<Player> _players;
};

} // namespace bird_lady
