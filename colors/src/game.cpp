#include <algorithm>
#include <numeric>
#include <stdexcept>

#include <colors/game.hpp>

namespace colors {
const std::unordered_map<Color, size_t> Game::color_counts = {
    {Color::red, 3}, {Color::green, 5}, {Color::blue, 2}, {Color::yellow, 4}};

const size_t Game::color_counts_sum =
    std::accumulate(color_counts.begin(), color_counts.end(), 0,
                    [](const auto sum, const auto &kv_pair) { return sum + kv_pair.second; });

const std::unordered_map<Color, Color> Game::color_pairs = {{Color::red, Color::blue}, {Color::green, Color::yellow}};

const std::unordered_map<Color, int> Game::color_values = {
    {Color::red, 7}, {Color::green, 4}, {Color::blue, -4}, {Color::yellow, 0}, {Color::none, -8}};

Game::Game(size_t number_of_players) : _players(number_of_players, Player()), _available_colors(color_counts) {}

auto Game::is_over() const -> bool {
  return std::all_of(_available_colors.begin(), _available_colors.end(),
                     [](const auto &kv_pair) { return kv_pair.second == 0; });
}

auto Game::player_ids() const -> std::vector<PlayerId> {
  std::vector<PlayerId> player_ids(_players.size());
  std::iota(player_ids.begin(), player_ids.end(), 0);
  return player_ids;
}

auto Game::current_player_id() const -> PlayerId { return _current_player_id; }

auto Game::player_score(const PlayerId &player_id) const -> double {
  const Player &player = player_for_id(player_id);
  return std::accumulate(player.colors.begin(), player.colors.end(), 0, [this](const auto score, const auto &kv_pair) {
    const auto &[color, color_count] = kv_pair;
    return score + color_values.at(color) * color_count;
  });
}

auto Game::available_actions() const -> std::vector<ActionId> {
  const Player &current_player = player_for_id(_current_player_id);
  const auto it = color_pairs.find(current_player.last_color_taken);
  if (it != color_pairs.end()) {
    return _available_colors.at(it->second) > 0
               ? std::vector{ActionId{_current_player_id, it->second, current_player.last_color_taken}}
               : std::vector{ActionId{_current_player_id, Color::none, current_player.last_color_taken}};
  } else {
    std::vector<ActionId> actions;
    actions.reserve(4);
    for (const auto color : {Color::red, Color::green, Color::blue, Color::yellow})
      if (current_player.last_color_taken != color && _available_colors.at(color) > 0)
        actions.push_back({_current_player_id, color, current_player.last_color_taken});

    if (actions.empty())
      actions.push_back({_current_player_id, Color::none, current_player.last_color_taken});

    return actions;
  }
}

auto Game::resource_ids() const -> std::vector<ResourceId> {
  return {Color::none, Color::red, Color::green, Color::blue, Color::yellow};
}

auto Game::resource_count(const ResourceId &resource_id) const -> size_t {
  return resource_id == Color::none ? color_counts_sum : color_counts.at(resource_id);
}

auto Game::resource_count_for_player(const ResourceId &resource_id, const PlayerId &player_id) const -> size_t {
  return player_for_id(player_id).colors.at(resource_id);
}

void Game::reset() {
  for (auto &player : _players)
    player.reset();
  _current_player_id = 0;
  _available_colors = color_counts;
}

void Game::perform_action(const ActionId &action_id, const PlayerId &player_id) {
  if (action_id.player != player_id)
    std::throw_with_nested(std::invalid_argument("player_id mismatch"));
  if (action_id.color_to_take != Color::none && _available_colors.at(action_id.color_to_take) < 1)
    std::throw_with_nested(std::invalid_argument("color unavailable"));
  Player &player = player_for_id(player_id);
  player.colors.at(action_id.color_to_take) += 1;
  player.last_color_taken = action_id.color_to_take;
  if (action_id.color_to_take != Color::none)
    _available_colors.at(action_id.color_to_take) -= 1;

  switch_to_next_player(player_id);
}

void Game::undo_action(const ActionId &action_id, const PlayerId &player_id) {
  if (action_id.player != player_id)
    std::throw_with_nested(std::invalid_argument("player_id mismatch"));
  Player &player = player_for_id(player_id);
  if (player.colors.at(action_id.color_to_take) < 1)
    std::throw_with_nested(std::invalid_argument("color unavailable"));
  player.colors.at(action_id.color_to_take) -= 1;
  player.last_color_taken = action_id.color_taken_before;
  if (action_id.color_to_take != Color::none)
    _available_colors.at(action_id.color_to_take) += 1;

  switch_to_prev_player(player_id);
}

auto Game::player_for_id(PlayerId player_id) -> Player & { return _players.at(player_id); }

auto Game::player_for_id(PlayerId player_id) const -> const Player & { return _players.at(player_id); }

void Game::switch_to_next_player(PlayerId player_id) { _current_player_id = (player_id + 1) % _players.size(); }

void Game::switch_to_prev_player(PlayerId player_id) { _current_player_id = (player_id - 1) % _players.size(); }

} // namespace colors
