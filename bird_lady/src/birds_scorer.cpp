// #include <iostream>
#include <numeric>
#include <stdexcept>
#include <string>

#include <bird_lady/consts.hpp>
#include <bird_lady/player.hpp>

#include <bird_lady/birds_scorer.hpp>

namespace bird_lady {

BirdsScorer::BirdsScorer(const Player &player)
    : _optimizer(_context),
      _z3_consts({
          {zero, _context.int_val(0)},
          {one, _context.int_val(1)},
          {no_food_type, _context.int_val(no_food_type)},
          {fruit_type, _context.int_val(fruit_type)},
          {vegetable_type, _context.int_val(vegetable_type)},
          {tomato_type, _context.int_val(tomato_type)},
          {fruit_available, _context.int_val(player.number_of_available_cards(CardHandle::fruit))},
          {vegetable_available, _context.int_val(player.number_of_available_cards(CardHandle::vegetable))},
          {tomato_available, _context.int_val(player.number_of_available_cards(CardHandle::tomato))},
          {number_of_toys, _context.int_val(player.number_of_available_cards(CardHandle::toy))},
          {number_of_aviaries, _context.int_val(player.number_of_available_cards(CardHandle::aviary))},
          {unhappy_bird_penalty, _context.int_val(consts::unhappy_bird_penalty)},
          {leftover_food_penalty_per_pair, _context.int_val(consts::leftover_food_penalty_per_pair)},
      }) {
  for (const auto &[card_handle, bird] : consts::birds)
    for (int i = 0; i < player.number_of_available_cards(card_handle); i++)
      register_bird(card_handle, bird);
}

auto BirdsScorer::score() -> BirdsScoreDetails {
  for (const auto food : {BirdFood::fruit, BirdFood::vegetable, BirdFood::tomato})
    _optimizer.add(z3_food_availability_requirement(food));

  const auto total =
      z3_birds_score() + z3_leftover_food() / _context.int_val(2) * _z3_consts.at(leftover_food_penalty_per_pair);
  _optimizer.maximize(total);
  _optimizer.check();

  z3::model m = _optimizer.get_model();

  // ---
  // for (const auto &bird_data : _birds) {
  // std::cout << (int)bird_data.bird_id << std::endl;
  // std::cout << "score: " << m.eval(z3_bird_score(bird_data)).get_numeral_int() << std::endl;
  // for (const auto &food_var : bird_data.z3_food_vars)
  // std::cout << "\t" << m.eval(food_var).get_numeral_int() << std::endl;
  // }
  // std::cout << "---" << std::endl;
  // std::cout << "---" << std::endl;
  // std::cout << _optimizer << std::endl;
  // std::cout << "---" << std::endl;
  // ---

  int score = m.eval(total).get_numeral_int();
  int happy_birds = m.eval(z3_number_of_happy_birds()).get_numeral_int();
  int leftover_food = m.eval(z3_leftover_food()).get_numeral_int();

  return {score, happy_birds, leftover_food};
}

void BirdsScorer::register_bird(CardHandle card_handle, const Bird &bird) {
  Z3BirdData bird_data{card_handle};
  const size_t bird_id = _birds.size();

  for (const auto color : {BirdColor::blue, BirdColor::green})
    bird_data.z3_color_consts.emplace(color, _context.int_val(bird.colors.find(color) == bird.colors.end() ? 0 : 1));

  for (const auto &[food, quantity] : bird.feeding.food_requirements) {
    for (size_t i = 0; i < quantity; i++) {
      std::string food_var_name =
          "bird_" + std::to_string(bird_id) + "_food_" + std::to_string(bird_data.z3_food_vars.size());
      auto it = bird_data.z3_food_vars.emplace(bird_data.z3_food_vars.end(), _context.int_const(food_var_name.c_str()));
      _optimizer.add((*it == z3_food_type(food)) || (*it == z3_food_type(BirdFood::tomato)) ||
                     (*it == z3_food_type(BirdFood::none)));
    }
  }

  _birds.emplace_back(bird_data);
}

auto BirdsScorer::z3_food_type(BirdFood food) const -> const z3::expr & {
  switch (food) {
  case BirdFood::none:
    return _z3_consts.at(no_food_type);
  case BirdFood::fruit:
    return _z3_consts.at(fruit_type);
  case BirdFood::vegetable:
    return _z3_consts.at(vegetable_type);
  case BirdFood::tomato:
    return _z3_consts.at(tomato_type);
  default:
    std::throw_with_nested(std::invalid_argument("unknown food"));
  }
}

auto BirdsScorer::z3_food_available(BirdFood food) const -> const z3::expr & {
  switch (food) {
  case BirdFood::fruit:
    return _z3_consts.at(fruit_available);
  case BirdFood::vegetable:
    return _z3_consts.at(vegetable_available);
  case BirdFood::tomato:
    return _z3_consts.at(tomato_available);
  default:
    std::throw_with_nested(std::invalid_argument("unknown food requirement"));
  }
}

auto BirdsScorer::z3_food_availability_requirement(BirdFood food) const -> z3::expr {
  auto food_used = std::accumulate(
      _birds.begin(), _birds.end(), _z3_consts.at(zero), [food, this](const auto &exp, const auto &bird_data) {
        return std::accumulate(
                   bird_data.z3_food_vars.begin(), bird_data.z3_food_vars.end(), _z3_consts.at(zero),
                   [food, this](const auto &exp, const auto &food_var) {
                     return z3::ite(food_var == z3_food_type(food), _z3_consts.at(one), _z3_consts.at(zero)) + exp;
                   }) +
               exp;
      });

  return food_used <= z3_food_available(food);
}

auto BirdsScorer::z3_leftover_food() const -> z3::expr {
  auto food_used = std::accumulate(_birds.begin(), _birds.end(), _z3_consts.at(zero),
                                   [this](const auto &exp, const auto &bird_data) {
                                     return z3::ite(z3_bird_unhappiness(bird_data) == _z3_consts.at(zero),
                                                    z3_bird_proper_food_count(bird_data), _z3_consts.at(zero)) +
                                            exp;
                                   });
  return _z3_consts.at(fruit_available) + _z3_consts.at(vegetable_available) + _z3_consts.at(tomato_available) -
         food_used;
}

auto BirdsScorer::z3_bird_unhappiness(const Z3BirdData &bird_data) const -> z3::expr {
  const Bird &bird = consts::birds.at(bird_data.bird_id);
  switch (bird.feeding.happy_when) {
  case BirdHappiness::fully_fed:
    return std::accumulate(
        bird_data.z3_food_vars.begin(), bird_data.z3_food_vars.end(), _z3_consts.at(zero),
        [this](const auto &exp, const auto &z3_food_var) {
          return z3::ite(z3_food_var == z3_food_type(BirdFood::none), _z3_consts.at(one), _z3_consts.at(zero)) + exp;
        });
  case BirdHappiness::fed_at_least_one_food:
    return z3::ite(z3_bird_proper_food_count(bird_data) > _z3_consts.at(zero), _z3_consts.at(zero), _z3_consts.at(one));
  default:
    std::throw_with_nested(std::invalid_argument("unknown happiness criteria"));
  }
}

auto BirdsScorer::z3_bird_proper_food_count(const Z3BirdData &bird_data) const -> z3::expr {
  return std::accumulate(
      bird_data.z3_food_vars.begin(), bird_data.z3_food_vars.end(), _z3_consts.at(zero),
      [this](const auto &exp, const auto &z3_food_var) {
        return z3::ite(z3_food_var != z3_food_type(BirdFood::none), _z3_consts.at(one), _z3_consts.at(zero)) + exp;
      });
}

auto BirdsScorer::z3_bird_score(const Z3BirdData &bird_data) -> z3::expr {
  const Bird &bird = consts::birds.at(bird_data.bird_id);
  z3::expr scoring_expr(_context);
  switch (bird.scoring.type) {
  case BirdScoringType::simple:
    scoring_expr = _context.int_val(1);
    break;
  case BirdScoringType::for_every_green:
    scoring_expr = z3_number_of_happy_birds(BirdColor::green);
    break;
  case BirdScoringType::for_every_blue:
    scoring_expr = z3_number_of_happy_birds(BirdColor::blue);
    break;
  case BirdScoringType::for_every_aviary:
    scoring_expr = _z3_consts.at(number_of_aviaries);
    break;
  case BirdScoringType::for_every_toy:
    scoring_expr = _z3_consts.at(number_of_toys);
    break;
  case BirdScoringType::for_every_food_fed:
    scoring_expr = z3_bird_proper_food_count(bird_data);
    break;
  default:
    std::throw_with_nested(std::invalid_argument("unknown scoring type"));
  }

  return z3::ite(z3_bird_unhappiness(bird_data) == _z3_consts.at(zero),
                 scoring_expr * _context.int_val(bird.scoring.multiplier), _z3_consts.at(unhappy_bird_penalty));
}

auto BirdsScorer::z3_birds_score() -> z3::expr {
  return std::accumulate(_birds.begin(), _birds.end(), _z3_consts.at(zero),
                         [this](const auto &exp, const auto &bird_data) { return z3_bird_score(bird_data) + exp; });
}

auto BirdsScorer::z3_number_of_happy_birds(BirdColor color) const -> z3::expr {
  return std::accumulate(_birds.begin(), _birds.end(), _z3_consts.at(zero),
                         [color, this](const auto &exp, const auto &bird_data) {
                           return z3::ite(z3_bird_unhappiness(bird_data) == _z3_consts.at(zero) &&
                                              bird_data.z3_color_consts.at(color) == _z3_consts.at(one),
                                          _z3_consts.at(one), _z3_consts.at(zero)) +
                                  exp;
                         });
}

auto BirdsScorer::z3_number_of_happy_birds() const -> z3::expr {
  return std::accumulate(
      _birds.begin(), _birds.end(), _z3_consts.at(zero), [this](const auto &exp, const auto &bird_data) {
        return z3::ite(z3_bird_unhappiness(bird_data) == _z3_consts.at(zero), _z3_consts.at(one), _z3_consts.at(zero)) +
               exp;
      });
}

} // namespace bird_lady
