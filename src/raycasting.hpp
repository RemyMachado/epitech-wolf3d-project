#include <SFML/System/Vector2.hpp>
#include "Grid.hpp"
#include "lines.hpp"

#ifndef EPITECH_WOLF3D_PROJECT_RAYCASTING_HPP
#define EPITECH_WOLF3D_PROJECT_RAYCASTING_HPP

typedef std::vector<Line> CubeRaycastSegments;

std::vector<CubeRaycastSegments> get_grid_raycast_segments(Grid<int> &grid);
std::optional<float> raycast(sf::Vector2f pos, float direction_deg, Grid<int> &grid);

#endif //EPITECH_WOLF3D_PROJECT_RAYCASTING_HPP
