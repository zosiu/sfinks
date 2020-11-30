#pragma once

#include <unordered_map>
#include <vector>

#include <cereal/cereal.hpp>

#include <sfinks/game.hpp>

namespace colors {

enum class Color { none, red, green, blue, yellow };

using PlayerId = size_t;
using ResourceId = Color;

struct ActionId {
  PlayerId player;
  Color color_to_take;
  Color color_taken_before;

  template <class Archive>
  void serialize(Archive &archive) {
    archive(cereal::make_nvp("player", player), cereal::make_nvp("color_taken", color_to_take));
  }
};

struct Player {
  std::unordered_map<Color, size_t> colors = {
      {Color::none, 0}, {Color::red, 0}, {Color::green, 0}, {Color::blue, 0}, {Color::yellow, 0}};
  Color last_color_taken = Color::none;

  void reset() {
    colors = {{Color::none, 0}, {Color::red, 0}, {Color::green, 0}, {Color::blue, 0}, {Color::yellow, 0}};
    last_color_taken = Color::none;
  }
};

class Game : public sfinks::Game<PlayerId, ActionId, ResourceId> {
public:
  static const std::unordered_map<Color, size_t> color_counts;
  static const size_t color_counts_sum;
  static const std::unordered_map<Color, Color> color_pairs;
  static const std::unordered_map<Color, int> color_values;

  Game(size_t number_of_players);

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
  [[nodiscard]] auto player_for_id(PlayerId player_id) -> Player &;
  [[nodiscard]] auto player_for_id(PlayerId player_id) const -> const Player &;
  void switch_to_next_player(PlayerId player_id);
  void switch_to_prev_player(PlayerId player_id);

  std::vector<Player> _players;
  size_t _current_player_id = 0;
  std::unordered_map<Color, size_t> _available_colors;
};

} // namespace colors
