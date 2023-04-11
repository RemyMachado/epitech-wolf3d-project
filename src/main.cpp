#include <iostream>
#include "GameManager.hpp"
#include "game_loop.hpp"

// retrieve filename from command line
int main(int argc, char **argv) {
  std::cout << "Hello, World!!!" << std::endl;

  GameManager game_manager = GameManager(argv[1], 10, {1024, 1024});

  // cout the grid
  std::cout << game_manager.grid << std::endl;

  run_game(game_manager);

  return EXIT_SUCCESS;
}
