#include <algorithm>
#include <iostream>
#include <numeric>
#include <stdexcept>

#include <bird_lady/consts.hpp>
#include <bird_lady/game.hpp>
#include <bird_lady/scorer.hpp>

namespace bird_lady {

Game::Game(int number_of_players)
    : _players(number_of_players), _deck(number_of_players),
      _mystery_birds(consts::mystery_birds.begin(), consts::mystery_birds.end()) {
  populate_board();
}

auto Game::is_over() const -> bool { return _deck.size() < consts::board_size; }

auto Game::player_ids() const -> std::vector<PlayerId> {
  std::vector<PlayerId> player_ids(_players.size());
  std::iota(player_ids.begin(), player_ids.end(), 0);
  return player_ids;
}

auto Game::current_player_id() const -> PlayerId { return _current_player_id; }

auto Game::player_score(const PlayerId &player_id) const -> double {
  int most_toys = 0;
  for (const auto &player : _players)
    most_toys = std::max(most_toys, player.number_of_available_cards(CardHandle::toy));

  auto [birds_score, toys_score, aviaries_score, eggs_score] = Scorer(&player_by_id(player_id), most_toys).score();
  return birds_score + toys_score + aviaries_score + eggs_score;
}

auto Game::available_actions() const -> std::vector<ActionId> {
  std::vector<ActionId> actions;
  actions.reserve(consts::board_size * 4);
  const auto available_slices = _board.available_slices();

  for (const auto slice_id : available_slices)
    actions.emplace_back(ActionId{slice_id, _current_player_id, _board.slice(slice_id), CardHandle::none});

  if (player_by_id(_current_player_id).number_of_available_cards(CardHandle::egg) >=
          consts::eggs_needed_to_hatch_mystery_bird &&
      !_mystery_birds.empty())
    for (auto mystery_bird : _mystery_birds)
      for (const auto slice_id : available_slices)
        actions.emplace_back(ActionId{slice_id, _current_player_id, _board.slice(slice_id), mystery_bird});

  return actions;
}

auto Game::resource_ids() const -> std::vector<ResourceId> {
  std::vector<ResourceId> resource_ids;
  for (const auto &[card_id, card_count] : _deck.initial_contents())
    resource_ids.push_back(card_id);
  for (const auto mystery_bird_id : consts::mystery_birds)
    resource_ids.push_back(mystery_bird_id);

  return resource_ids;
}

auto Game::resource_count(const ResourceId &resource_id) const -> int {
  return std::find(consts::mystery_birds.begin(), consts::mystery_birds.end(), resource_id) !=
                 consts::mystery_birds.end()
             ? 1
             : (int)_deck.max_card_count(resource_id);
}

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
  if (action_id.player_id != player_id)
    std::throw_with_nested(std::invalid_argument("player_id mismatch"));
  Player &player = player_by_id(player_id);
  for (const auto card : _board.replace(action_id.slice_id, _deck.draw(consts::board_size)))
    player.acquire_card(card);

  if (action_id.mystery_bird != CardHandle::none) {
    _mystery_birds.remove(action_id.mystery_bird);
    player.spend_card(CardHandle::egg, consts::eggs_needed_to_hatch_mystery_bird);
    player.acquire_card(action_id.mystery_bird);
  }

  switch_to_next_player(player_id);
}

void Game::undo_action(const ActionId &action_id, const PlayerId &player_id) {
  if (action_id.player_id != player_id)
    std::throw_with_nested(std::invalid_argument("player_id mismatch"));
  Player &player = player_by_id(player_id);
  const auto &cards = action_id.slice_contents;
  for (const auto card : cards)
    player.lose_card(card);

  _deck.put_on_top(_board.replace(action_id.slice_id, cards));

  if (action_id.mystery_bird != CardHandle::none) {
    player.lose_card(action_id.mystery_bird);
    player.unspend_card(CardHandle::egg, consts::eggs_needed_to_hatch_mystery_bird);
    _mystery_birds.push_back(action_id.mystery_bird);
  }

  switch_to_prev_player(player_id);
}

auto Game::player_by_id(PlayerId player_id) -> Player & { return _players[player_id]; }

auto Game::player_by_id(PlayerId player_id) const -> const Player & { return _players[player_id]; }

void Game::switch_to_next_player(PlayerId player_id) { _current_player_id = (player_id + 1) % _players.size(); }

void Game::switch_to_prev_player(PlayerId player_id) { _current_player_id = (player_id - 1) % _players.size(); }

void Game::populate_board() {
  for (size_t i = 0; i < consts::board_size; i++) {
    _board.replace(i, _deck.draw(consts::board_size));
  }
}

} // namespace bird_lady
