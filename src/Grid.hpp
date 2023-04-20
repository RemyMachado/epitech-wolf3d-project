#ifndef EPITECH_WOLF3D_PROJECT_SRC_GRID_HPP_
#define EPITECH_WOLF3D_PROJECT_SRC_GRID_HPP_

#include <vector>
#include <SFML/System/Vector2.hpp>
#include <iostream>
#include "Tile.hpp"
#include "file_parser.hpp"
#include "Enemy.hpp"
#include "Pickup.hpp"

class Grid {
 public:
  const int width;
  const int height;
  const float tile_size;
  std::vector<Tile> tiles;

 public:
  // constructor for FileData type
  Grid(FileData file_data, float tile_size);

 public:
  [[nodiscard]] Tile get(int x, int y) const;
  sf::Vector2i pos_to_coords(sf::Vector2f pos) const;
  sf::Vector2f get_tile_center(sf::Vector2i coords) const;
  // function to get the initial position of the player
  [[nodiscard]] sf::Vector2f get_player_initial_pos() const;
  [[nodiscard]] std::vector<Enemy> get_initial_enemies() const;
  [[nodiscard]] std::vector<Pickup> get_initial_pickups() const;
  void set_tile_symbol(sf::Vector2i coords, Tile::Symbol new_symbol);

  // stringify the grid with the cout operator (not working anymore)
//  friend std::ostream &operator<<(std::ostream &os, const Grid &grid);

 private:
  [[nodiscard]] unsigned int count_enemies() const;
  /* adds 1 for the final pickup key (appearing at the player initial_position) */
  [[nodiscard]] unsigned int count_pickups() const;
};

#endif //EPITECH_WOLF3D_PROJECT_SRC_GRID_HPP_
