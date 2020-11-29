#include <utility>

#include <bird_lady/bird.hpp>

namespace bird_lady {

Bird::Bird(BirdColor color, BirdFeeding feeding, BirdScoring scoring)
    : color(color), feeding(std::move(feeding)), scoring(scoring) {}

} // namespace bird_lady
