#pragma once

#include <list>
#include <vector>

#include <cereal/types/vector.hpp>

#include <sfinks/game.hpp>

#include <bird_lady/board.hpp>
#include <bird_lady/card_handle.hpp>
#include <bird_lady/deck.hpp>
#include <bird_lady/player.hpp>

namespace bird_lady {

using PlayerId = size_t;

struct ActionId {
  size_t slice_id;
  PlayerId player_id;
  std::vector<CardHandle> slice_contents;
  CardHandle mystery_bird;

  template <class Archive>
  void serialize(Archive &archive) {
    archive(cereal::make_nvp("board_slice", slice_id), cereal::make_nvp("cards", slice_contents),
            cereal::make_nvp("mystery_bird", mystery_bird));
  }
};

using ResourceId = CardHandle;

class Game : public sfinks::Game<PlayerId, ActionId, ResourceId> {
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
  [[nodiscard]] auto player_by_id(PlayerId player_id) -> Player &;
  [[nodiscard]] auto player_by_id(PlayerId player_id) const -> const Player &;
  void switch_to_next_player(PlayerId player_id);
  void switch_to_prev_player(PlayerId player_id);
  void populate_board();

  size_t _current_player_id = 0;
  std::vector<Player> _players;
  Deck _deck;
  Board _board;
  std::list<CardHandle> _mystery_birds;
};

} // namespace bird_lady
