#include <optional>
#include <SFML/System/Vector2.hpp>
#include "raycasting.hpp"
#include "my_math.hpp"
#include "Grid.hpp"
#include "Tile.hpp"

Tile::Side determine_hit_side(float tile_size, const Tile &tile, const Line &intersected_segment) {
  // Get the center of the intersected segment
  sf::Vector2f segment_center = (intersected_segment.start + intersected_segment.end) / 2.0f;

  // Get the tile center
  sf::Vector2f tile_center = tile.pos + sf::Vector2f{tile_size / 2.0f, tile_size / 2.0f};

  // Calculate the direction vector from the center of the tile to the center of the hit segment
  sf::Vector2f tile_center_to_segment_center = segment_center - tile_center;

  // Normalize the vector
  sf::Vector2f normalized_tile_center_to_segment_center = normalize_vector(tile_center_to_segment_center);

  // Check the direction of the normalized vector to determine the hit side
  if (std::abs(normalized_tile_center_to_segment_center.x) > std::abs(normalized_tile_center_to_segment_center.y)) {
    // The raycast hit an east or west face
    return (normalized_tile_center_to_segment_center.x > 0) ? Tile::Side::EAST : Tile::Side::WEST;
  } else {
    // The raycast hit a north or south face
    return (normalized_tile_center_to_segment_center.y > 0) ? Tile::Side::SOUTH : Tile::Side::NORTH;
  }
}

std::optional<Raycast> raycast(sf::Vector2f origin,
                               float direction_deg,
                               float render_distance,
                               Grid &grid,
                               Tile::Symbol symbol_target) {
  // get closest intersection from pos to dir_deg with symbol_target horizontal_segments
  std::optional<Raycast> closest_raycast = std::nullopt;

  // iterate over each tile
  for (const Tile &tile : grid.tiles) {
    if (tile.symbol != symbol_target) {
      continue;
    }

    // iterate over each segment of the tile
    for (const Line &horizontal_segment : tile.horizontal_segments) {
      // get the intersection between the ray and the horizontal_segment
      sf::Vector2f
          ray_max_distance_pos = polar_to_cartesian(origin, render_distance, direction_deg);
      Line ray = {origin, ray_max_distance_pos};

      std::optional<sf::Vector2f>
          intersection = get_segments_intersection(ray, horizontal_segment);

      // if there is an intersection
      if (intersection.has_value()) {
        float distance_to_intersection = get_magnitude(intersection.value() - origin);

        // if there is no closest intersection yet, or if the current intersection is closer than the closest intersection
        if (!closest_raycast.has_value()
            || distance_to_intersection < closest_raycast.value().distance) {
          sf::Vector2f local_intersection =
              {intersection.value().x - (float) tile.pos.x, intersection.value().y - (float) tile.pos.y};

          Tile::Side hit_side = determine_hit_side(grid.tile_size, tile, horizontal_segment);

          // set the closest intersection to the current intersection
          closest_raycast = Raycast{
              distance_to_intersection,
              intersection.value(),
              local_intersection,
              hit_side,
              horizontal_segment,
              tile
          };
        }
      }
    }
  }

  return closest_raycast;
}
