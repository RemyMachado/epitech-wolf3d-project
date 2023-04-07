#include <iostream>
#include "game_loop.hpp"
#include "lines.hpp"
#include "Grid.hpp"
#include "file_parser.hpp"

// retrieve filename from command line
int main(int argc, char **argv) {
  std::cout << "Hello, World!!!" << std::endl;

  Grid<int> grid = parse_file(argv[1]);

  // cout the grid
  std::cout << grid << std::endl;

  run_game(grid);

  return EXIT_SUCCESS;
}
