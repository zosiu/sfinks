#include <utility>

#include <bird_lady/bird.hpp>

namespace bird_lady {

Bird::Bird(std::set<BirdColor> colors, BirdFeeding feeding, BirdScoring scoring)
    : colors(std::move(colors)), feeding(std::move(feeding)), scoring(scoring) {}

} // namespace bird_lady
