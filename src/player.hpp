#ifndef EPITECH_WOLF3D_PROJECT_SRC_PLAYER_HPP_
#define EPITECH_WOLF3D_PROJECT_SRC_PLAYER_HPP_

#include <SFML/Graphics.hpp>
#include <optional>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Event.hpp>
#include "Grid.hpp"
#include "raycasting.hpp"

void handle_player_movement(Grid<int> &grid, std::vector<CubeRaycastSegments> &grid_raycast_segments);
void handle_player_camera_rotation(Grid<int> &grid, sf::RenderWindow &window, sf::Event &event);

#endif //EPITECH_WOLF3D_PROJECT_SRC_PLAYER_HPP_
