#pragma once

#include <array>
#include <unordered_map>

#include <bird_lady/bird.hpp>
#include <bird_lady/card_handle.hpp>

namespace bird_lady::consts {

constexpr int unhappy_bird_penalty = -1;
constexpr int leftover_food_penalty_per_pair = -1;
constexpr int no_aviary_penalty = -1;
constexpr int egg_reward_per_pair = 3;

constexpr int eggs_needed_to_hatch_mystery_bird = 2;

constexpr int maximum_consumable_food = 4;

static const std::unordered_map<CardHandle, Bird> birds = {
    {CardHandle::bird_01,
     Bird(BirdColor::blue, {BirdHappiness::fully_fed, {{BirdFood::fruit, 1}}}, {BirdScoringType::simple, 2})},
    {CardHandle::bird_02,
     Bird(BirdColor::green, {BirdHappiness::fully_fed, {{BirdFood::vegetable, 2}}}, {BirdScoringType::simple, 3})},
    {CardHandle::bird_03,
     Bird(BirdColor::green, {BirdHappiness::fully_fed, {{BirdFood::fruit, 1}, {BirdFood::vegetable, 1}}},
          {BirdScoringType::simple, 4})},
    {CardHandle::bird_04,
     Bird(BirdColor::blue, {BirdHappiness::fully_fed, {{BirdFood::fruit, 1}, {BirdFood::vegetable, 1}}},
          {BirdScoringType::for_every_blue, 3})},
    {CardHandle::bird_05,
     Bird(BirdColor::green, {BirdHappiness::fully_fed, {{BirdFood::tomato, 1}}}, {BirdScoringType::simple, 6})},

    {CardHandle::mystery_bird_01,
     Bird(BirdColor::blue,
          {BirdHappiness::fed_at_least_one_food,
           {{BirdFood::fruit, maximum_consumable_food}, {BirdFood::vegetable, maximum_consumable_food}}},
          {BirdScoringType::for_every_food_fed, 2})},
    {CardHandle::mystery_bird_02, Bird(BirdColor::blue, {BirdHappiness::fully_fed, {{BirdFood::vegetable, 2}}},
                                       {BirdScoringType::for_every_aviary, 3})},
};

static const std::array<CardHandle, 2> mystery_birds = {CardHandle::mystery_bird_01, CardHandle::mystery_bird_02};

static const std::unordered_map<CardHandle, size_t> card_counts = {{{CardHandle::fruit, 5},
                                                                    {CardHandle::vegetable, 6},
                                                                    {CardHandle::tomato, 2},

                                                                    {CardHandle::aviary, 4},
                                                                    {CardHandle::egg, 4},

                                                                    {CardHandle::bird_01, 2},
                                                                    {CardHandle::bird_02, 2},
                                                                    {CardHandle::bird_03, 1},
                                                                    {CardHandle::bird_04, 1},
                                                                    {CardHandle::bird_05, 1}}};

} // namespace bird_lady::consts
