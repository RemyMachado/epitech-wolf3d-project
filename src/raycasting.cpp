#include <tuple>
#include <iostream>
#include "raycasting.hpp"
#include "lines.hpp"

Cube get_cube(sf::Vector2i pos, Grid<int> &grid) {
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

  return {
      (CubeValue) grid.get(pos.x, pos.y),
      pos,
      {front, right, back, left}
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

      std::optional<sf::Vector2f>
          intersection = get_segments_intersection({pos, ray_max_distance_pos}, segment);

      // if there is an intersection
      if (intersection.has_value()) {
        float distance_to_intersection = magnitude(intersection.value() - pos);

        // if there is no closest intersection yet, or if the current intersection is closer than the closest intersection
        if (!closest_raycast.has_value()
            || distance_to_intersection < closest_raycast.value().distance) {
          // set the closest intersection to the current intersection
          closest_raycast = Raycast{
              distance_to_intersection,
              intersection.value(),
              // TODO: handle faces behind, they risk to have a negative local_intersection_x
              intersection.value().x - (float) cube.pos.x * grid.cube_size,
              segment,
              cube
          };
        }
      }
    }
  }

  return closest_raycast;
}
