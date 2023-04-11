#ifndef EPITECH_WOLF3D_PROJECT_SRC_DRAW_HPP_
#define EPITECH_WOLF3D_PROJECT_SRC_DRAW_HPP_

#include <SFML/Graphics.hpp>
#include <SFML/System/Vector2.hpp>

class GameManager;

void draw_minimap(GameManager &game_manager);
void draw_floor_and_ceiling_3d(GameManager &game_manager,
                               sf::Texture &floor_texture,
                               sf::Sprite &floor_sprite,
                               sf::Texture &ceiling_texture,
                               sf::Sprite &ceiling_sprite);
void draw_walls_3d(GameManager &game_manager,
                   sf::Texture &wall_texture,
                   sf::Sprite &wall_sprite);
void draw_hud(GameManager &game_manager);

#endif //EPITECH_WOLF3D_PROJECT_SRC_DRAW_HPP_
