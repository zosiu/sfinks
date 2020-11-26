#pragma once

#include <unordered_map>

#include <bird_lady/card_handle.hpp>

namespace bird_lady {

static const std::unordered_map<size_t, std::unordered_map<CardHandle, size_t>> card_counts_by_player_number = {
    {2,
     {{CardHandle::fruit, 3},
      {CardHandle::vegetable, 3},
      {CardHandle::tomato, 3},
      {CardHandle::toy, 3},
      {CardHandle::aviary, 3},
      {CardHandle::egg, 3}}},
    {3,
     {{CardHandle::fruit, 3},
      {CardHandle::vegetable, 3},
      {CardHandle::tomato, 3},
      {CardHandle::toy, 3},
      {CardHandle::aviary, 3},
      {CardHandle::egg, 3}}},
    {4,
     {{CardHandle::fruit, 3},
      {CardHandle::vegetable, 3},
      {CardHandle::tomato, 3},
      {CardHandle::toy, 3},
      {CardHandle::aviary, 3},
      {CardHandle::egg, 3}}}};

} // namespace bird_lady
