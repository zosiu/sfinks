#pragma once

#include <string>

namespace sfinks {

template <typename ActionId>
struct ActionResult {
  ActionId action_id;
  std::string state;
  double score = 0;
};

} // namespace sfinks
