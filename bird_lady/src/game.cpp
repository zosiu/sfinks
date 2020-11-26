#include <iostream>
#include <numeric>

#include <bird_lady/consts.hpp>
#include <bird_lady/game.hpp>

namespace bird_lady {

Game::Game(int number_of_players) : _players(number_of_players), _deck(number_of_players) { populate_board(); }

auto Game::is_over() const -> bool { return true; }

auto Game::player_ids() const -> std::vector<PlayerId> {
  std::vector<PlayerId> player_ids(_players.size());
  std::iota(player_ids.begin(), player_ids.end(), 0);
  return player_ids;
}

auto Game::current_player_id() const -> PlayerId { return _current_player_id; }

auto Game::player_score(const PlayerId &player_id) const -> double { return 0; }

auto Game::available_actions() const -> std::vector<ActionId> { return {}; }

auto Game::resource_ids() const -> std::vector<ResourceId> { return {}; }

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

void Game::perform_action(const ActionId &action_id, const PlayerId &player_id) { switch_to_next_player(); }

void Game::undo_action(const ActionId &action_id, const PlayerId &player_id) { switch_to_prev_player(); }

auto Game::current_player() -> Player & { return _players[_current_player_id]; }

auto Game::player_by_id(PlayerId player_id) const -> const Player & { return _players[player_id]; }

void Game::switch_to_next_player() { _current_player_id = (_current_player_id + 1) % _players.size(); }

void Game::switch_to_prev_player() { _current_player_id = (_current_player_id - 1) % _players.size(); }

void Game::populate_board() {
  for (size_t i = 0; i < board_size; i++) {
    _board.replace(i, _deck.draw(board_size));
  }
}

} // namespace bird_lady
