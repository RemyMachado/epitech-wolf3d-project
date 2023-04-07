#include <tuple>
#include <iostream>
#include "raycasting.hpp"
#include "lines.hpp"

CubeRaycastSegments get_raycast_cube_segments(sf::Vector2i pos, Grid<int> &grid) {
  if (grid.get(pos.x, pos.y) != 1)
    return {};

  Line front = {
      {(float) pos.x * grid.cube_size, (float) pos.y * grid.cube_size},
      {(float) (pos.x + 1) * grid.cube_size, (float) pos.y * grid.cube_size}
  };
  Line right = {
      {(float) (pos.x + 1) * grid.cube_size, (float) pos.y * grid.cube_size},
      {(float) (pos.x + 1) * grid.cube_size, ((float) pos.y + 1) * grid.cube_size}
  };
  Line back = {
      {(float) (pos.x + 1) * grid.cube_size, ((float) pos.y + 1) * grid.cube_size},
      {(float) pos.x * grid.cube_size, ((float) pos.y + 1) * grid.cube_size}
  };
  Line left = {
      {(float) pos.x * grid.cube_size, ((float) pos.y + 1) * grid.cube_size},
      {(float) pos.x * grid.cube_size, (float) pos.y * grid.cube_size}
  };

  return {front, right, back, left};
}

std::vector<CubeRaycastSegments> get_grid_raycast_segments(Grid<int> &grid) {
  std::vector<CubeRaycastSegments> segments;

  for (int y = 0; y < grid.height; y++) {
    for (int x = 0; x < grid.width; x++) {
      segments.push_back(get_raycast_cube_segments({x, y}, grid));
    }
  }

  return segments;
}

std::optional<float> raycast(sf::Vector2f pos, float direction_deg, Grid<int> &grid) {
  // get the raycast segments for each cube in the grid
  std::vector<CubeRaycastSegments> cube_segments = get_grid_raycast_segments(grid);

  // get closest intersection from pos to direction_deg with cube_segments
  std::optional<sf::Vector2f> closest_intersection;
  // iterate over each cube
  for (CubeRaycastSegments cube_segment : cube_segments) {
    // iterate over each segment of the cube
    for (Line segment : cube_segment) {
      // get the intersection between the ray and the segment
      sf::Vector2f ray_end_pos = polar_to_cartesian(pos, grid.render_distance, direction_deg);

      std::optional<sf::Vector2f>
          intersection = get_segments_intersection(pos, ray_end_pos, segment.start_pos, segment.end_pos);

      // if there is an intersection
      if (intersection.has_value()) {
        float distance_to_intersection = magnitude(intersection.value() - pos);

        // if there is no closest intersection yet, or if the current intersection is closer than the closest intersection
        if (!closest_intersection.has_value()
            || distance_to_intersection < magnitude(closest_intersection.value() - pos)) {
          // set the closest intersection to the current intersection
          closest_intersection = intersection;
        }
      }
    }
  }

  // if there is a closest intersection, return the distance to it
  if (closest_intersection.has_value()) {
    return magnitude(closest_intersection.value() - pos);
  }

  return std::nullopt;
}
