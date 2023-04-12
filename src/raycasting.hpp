#ifndef EPITECH_WOLF3D_PROJECT_RAYCASTING_HPP
#define EPITECH_WOLF3D_PROJECT_RAYCASTING_HPP

#include <utility>
#include <unordered_map>
#include <SFML/System/Vector2.hpp>
#include <optional>
#include "Tile.hpp"

class GameManager;
class Grid;
class Line;

/*
 * To store a pair of two values as a key in an unordered_map, we need to define a hash function for it.
 * */
struct PairHash {
  template<class T1, class T2>
  std::size_t operator()(const std::pair<T1, T2> &pair) const {
	auto hash1 = std::hash<T1>{}(pair.first);
	auto hash2 = std::hash<T2>{}(pair.second);
	return hash1 ^ hash2;
  }
};

struct ComputedDrawHit {
  sf::Vector2i pixel_pos;
  Tile::Symbol tile_symbol;
  sf::Vector2f texture_percentage_coords;

  ComputedDrawHit(Tile::Symbol tile_symbol, sf::Vector2f texture_percentage_coords)
	  : tile_symbol(tile_symbol), texture_percentage_coords(texture_percentage_coords) {}
};

struct Raycast {
  float distance;
  sf::Vector2f intersection_pos;
  sf::Vector2f local_intersection;
  Tile::Side hit_side;
  Line intersected_segment;
  Tile tile;
};

Tile::Side determine_hit_side(float tile_size, const Tile &tile, const Line &intersected_segment);
std::optional<Raycast> raycast(sf::Vector2f origin,
							   float direction_deg,
							   float render_distance,
							   Grid &grid,
							   Tile::Symbol symbol_target);
std::vector<ComputedDrawHit> compute_partial_walls_raycast_vec(
	GameManager &game_manager,
	int start_x,
	int end_x,
	int ray_thickness);
std::vector<ComputedDrawHit> compute_partial_floor_raycast_vec(GameManager &game_manager,
															   int start_y,
															   int end_y,
															   int ray_thickness);
std::vector<ComputedDrawHit> &compute_walls_raycast_vec(std::vector<ComputedDrawHit> &raycast_vec,
														GameManager &game_manager,
														int field_width,
														int field_height,
														int ray_thickness);
std::vector<ComputedDrawHit> &compute_floor_raycast_vec(std::vector<ComputedDrawHit> &raycast_vec,
														GameManager &game_manager,
														int field_width,
														int field_height,
														int ray_thickness);

#endif //EPITECH_WOLF3D_PROJECT_RAYCASTING_HPP
