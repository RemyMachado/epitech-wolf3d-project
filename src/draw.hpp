#ifndef EPITECH_WOLF3D_PROJECT_SRC_DRAW_HPP_
#define EPITECH_WOLF3D_PROJECT_SRC_DRAW_HPP_

#include <SFML/Graphics.hpp>
#include "Grid.hpp"
#include "raycasting.hpp"

void draw_minimap(sf::RenderWindow &window, Grid<int> &grid, std::vector<Cube> &grid_cubes);
void draw_floor_and_ceiling_3d(sf::RenderWindow &window,
                               Grid<int> &grid,
                               std::vector<Cube> &grid_cubes,
                               sf::Texture &floor_texture,
                               sf::Sprite &floor_sprite,
                               sf::Texture &ceiling_texture,
                               sf::Sprite &ceiling_sprite);
void draw_walls_3d(sf::RenderWindow &window,
                   Grid<int> &grid,
                   std::vector<Cube> &grid_cubes,
                   sf::Texture &wall_texture,
                   sf::Sprite &wall_sprite, sf::Texture &floor_texture,
                   sf::Sprite &floor_sprite, sf::Texture &ceiling_texture,
                   sf::Sprite &ceiling_sprite);

#endif //EPITECH_WOLF3D_PROJECT_SRC_DRAW_HPP_
