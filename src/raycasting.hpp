#ifndef EPITECH_WOLF3D_PROJECT_RAYCASTING_HPP
#define EPITECH_WOLF3D_PROJECT_RAYCASTING_HPP

#include <SFML/System/Vector2.hpp>
#include <optional>
#include "Tile.hpp"

class Grid;
class Line;

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

#endif //EPITECH_WOLF3D_PROJECT_RAYCASTING_HPP
