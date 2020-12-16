#pragma once

#include <array>

#include <sfinks/game.hpp>

namespace tic_tac_toe {

enum class Mark { none, x, o };
constexpr int board_size = 9;
using Board = std::array<Mark, board_size>;

using PlayerId = Mark;
using ActionId = size_t;
using ResourceId = size_t;

class Game : public sfinks::Game<PlayerId, ActionId, ResourceId> {
public:
  Game();

  [[nodiscard]] auto is_over() const -> bool override;

  [[nodiscard]] auto player_ids() const -> std::vector<PlayerId> override;
  [[nodiscard]] auto current_player_id() const -> PlayerId override;
  [[nodiscard]] auto player_score(const PlayerId &player_id) const -> double override;

  [[nodiscard]] auto available_actions() const -> std::vector<ActionId> override;

  [[nodiscard]] auto resource_ids() const -> std::vector<ResourceId> override;
  [[nodiscard]] auto resource_count(const ResourceId &resource_id) const -> size_t override;
  [[nodiscard]] auto resource_count_for_player(const ResourceId &resource_id, const PlayerId &player_id) const
      -> size_t override;

  void reset() override;
  void perform_action(const ActionId &action_id, const PlayerId &player_id) override;
  void undo_action(const ActionId &action_id, const PlayerId &player_id) override;

private:
  [[nodiscard]] inline auto board_is_full() const -> bool;
  [[nodiscard]] inline auto winner() const -> Mark;
  [[nodiscard]] inline auto is_winner(Mark mark) const -> bool;
  inline void switch_player();

  Mark _current_player;
  Board _board;
};

} // namespace tic_tac_toe
