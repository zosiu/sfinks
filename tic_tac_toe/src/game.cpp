#include <algorithm>
#include <numeric>
#include <stdexcept>

#include <tic_tac_toe/game.hpp>

namespace tic_tac_toe {

Game::Game() : _current_player(Mark::x), _board({Mark::none}) {}

auto Game::is_over() const -> bool { return winner() != Mark::none || board_is_full(); }

auto Game::player_ids() const -> std::vector<PlayerId> { return {Mark::x, Mark::o}; }

auto Game::current_player_id() const -> PlayerId { return _current_player; }

auto Game::player_score(const PlayerId &player_id) const -> double {
  if (is_over() && winner() == Mark::none)
    return 1;

  return is_winner(player_id) ? 2 : 0;
}

auto Game::available_actions() const -> std::vector<ActionId> {
  std::vector<ActionId> action_ids;
  for (size_t i = 0; i < _board.size(); i++)
    if (_board.at(i) == Mark::none)
      action_ids.push_back(i);
  return action_ids;
}

auto Game::resource_ids() const -> std::vector<ResourceId> {
  std::vector<ResourceId> resource_ids(_board.size());
  std::iota(resource_ids.begin(), resource_ids.end(), 0);
  return resource_ids;
}

auto Game::resource_count(const ResourceId & /*resource_id*/) const -> int { return 1; }

auto Game::resource_count_for_player(const ResourceId &resource_id, const PlayerId &player_id) const -> int {
  return _board.at(resource_id) == player_id ? 1 : 0;
}

void Game::reset() { _board.fill(Mark::none); }

void Game::perform_action(const ActionId &action_id, const PlayerId &player_id) {
  if (_board.at(action_id) != Mark::none || is_over())
    std::throw_with_nested(std::invalid_argument("action unavailable"));

  _board.at(action_id) = player_id;
  switch_player();
}

void Game::undo_action(const ActionId &action_id, const PlayerId &player_id) {
  if (_board.at(action_id) != player_id)
    std::throw_with_nested(std::invalid_argument("invalid undo"));

  _board.at(action_id) = Mark::none;
  switch_player();
}

auto Game::board_is_full() const -> bool {
  return std::all_of(_board.begin(), _board.end(), [](const auto x) { return x != Mark::none; });
}

auto Game::winner() const -> Mark {
  if (is_winner(Mark::o))
    return Mark::o;
  if (is_winner(Mark::x))
    return Mark::x;

  return Mark::none;
}

auto Game::is_winner(Mark mark) const -> bool {
  const auto [b1, b2, b3, b4, b5, b6, b7, b8, b9] = _board;

  return ((b1 == mark && b2 == mark && b3 == mark) || (b4 == mark && b5 == mark && b6 == mark) ||
          (b7 == mark && b8 == mark && b9 == mark) ||

          (b1 == mark && b4 == mark && b7 == mark) || (b2 == mark && b5 == mark && b8 == mark) ||
          (b3 == mark && b6 == mark && b9 == mark) ||

          (b1 == mark && b5 == mark && b9 == mark) || (b3 == mark && b5 == mark && b7 == mark));
}

void Game::switch_player() { _current_player = _current_player == Mark::x ? Mark::o : Mark::x; }

} // namespace tic_tac_toe
