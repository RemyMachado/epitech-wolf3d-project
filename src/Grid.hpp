#ifndef EPITECH_WOLF3D_PROJECT_SRC_GRID_HPP_
#define EPITECH_WOLF3D_PROJECT_SRC_GRID_HPP_

#include <vector>
#include <iostream>
#include "Tile.hpp"
#include "file_parser.hpp"

class Grid {
 public:
  const int width;
  const int height;
  const float tile_size;
  std::vector<Tile> tiles;

  // constructor for FileData type
  Grid(FileData file_data, float tile_size)
      : width(file_data.width),
        height(file_data.height),
        tiles(std::move(file_data.tiles)), tile_size(tile_size) {
  };

  [[nodiscard]] Tile get(int x, int y) const {
    return tiles[y * width + x];
  }

  // function to get the initial position of the player
  [[nodiscard]] sf::Vector2f get_player_initial_pos() const {
    // find the player position in the grid
    for (int y = 0; y < height; y++) {
      for (int x = 0; x < width; x++) {
        if (get(x, y).symbol == Tile::Symbol::PLAYER) {
          return {(float) x * tile_size + tile_size / 2, (float) y * tile_size + tile_size / 2};
        }
      }
    }

    // if the player position is not found, throw an invalid file exception
    throw std::invalid_argument(
        "The grid file is invalid: the player position was not found \'" + std::to_string(Tile::Symbol::PLAYER) + "\'");
  }

  // stringify the grid with the cout operator
  friend std::ostream &operator<<(std::ostream &os, const Grid &grid) {
    for (int y = 0; y < grid.height; y++) {
      for (int x = 0; x < grid.width; x++) {
        os << grid.get(x, y).symbol;
      }
      os << std::endl;
    }
    return os;
  }
};

#endif //EPITECH_WOLF3D_PROJECT_SRC_GRID_HPP_
