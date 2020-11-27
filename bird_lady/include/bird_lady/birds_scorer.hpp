#pragma once

#include <list>
#include <unordered_map>

#include <c++/z3++.h>

#include <bird_lady/bird.hpp>
#include <bird_lady/card_handle.hpp>

namespace bird_lady {

class Player;

struct BirdsScoreDetails {
  int total;
  int happy_blue_birds_count;
  int happy_green_birds_count;
  int happy_birds_count;
  int leftover_food_count;
};

class BirdsScorer {
public:
  BirdsScorer(const Player &player);
  auto score() -> BirdsScoreDetails;

private:
  enum Z3ConstName {
    zero = 0,
    one,
    no_food_type = 10,
    fruit_type,
    vegetable_type,
    tomato_type,
    fruit_available = 20,
    vegetable_available,
    tomato_available,
    number_of_toys = 30,
    number_of_aviaries,
    unhappy_bird_penalty = 40
  };
  struct Z3BirdData {
    CardHandle bird_id;
    std::unordered_map<BirdColor, z3::expr> z3_color_consts = {};
    std::list<z3::expr> z3_food_vars = {};
  };

  void register_bird(CardHandle card_handle, const Bird &bird);
  [[nodiscard]] auto z3_food_type(BirdFood food) const -> const z3::expr &;
  [[nodiscard]] auto z3_food_available(BirdFood food) const -> const z3::expr &;
  [[nodiscard]] auto z3_food_availability_requirement(BirdFood food) const -> z3::expr;
  [[nodiscard]] auto z3_bird_unhappiness(const Z3BirdData &bird_data) const -> z3::expr;
  [[nodiscard]] auto z3_bird_proper_food_count(const Z3BirdData &bird_data) const -> z3::expr;
  [[nodiscard]] auto z3_bird_score(const Z3BirdData &bird_data) -> z3::expr;
  [[nodiscard]] auto z3_total_score() -> z3::expr;
  [[nodiscard]] auto z3_number_of_happy_birds(BirdColor color) const -> z3::expr;

  z3::context _context;
  z3::optimize _optimizer;
  std::unordered_map<Z3ConstName, z3::expr> _z3_consts;
  std::list<Z3BirdData> _birds = {};
};

} // namespace bird_lady
