#pragma once

#include <cereal/cereal.hpp>

namespace bird_lady {

enum class CardHandle {
  none,

  fruit,
  vegetable,
  tomato,

  toy,
  aviary,
  egg,

  bird_01,
  bird_02,
  bird_03,
  bird_04,
  bird_05,
  bird_06,
  bird_07,
  bird_08,
  bird_09,
  bird_10,
  bird_11,
  bird_12,
  bird_13,

  mystery_bird_01,
  mystery_bird_02,
  mystery_bird_03
};

template <class Archive>
inline auto save_minimal(const Archive & /*archive*/, CardHandle handle) -> std::string {
  switch (handle) {
  case bird_lady::CardHandle::none:
    return "nothing";
  case bird_lady::CardHandle::fruit:
    return "fruit";
  case bird_lady::CardHandle::vegetable:
    return "vegetable";
  case bird_lady::CardHandle::tomato:
    return "tomato";
  case bird_lady::CardHandle::toy:
    return "toy";
  case bird_lady::CardHandle::aviary:
    return "aviary";
  case bird_lady::CardHandle::egg:
    return "egg";
  case bird_lady::CardHandle::bird_01:
    return "bird 01";
  case bird_lady::CardHandle::bird_02:
    return "bird 02";
  case bird_lady::CardHandle::bird_03:
    return "bird 03";
  case bird_lady::CardHandle::bird_04:
    return "bird 04";
  case bird_lady::CardHandle::bird_05:
    return "bird 05";
  case bird_lady::CardHandle::bird_06:
    return "bird 06";
  case bird_lady::CardHandle::bird_07:
    return "bird 07";
  case bird_lady::CardHandle::bird_08:
    return "bird 08";
  case bird_lady::CardHandle::bird_09:
    return "bird 09";
  case bird_lady::CardHandle::bird_10:
    return "bird 10";
  case bird_lady::CardHandle::bird_11:
    return "bird 11";
  case bird_lady::CardHandle::bird_12:
    return "bird 12";
  case bird_lady::CardHandle::bird_13:
    return "bird 13";
  case bird_lady::CardHandle::mystery_bird_01:
    return "mystery bird 01";
  case bird_lady::CardHandle::mystery_bird_02:
    return "mystery bird 02";
  case bird_lady::CardHandle::mystery_bird_03:
    return "mystery bird 03";
  default:
    return "unknown handle";
  }
}

template <class Archive>
inline void load_minimal(const Archive & /*archive*/, CardHandle &handle, const std::string & /*value*/) {
  // TODO
  handle = CardHandle::none;
}

} // namespace bird_lady
