#ifndef EPITECH_WOLF3D_PROJECT_SRC_DRAW_HPP_
#define EPITECH_WOLF3D_PROJECT_SRC_DRAW_HPP_

#include <utility>
#include <unordered_map>
#include <SFML/Graphics.hpp>
#include <SFML/System/Vector2.hpp>

class GameManager;
struct ComputedDrawHit;
struct PairHash;

void draw_minimap(GameManager &game_manager);
void draw_chunked_raycast_hits(std::vector<std::vector<ComputedDrawHit>>
							   &chunked_raycast_hits, GameManager &game_manager,
							   int raycast_thickness,
							   sf::Texture &wall_texture,
							   sf::Sprite &wall_sprite,
							   sf::Texture &floor_texture,
							   sf::Sprite &floor_sprite,
							   sf::Texture &ceiling_texture,
							   sf::Sprite &ceiling_sprite);
void draw_floor_and_ceiling_3d(GameManager &game_manager,
							   sf::Texture &floor_texture,
							   sf::Sprite &floor_sprite,
							   sf::Texture &ceiling_texture,
							   sf::Sprite &ceiling_sprite);
void draw_walls_3d(GameManager &game_manager,
				   sf::Texture &wall_texture,
				   sf::Sprite &wall_sprite);
void draw_raycast_map(std::vector<ComputedDrawHit> &raycast_map,
					  sf::RenderWindow &window,
					  int raycast_thickness,
					  sf::Texture &wall_texture,
					  sf::Sprite &wall_sprite,
					  sf::Texture &floor_texture,
					  sf::Sprite &floor_sprite,
					  sf::Texture &ceiling_texture,
					  sf::Sprite &ceiling_sprite);
void draw_hud(GameManager &game_manager, sf::Texture &hud_texture, sf::Sprite &hud_sprite);
void render_game_frame(GameManager &game_manager,
					   sf::Texture &wall_texture,
					   sf::Sprite &wall_sprite,
					   sf::Texture &floor_texture,
					   sf::Sprite &floor_sprite,
					   sf::Texture &ceiling_texture,
					   sf::Sprite &ceiling_sprite,
					   sf::Texture &hud_empty_texture,
					   sf::Sprite &hud_empty_sprite);

#endif //EPITECH_WOLF3D_PROJECT_SRC_DRAW_HPP_
