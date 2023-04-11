#include <fstream>
#include <string>
#include <iostream>
#include "file_parser.hpp"
#include "Tile.hpp"

/*
 * A function parsing a file and returning a Grid<char> object
 * A file like the following (rectangular grid)
 *
 * ######
 * # #  #
 * #   ##
 * ## # #
 * #  P #
 * ######
 * */
FileData parse_file(char *filename, float tile_size) {
  // check if filename is not null
  if (filename == nullptr) {
    std::cerr << "Error: filename is null" << std::endl;
    exit(EXIT_FAILURE);
  }

  std::ifstream file(filename);
  std::string line;
  std::vector<Tile> values;
  int width = 0;
  int height = 0;

  // handle file opening
  if (!file.is_open()) {
    std::cerr << "Error: could not open file \"" << filename << "\"" << std::endl;
    exit(EXIT_FAILURE);
  }

  while (std::getline(file, line)) {
    width = (int) line.size();
    int x = 0;
    for (char c : line) {
      if (!Tile::is_valid_symbol(c)) {
        std::cerr << "Error: invalid symbol \"" << c << "\" in file \"" << filename << "\"" << std::endl;
        exit(EXIT_FAILURE);
      }
      values.push_back(Tile((Tile::Symbol) c, {x, height}, tile_size));
      x++;
    }
    height++;
  }

  return {width, height, values};
}
