#ifndef EPITECH_WOLF3D_PROJECT_GRID_HPP
#define EPITECH_WOLF3D_PROJECT_GRID_HPP

#include <vector>
#include <SFML/System/Vector2.hpp>
#include <ostream>

template<typename T>
class Grid {
 public:
  const int width;
  const int height;
  const std::vector<T> values;
  const int cube_size = 100;
  sf::Vector2f player_pos = {0, 0};

  Grid(int width, int height, std::vector<T> values) : width(width), height(height), values(values) {}

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
