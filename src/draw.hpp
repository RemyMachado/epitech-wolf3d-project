#ifndef EPITECH_WOLF3D_PROJECT_SRC_DRAW_HPP_
#define EPITECH_WOLF3D_PROJECT_SRC_DRAW_HPP_

#include <utility>
#include <unordered_map>
#include <SFML/Graphics.hpp>
#include <SFML/System/Vector2.hpp>
#include "raycasting.hpp"

class GameManager;
struct ComputedDrawHit;
/*struct PairHash;*/

void draw_minimap(GameManager &game_manager);
/*
void draw_chunked_raycast_hits(std::vector<std::vector<ComputedDrawHit>>
							   &chunked_raycast_hits, GameManager &game_manager,
							   int raycast_thickness,
							   sf::Texture &wall_texture,
							   sf::Sprite &wall_sprite,
							   sf::Texture &floor_texture,
							   sf::Sprite &floor_sprite,
							   sf::Texture &ceiling_texture,
							   sf::Sprite &ceiling_sprite);
*/
void draw_floor_and_ceiling_3d(GameManager &game_manager,
							   sf::Sprite &floor_sprite,
							   sf::Sprite &ceiling_sprite);

/*
 * Returns a vector of raycast hits, sorted from x=0 to x=window_width.
 * */
std::vector<std::optional<Raycast>> draw_walls_3d(GameManager &game_manager,
												  sf::Sprite &wall_sprite,
												  sf::Sprite &wall_shadow_sprite);
/*
void draw_raycast_map(std::vector<ComputedDrawHit> &raycast_map,
					  sf::RenderWindow &window,
					  int raycast_thickness,
					  sf::Texture &wall_texture,
					  sf::Sprite &wall_sprite,
					  sf::Texture &floor_texture,
					  sf::Sprite &floor_sprite,
					  sf::Texture &ceiling_texture,
					  sf::Sprite &ceiling_sprite);
*/
void draw_hud(GameManager &game_manager, sf::Sprite &hud_sprite);
void render_game_frame(GameManager &game_manager,
					   sf::Sprite &wall_sprite,
					   sf::Sprite &wall_shadow_sprite,
					   sf::Sprite &floor_sprite,
					   sf::Sprite &ceiling_sprite,
					   sf::Sprite &hud_empty_sprite);
void render_game_over_screen(GameManager &game_manager, sf::Sprite &hud_empty_sprite);

#endif //EPITECH_WOLF3D_PROJECT_SRC_DRAW_HPP_
