#include <tuple>
#include <iostream>
#include "raycasting.hpp"
#include "lines.hpp"

Cube get_cube(sf::Vector2i indices, Grid<int> &grid) {
  Line north = {
      {(float) indices.x * grid.cube_size, (float) indices.y * grid.cube_size},
      {(float) (indices.x + 1) * grid.cube_size, (float) indices.y * grid.cube_size}
  };
  Line east = {
      {(float) (indices.x + 1) * grid.cube_size, (float) indices.y * grid.cube_size},
      {(float) (indices.x + 1) * grid.cube_size, ((float) indices.y + 1) * grid.cube_size}
  };
  Line south = {
      {(float) indices.x * grid.cube_size, ((float) indices.y + 1) * grid.cube_size},
      {(float) (indices.x + 1) * grid.cube_size, ((float) indices.y + 1) * grid.cube_size}
  };
  Line west = {
      {(float) indices.x * grid.cube_size, (float) indices.y * grid.cube_size},
      {(float) indices.x * grid.cube_size, ((float) indices.y + 1) * grid.cube_size}
  };

  return {
      (CubeValue) grid.get(indices.x, indices.y),
      indices,
      {(float) indices.x * grid.cube_size, (float) indices.y * grid.cube_size},
      {north, east, south, west}
  };
}

std::vector<Cube> get_grid_cubes(Grid<int> &grid) {
  std::vector<Cube> cubes;

  for (int y = 0; y < grid.height; y++) {
    for (int x = 0; x < grid.width; x++) {
      cubes.push_back(get_cube({x, y}, grid));
    }
  }

  return cubes;
}

CubeSide determine_hit_side(Grid<int> &grid, const Line &intersected_segment, const Cube &cube) {
  // Get the center of the intersected segment
  sf::Vector2f segment_center = (intersected_segment.start + intersected_segment.end) / 2.0f;

  // Get the cube center
  sf::Vector2f cube_center = cube.pos + sf::Vector2f{grid.cube_size / 2.0f, grid.cube_size / 2.0f};

  // Calculate the direction vector from the center of the cube to the center of the hit segment
  sf::Vector2f cube_center_to_segment_center = segment_center - cube_center;

  // Normalize the vector
  sf::Vector2f normalized_cube_center_to_segment_center = normalize_vector(cube_center_to_segment_center);

  // Check the direction of the normalized vector to determine the hit side
  if (std::abs(normalized_cube_center_to_segment_center.x) > std::abs(normalized_cube_center_to_segment_center.y)) {
    // The raycast hit an east or west face
    return (normalized_cube_center_to_segment_center.x > 0) ? CubeSide::EAST : CubeSide::WEST;
  } else {
    // The raycast hit a north or south face
    return (normalized_cube_center_to_segment_center.y > 0) ? CubeSide::SOUTH : CubeSide::NORTH;
  }
}

std::optional<Raycast> raycast(sf::Vector2f pos,
                               float direction_deg,
                               Grid<int> &grid,
                               std::vector<Cube> &grid_cubes, CubeValue cube_target) {
  // get closest intersection from pos to direction_deg with cube_target segments
  std::optional<Raycast> closest_raycast = std::nullopt;

  // iterate over each cube
  for (const Cube &cube : grid_cubes) {
    if (cube.value != cube_target) {
      continue;
    }

    // iterate over each segment of the cube
    for (Line segment : cube.segments) {
      // get the intersection between the ray and the segment
      sf::Vector2f ray_max_distance_pos = polar_to_cartesian(pos, grid.render_distance, direction_deg);
      Line ray = {pos, ray_max_distance_pos};

      std::optional<sf::Vector2f>
          intersection = get_segments_intersection(ray, segment);

      // if there is an intersection
      if (intersection.has_value()) {
        float distance_to_intersection = get_magnitude(intersection.value() - pos);

        // if there is no closest intersection yet, or if the current intersection is closer than the closest intersection
        if (!closest_raycast.has_value()
            || distance_to_intersection < closest_raycast.value().distance) {
          sf::Vector2f local_intersection =
              {intersection.value().x - (float) cube.pos.x, intersection.value().y - (float) cube.pos.y};

          CubeSide hit_side = determine_hit_side(grid, segment, cube);

          // set the closest intersection to the current intersection
          closest_raycast = Raycast{
              distance_to_intersection,
              intersection.value(),
              local_intersection,
              hit_side,
              segment,
              cube
          };
        }
      }
    }
  }

  return closest_raycast;
}
