#include <SFML/System/Vector2.hpp>
#include "Grid.hpp"

#ifndef EPITECH_WOLF3D_PROJECT_RAYCASTING_HPP
#define EPITECH_WOLF3D_PROJECT_RAYCASTING_HPP

float raycast(sf::Vector2f pos, float direction_deg, const Grid<int> &grid);

#endif //EPITECH_WOLF3D_PROJECT_RAYCASTING_HPP
