#include <iostream>
#include "managers/GameManager.hpp"
#include "game_loop.hpp"

// TODO: create doors with wood texture and destroy it with any weapon

// retrieve filename from command line
int main(int argc, char **argv) {
  GameManager game_manager = GameManager(argv[1], 200, {960, 600});

  run_game(game_manager);

  return EXIT_SUCCESS;
}
