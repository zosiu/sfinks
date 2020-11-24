#include <iostream>

#include <sfinks/engine.hpp>

#include <tic_tac_toe/game.hpp>

namespace tic_tac_toe {

void Game::say_hello() {
  sfinks::Engine::say_hello();
  std::cout << "Hello from tic tac toe!" << std::endl;
}

} // namespace tic_tac_toe
