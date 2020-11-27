#pragma once

#include <unordered_map>

#include <bird_lady/bird.hpp>
#include <bird_lady/card_handle.hpp>

namespace bird_lady::consts {

constexpr int unhappy_bird_penalty = -2;

constexpr size_t board_size = 3;
constexpr int maximum_consumable_food = 4;

static const std::unordered_map<CardHandle, Bird> birds = {
    {CardHandle::bird_01,
     Bird({BirdColor::blue}, {BirdHappiness::fully_fed, {{BirdFood::fruit, 1}}}, {BirdScoringType::simple, 2})},
    {CardHandle::bird_02,
     Bird({BirdColor::blue}, {BirdHappiness::fully_fed, {{BirdFood::vegetable, 1}}}, {BirdScoringType::simple, 1})},
    {CardHandle::bird_03,
     Bird({BirdColor::green}, {BirdHappiness::fully_fed, {{BirdFood::fruit, 1}}}, {BirdScoringType::simple, 2})},
    {CardHandle::bird_04,
     Bird({BirdColor::green}, {BirdHappiness::fully_fed, {{BirdFood::vegetable, 1}}}, {BirdScoringType::simple, 1})},

    {CardHandle::mystery_bird_01,
     Bird({BirdColor::blue},
          {BirdHappiness::fed_at_least_one_food,
           {{BirdFood::fruit, maximum_consumable_food}, {BirdFood::vegetable, maximum_consumable_food}}},
          {BirdScoringType::for_every_food_fed, 2})}};

static const std::unordered_map<size_t, std::unordered_map<CardHandle, size_t>> card_counts_by_player_number = {
    {2,
     {{CardHandle::fruit, 7},
      {CardHandle::vegetable, 9},
      {CardHandle::tomato, 2},
      {CardHandle::toy, 7},
      {CardHandle::aviary, 5},
      {CardHandle::egg, 8},
      {CardHandle::bird_01, 2},
      {CardHandle::bird_02, 1},
      {CardHandle::bird_03, 2},
      {CardHandle::bird_04, 1}}},
    {3,
     {{CardHandle::fruit, 9},
      {CardHandle::vegetable, 11},
      {CardHandle::tomato, 2},
      {CardHandle::toy, 8},
      {CardHandle::aviary, 6},
      {CardHandle::egg, 10}}},
    {4,
     {{CardHandle::fruit, 12},
      {CardHandle::vegetable, 14},
      {CardHandle::tomato, 3},
      {CardHandle::toy, 9},
      {CardHandle::aviary, 7},
      {CardHandle::egg, 10}}}};

} // namespace bird_lady::consts
