#include <iostream>
#include <numeric>

#include <bird_lady/consts.hpp>
#include <bird_lady/game.hpp>

namespace bird_lady {

Game::Game(int number_of_players) : _players(number_of_players), _deck(number_of_players) { populate_board(); }

auto Game::is_over() const -> bool { return _deck.size() < board_size; }

auto Game::player_ids() const -> std::vector<PlayerId> {
  std::vector<PlayerId> player_ids(_players.size());
  std::iota(player_ids.begin(), player_ids.end(), 0);
  return player_ids;
}

auto Game::current_player_id() const -> PlayerId { return _current_player_id; }

auto Game::player_score(const PlayerId &player_id) const -> double { return 0; }

auto Game::available_actions() const -> std::vector<ActionId> {
  std::vector<ActionId> actions;
  for (const auto slice_id : _board.available_slices())
    actions.emplace_back(ActionId{slice_id, _board.slice(slice_id)});

  return actions;
}

auto Game::resource_ids() const -> std::vector<ResourceId> {
  std::vector<ResourceId> resource_ids;
  for (const auto &[card_id, card_count] : _deck.initial_contents())
    resource_ids.push_back(card_id);
  return resource_ids;
}

auto Game::resource_count(const ResourceId &resource_id) const -> int { return _deck.max_card_count(resource_id); }

auto Game::resource_count_for_player(const ResourceId &resource_id, const PlayerId &player_id) const -> int {
  return player_by_id(player_id).number_of_available_cards(resource_id);
}

void Game::reset() {
  _deck.reset();
  _board.reset();

  for (auto &player : _players)
    player.reset();
  _current_player_id = 0;

  populate_board();
}

void Game::perform_action(const ActionId &action_id, const PlayerId &player_id) {
  Player &player = player_by_id(player_id);
  for (const auto card : _board.replace(action_id.slice_id, _deck.draw(board_size)))
    player.acquire_card(card);

  switch_to_next_player();
}

void Game::undo_action(const ActionId &action_id, const PlayerId &player_id) {
  auto [slice_id, cards] = action_id;
  Player &player = player_by_id(player_id);
  for (const auto card : cards)
    player.lose_card(card);

  _deck.put_on_top(_board.replace(slice_id, cards));

  switch_to_prev_player();
}

auto Game::player_by_id(PlayerId player_id) -> Player & { return _players[player_id]; }

auto Game::player_by_id(PlayerId player_id) const -> const Player & { return _players[player_id]; }

void Game::switch_to_next_player() { _current_player_id = (_current_player_id + 1) % _players.size(); }

void Game::switch_to_prev_player() { _current_player_id = (_current_player_id - 1) % _players.size(); }

void Game::populate_board() {
  for (size_t i = 0; i < board_size; i++) {
    _board.replace(i, _deck.draw(board_size));
  }
}

} // namespace bird_lady
