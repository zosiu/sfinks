#pragma once

#include <set>
#include <unordered_map>

namespace bird_lady {

enum class BirdColor { blue, green };
enum class BirdFood { none, fruit, vegetable, tomato };
enum class BirdScoringType {
  simple,
  for_every_green,
  for_every_blue,
  for_every_aviary,
  for_every_toy,
  for_every_food_fed
};
enum class BirdHappiness { fully_fed, fed_at_least_one_food };

struct BirdFeeding {
  BirdHappiness happy_when;
  std::unordered_map<BirdFood, size_t> food_requirements = {};
};

struct BirdScoring {
  BirdScoringType type;
  int multiplier;
};

struct Bird {
  Bird(std::set<BirdColor> colors, BirdFeeding feeding, BirdScoring scoring);

  std::set<BirdColor> colors;
  BirdFeeding feeding;
  BirdScoring scoring;
};

} // namespace bird_lady
