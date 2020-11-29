#include <algorithm>
#include <numeric>
#include <stdexcept>

#include <bird_lady/consts.hpp>
#include <bird_lady/game.hpp>
#include <bird_lady/scorer.hpp>

namespace bird_lady {

Game::Game(int number_of_players)
    : _players(number_of_players), _mystery_birds(consts::mystery_birds.begin(), consts::mystery_birds.end()) {
  populate_board();
}

auto Game::is_over() const -> bool { return _deck.size() < Board::size; }

auto Game::player_ids() const -> std::vector<PlayerId> {
  std::vector<PlayerId> player_ids(_players.size());
  std::iota(player_ids.begin(), player_ids.end(), 0);
  return player_ids;
}

auto Game::current_player_id() const -> PlayerId { return _current_player_id; }

auto Game::player_score(const PlayerId &player_id) const -> double {
  auto [birds_score, aviaries_score, eggs_score] = Scorer::score(player_by_id(player_id));
  return birds_score + aviaries_score + eggs_score;
}

auto Game::available_actions() const -> std::vector<ActionId> {
  std::vector<ActionId> actions;
  const auto available_slices = Board::slice_ids;
  actions.reserve(available_slices.size() * consts::mystery_birds.size());

  if (player_by_id(_current_player_id).number_of_available_cards(CardHandle::egg) >=
          consts::eggs_needed_to_hatch_mystery_bird &&
      !_mystery_birds.empty())
    for (auto mystery_bird : _mystery_birds)
      for (const auto slice_id : available_slices)
        actions.emplace_back(ActionId{slice_id, _current_player_id, _board.slice(slice_id), mystery_bird});

  for (const auto slice_id : available_slices)
    actions.emplace_back(ActionId{slice_id, _current_player_id, _board.slice(slice_id), CardHandle::none});

  return actions;
}

auto Game::resource_ids() const -> std::vector<ResourceId> {
  std::vector<ResourceId> resource_ids;
  resource_ids.reserve(consts::card_counts.size() + consts::mystery_birds.size());

  for (const auto &[card_id, card_count] : consts::card_counts)
    resource_ids.push_back(card_id);
  for (const auto mystery_bird_id : consts::mystery_birds)
    resource_ids.push_back(mystery_bird_id);

  return resource_ids;
}

auto Game::resource_count(const ResourceId &resource_id) const -> size_t {
  return std::find(consts::mystery_birds.begin(), consts::mystery_birds.end(), resource_id) !=
                 consts::mystery_birds.end()
             ? 1
             : consts::card_counts.at(resource_id);
}

auto Game::resource_count_for_player(const ResourceId &resource_id, const PlayerId &player_id) const -> size_t {
  return player_by_id(player_id).number_of_available_cards(resource_id);
}

void Game::reset() {
  _deck.reset();
  _board.reset();

  for (auto &player : _players)
    player.reset();
  _current_player_id = 0;

  populate_board();
  _mystery_birds.clear();
  for (const auto mystery_bird : consts::mystery_birds)
    _mystery_birds.push_back(mystery_bird);
}

void Game::perform_action(const ActionId &action_id, const PlayerId &player_id) {
  if (action_id.player_id != player_id)
    std::throw_with_nested(std::invalid_argument("player_id mismatch"));
  Player &player = player_by_id(player_id);
  const auto [card1, card2] = _board.replace(action_id.slice_id, _deck.draw_for_board());
  player.acquire_card(card1);
  player.acquire_card(card2);

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
  const auto [card1, card2] = action_id.slice_contents;
  player.lose_card(card1);
  player.lose_card(card2);

  const auto [deck_card1, deck_card2] = _board.replace(action_id.slice_id, {card1, card2});
  _deck.put_on_top(deck_card2);
  _deck.put_on_top(deck_card1);

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
  for (size_t i = 0; i < Board::size; i++) {
    _board.replace(i, _deck.draw_for_board());
  }
}

} // namespace bird_lady
