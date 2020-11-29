#pragma once

#include <cereal/cereal.hpp>

namespace bird_lady {

enum class CardHandle {
  none,

  fruit,
  vegetable,
  tomato,

  aviary,
  egg,

  bird_01,
  bird_02,
  bird_03,
  bird_04,
  bird_05,

  mystery_bird_01,
  mystery_bird_02,
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
  case bird_lady::CardHandle::mystery_bird_01:
    return "mystery bird 01";
  case bird_lady::CardHandle::mystery_bird_02:
    return "mystery bird 02";
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
