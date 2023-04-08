#ifndef EPITECH_WOLF3D_PROJECT_GRID_HPP
#define EPITECH_WOLF3D_PROJECT_GRID_HPP

#include <vector>
#include <SFML/System/Vector2.hpp>
#include <ostream>
#include "my_math.hpp"

enum CubeValue {
  EMPTY = 0,
  WALL = 1,
  PLAYER = 9,
};

template<typename T>
class Grid {
 public:
  const int width;
  const int height;
  const std::vector<T> values;
  const float cube_size = 10;
  sf::Vector2f player_pos = {0, 0};
  float player_direction_deg = 0;
  float player_move_speed;
  float render_distance; // equals the diagonal of the grid
  float fov_deg = 60;
  bool recenter_mouse = false;
  float mouse_rotation_speed_deg = 0.02f;
  float keyboard_rotation_speed_deg = 5;
  float player_circle_radius_minimap = 5;

  Grid(int width, int height, std::vector<T> values)
      : width(width),
        height(height),
        values(values),
        render_distance(magnitude({(float) width, (float) height}) * cube_size),
        player_move_speed(cube_size * 0.1f) {
    // find the player position in the grid (number '9')
    for (int y = 0; y < height; y++) {
      for (int x = 0; x < width; x++) {
        if (get(x, y) == 9) {
          player_pos = {(float) x * cube_size + cube_size / 2, (float) y * cube_size + cube_size / 2};
          return;
        }
      }
    }

    // if the player position is not found, throw an invalid file exception
    throw std::invalid_argument("The grid file is invalid: the player position is not found ('9').");
  }

  const T &get(int x, int y) const {
    return values[y * width + x];
  }

  // stringify the grid with the cout operator
  friend std::ostream &operator<<(std::ostream &os, const Grid<T> &grid) {
    for (int y = 0; y < grid.height; y++) {
      for (int x = 0; x < grid.width; x++) {
        os << grid.get(x, y);
      }
      os << std::endl;
    }
    return os;
  }
};

#endif //EPITECH_WOLF3D_PROJECT_GRID_HPP
