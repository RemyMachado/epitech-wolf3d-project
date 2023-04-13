#include <iostream>
#include "GameManager.hpp"
#include "game_loop.hpp"

// TODO: idea -> start the game with no weapons,
//  and kill an officer to get a knife (use the knife sprite-sheet to create a fist sprite-sheet)
//  if too hard, give the player a knife at the start of the game

// retrieve filename from command line
int main(int argc, char **argv) {
  std::cout << "Hello, World!!!" << std::endl;

  GameManager game_manager = GameManager(argv[1], 8, {800, 800});

  // cout the grid
  std::cout << game_manager.grid << std::endl;

  run_game(game_manager);

  return EXIT_SUCCESS;
}
