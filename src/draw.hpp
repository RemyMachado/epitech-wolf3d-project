#ifndef EPITECH_WOLF3D_PROJECT_SRC_DRAW_HPP_
#define EPITECH_WOLF3D_PROJECT_SRC_DRAW_HPP_

#include <SFML/Graphics.hpp>
#include "Grid.hpp"
#include "raycasting.hpp"

void draw_minimap(sf::RenderWindow &window, Grid<int> &grid, std::vector<CubeRaycastSegments> &grid_raycast_segments);
void draw_walls_3d(sf::RenderWindow &window, Grid<int> &grid, std::vector<CubeRaycastSegments> &grid_raycast_segments);

#endif //EPITECH_WOLF3D_PROJECT_SRC_DRAW_HPP_
