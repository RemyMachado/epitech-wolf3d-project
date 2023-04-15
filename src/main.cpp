#include <iostream>
#include "managers/GameManager.hpp"
#include "game_loop.hpp"

// TODO: create doors with wood texture and destroy it with any weapon
// TODO: add shader to the wall texture depending on hit_side (create a darker texture in Gimp)

// retrieve filename from command line
int main(int argc, char **argv) {
  std::cout << "Hello, World!!!" << std::endl;

  GameManager game_manager = GameManager(argv[1], 200, {800, 800});

  // cout the grid
  std::cout << game_manager.grid << std::endl;

  run_game(game_manager);

  return EXIT_SUCCESS;
}
