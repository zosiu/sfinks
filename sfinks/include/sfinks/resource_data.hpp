#pragma once

#include <cereal/cereal.hpp>

namespace sfinks {

struct ResourceData {
  int owned_by_player;
  bool more_available;

  template <class Archive>
  void serialize(Archive &archive) {
    archive(CEREAL_NVP(owned_by_player), CEREAL_NVP(more_available));
  }
};

} // namespace sfinks
