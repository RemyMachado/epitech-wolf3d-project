#include <SFML/System/Vector2.hpp>
#include "Grid.hpp"
#include "lines.hpp"

#ifndef EPITECH_WOLF3D_PROJECT_RAYCASTING_HPP
#define EPITECH_WOLF3D_PROJECT_RAYCASTING_HPP

struct Cube {
  CubeValue value;
  sf::Vector2i pos;
  std::vector<Line> segments;
};

struct Raycast {
  float distance;
  sf::Vector2f intersection_pos;
  float local_intersection_x;
  Line intersected_segment;
  Cube cube;
};

std::vector<Cube> get_grid_cubes(Grid<int> &grid);
std::optional<Raycast> raycast(sf::Vector2f pos,
                               float direction_deg,
                               Grid<int> &grid,
                               std::vector<Cube> &grid_cubes, CubeValue cube_target);

#endif //EPITECH_WOLF3D_PROJECT_RAYCASTING_HPP
