#ifndef EPITECH_WOLF3D_PROJECT_SRC_GRID_HPP_
#define EPITECH_WOLF3D_PROJECT_SRC_GRID_HPP_

#include <vector>
#include <iostream>
#include "Tile.hpp"
#include "file_parser.hpp"
#include "Enemy.hpp"

class Grid {
 public:
  const int width;
  const int height;
  const float tile_size;
  std::vector<Tile> tiles;

  // constructor for FileData type
  Grid(FileData file_data, float tile_size);;

  [[nodiscard]] Tile get(int x, int y) const;

  // function to get the initial position of the player
  [[nodiscard]] sf::Vector2f get_player_initial_pos() const;

  [[nodiscard]] std::vector<Enemy> get_initial_enemies() const;

  // stringify the grid with the cout operator (not working anymore)
//  friend std::ostream &operator<<(std::ostream &os, const Grid &grid);
};

#endif //EPITECH_WOLF3D_PROJECT_SRC_GRID_HPP_
