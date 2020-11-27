#pragma once

#include <array>
#include <unordered_map>

#include <bird_lady/bird.hpp>
#include <bird_lady/card_handle.hpp>

namespace bird_lady::consts {

constexpr size_t board_size = 3;

constexpr int unhappy_bird_penalty = -2;
constexpr int leftover_food_penalty_per_pair = -1;
constexpr int no_toys_penalty = -2;
constexpr int most_toys_reward = 6;
constexpr int single_aviary_penalty = -2;
constexpr int egg_reward_per_pair = 2;

constexpr int maximum_consumable_food = 4;

constexpr int eggs_needed_to_hatch_mystery_bird = 2;

static const std::unordered_map<CardHandle, Bird> birds = {
    {CardHandle::bird_01,
     Bird({BirdColor::blue}, {BirdHappiness::fully_fed, {{BirdFood::fruit, 1}}}, {BirdScoringType::simple, 2})},
    {CardHandle::bird_02,
     Bird({BirdColor::blue}, {BirdHappiness::fully_fed, {{BirdFood::vegetable, 1}}}, {BirdScoringType::simple, 1})},
    {CardHandle::bird_03,
     Bird({BirdColor::green}, {BirdHappiness::fully_fed, {{BirdFood::fruit, 1}}}, {BirdScoringType::simple, 2})},
    {CardHandle::bird_04,
     Bird({BirdColor::green}, {BirdHappiness::fully_fed, {{BirdFood::vegetable, 1}}}, {BirdScoringType::simple, 1})},
    {CardHandle::bird_05,
     Bird({BirdColor::blue}, {BirdHappiness::fully_fed, {{BirdFood::fruit, 2}}}, {BirdScoringType::simple, 4})},
    {CardHandle::bird_06,
     Bird({BirdColor::blue}, {BirdHappiness::fully_fed, {{BirdFood::vegetable, 2}}}, {BirdScoringType::simple, 3})},
    {CardHandle::bird_07,
     Bird({BirdColor::green}, {BirdHappiness::fully_fed, {{BirdFood::fruit, 2}}}, {BirdScoringType::simple, 4})},
    {CardHandle::bird_08,
     Bird({BirdColor::green}, {BirdHappiness::fully_fed, {{BirdFood::vegetable, 2}}}, {BirdScoringType::simple, 3})},
    {CardHandle::bird_09,
     Bird({BirdColor::green}, {BirdHappiness::fully_fed, {{BirdFood::fruit, 2}, {BirdFood::vegetable, 1}}},
          {BirdScoringType::simple, 6})},
    {CardHandle::bird_10,
     Bird({BirdColor::blue}, {BirdHappiness::fully_fed, {{BirdFood::fruit, 1}, {BirdFood::vegetable, 2}}},
          {BirdScoringType::simple, 6})},

    {CardHandle::bird_11,
     Bird({BirdColor::blue}, {BirdHappiness::fully_fed, {{BirdFood::fruit, 1}, {BirdFood::vegetable, 1}}},
          {BirdScoringType::for_every_blue, 2})},
    {CardHandle::bird_12,
     Bird({BirdColor::green}, {BirdHappiness::fully_fed, {{BirdFood::fruit, 1}, {BirdFood::vegetable, 2}}},
          {BirdScoringType::for_every_green, 2})},

    {CardHandle::bird_13, Bird({BirdColor::blue, BirdColor::green}, {BirdHappiness::fully_fed, {{BirdFood::tomato, 1}}},
                               {BirdScoringType::simple, 5})},

    {CardHandle::mystery_bird_01,
     Bird({BirdColor::blue},
          {BirdHappiness::fed_at_least_one_food,
           {{BirdFood::fruit, maximum_consumable_food}, {BirdFood::vegetable, maximum_consumable_food}}},
          {BirdScoringType::for_every_food_fed, 2})},
    {CardHandle::mystery_bird_02,
     Bird({BirdColor::green}, {BirdHappiness::fully_fed, {{BirdFood::fruit, 1}}}, {BirdScoringType::for_every_toy, 1})},
    {CardHandle::mystery_bird_03, Bird({BirdColor::blue}, {BirdHappiness::fully_fed, {{BirdFood::vegetable, 2}}},
                                       {BirdScoringType::for_every_aviary, 2})},
};

static const std::array<CardHandle, 3> mystery_birds = {CardHandle::mystery_bird_01, CardHandle::mystery_bird_02,
                                                        CardHandle::mystery_bird_03};

static const std::unordered_map<size_t, std::unordered_map<CardHandle, size_t>> card_counts_by_player_number = {
    {2,
     {
         {CardHandle::fruit, 7},   {CardHandle::vegetable, 9}, {CardHandle::tomato, 2},

         {CardHandle::toy, 7},     {CardHandle::aviary, 5},    {CardHandle::egg, 8},

         {CardHandle::bird_01, 1}, {CardHandle::bird_02, 0},   {CardHandle::bird_03, 0}, {CardHandle::bird_04, 1},
         {CardHandle::bird_05, 1}, {CardHandle::bird_06, 0},   {CardHandle::bird_07, 0}, {CardHandle::bird_08, 1},
         {CardHandle::bird_09, 1}, {CardHandle::bird_10, 1},

         {CardHandle::bird_11, 0}, {CardHandle::bird_12, 0},

         {CardHandle::bird_13, 0},
     }},
    {3,
     {
         {CardHandle::fruit, 9},   {CardHandle::vegetable, 11}, {CardHandle::tomato, 2},

         {CardHandle::toy, 8},     {CardHandle::aviary, 6},     {CardHandle::egg, 10},

         {CardHandle::bird_01, 1}, {CardHandle::bird_02, 1},    {CardHandle::bird_03, 1}, {CardHandle::bird_04, 1},
         {CardHandle::bird_05, 1}, {CardHandle::bird_06, 0},    {CardHandle::bird_07, 0}, {CardHandle::bird_08, 1},
         {CardHandle::bird_09, 1}, {CardHandle::bird_10, 1},

         {CardHandle::bird_11, 1}, {CardHandle::bird_12, 1},

         {CardHandle::bird_13, 0},
     }},
    {4,
     {
         {CardHandle::fruit, 12},  {CardHandle::vegetable, 14}, {CardHandle::tomato, 3},

         {CardHandle::toy, 9},     {CardHandle::aviary, 7},     {CardHandle::egg, 10},

         {CardHandle::bird_01, 1}, {CardHandle::bird_02, 1},    {CardHandle::bird_03, 1}, {CardHandle::bird_04, 1},
         {CardHandle::bird_05, 1}, {CardHandle::bird_06, 1},    {CardHandle::bird_07, 1}, {CardHandle::bird_08, 1},
         {CardHandle::bird_09, 1}, {CardHandle::bird_10, 1},

         {CardHandle::bird_11, 1}, {CardHandle::bird_12, 1},

         {CardHandle::bird_13, 1},
     }}};

} // namespace bird_lady::consts
