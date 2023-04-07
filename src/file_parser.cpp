#include <fstream>
#include <string>
#include <iostream>
#include "file_parser.hpp"

/*
 * A function parsing a file and returning a Grid<int> object
 * A file like the following (rectangular grid)
 *
 * 111111
 * 101001
 * 100011
 * 110101
 * 100001
 * 111111
 * */
Grid<int> parse_file(char *&filename) {
  // check if filename is not null
  if (filename == nullptr) {
    std::cerr << "Error: filename is null" << std::endl;
    exit(EXIT_FAILURE);
  }

  std::ifstream file(filename);
  std::string line;
  std::vector<int> values;
  int width = 0;
  int height = 0;

  // handle file opening
  if (!file.is_open()) {
    std::cerr << "Error: could not open file \"" << filename << "\"" << std::endl;
    exit(EXIT_FAILURE);
  }

  while (std::getline(file, line)) {
    width = line.size();
    for (char c : line) {
      values.push_back(c - '0');
    }
    height++;
  }

  return {width, height, values};
}
