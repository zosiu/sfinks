#include <cereal/archives/json.hpp>
#include <cereal/types/string.hpp>
#include <iostream>

#include <sfinks/engine.hpp>

namespace sfinks {

void Engine::say_hello() {
  {
    cereal::JSONOutputArchive archive(std::cout);
    archive(cereal::make_nvp("hello", std::string("from the sfinks engine")));
  }

  std::cout << std::endl;
}

} // namespace sfinks
