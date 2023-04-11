#ifndef EPITECH_WOLF3D_PROJECT_FILE_PARSER_HPP
#define EPITECH_WOLF3D_PROJECT_FILE_PARSER_HPP

#include <memory>
#include <vector>

class Tile;

struct FileData {
  int width;
  int height;
  std::vector<Tile> tiles;
};

FileData parse_file(char *filename, float tile_size);

#endif //EPITECH_WOLF3D_PROJECT_FILE_PARSER_HPP
