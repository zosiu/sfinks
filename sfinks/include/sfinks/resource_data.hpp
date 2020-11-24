#pragma once

#include <cereal/cereal.hpp>

namespace sfinks {

struct ResourceData {
  int owned_by_player = 0;
  int available = 0;

  template <class Archive>
  void serialize(Archive &archive) {
    archive(CEREAL_NVP(owned_by_player), CEREAL_NVP(available));
  }
};

} // namespace sfinks
