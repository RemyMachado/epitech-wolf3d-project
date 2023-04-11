#ifndef EPITECH_WOLF3D_PROJECT_SRC_PLAYER_HPP_
#define EPITECH_WOLF3D_PROJECT_SRC_PLAYER_HPP_

#include <SFML/System/Vector2.hpp>

class Grid;
class Camera;

class Player {
 public:
  sf::Vector2f pos;
  float dir_deg;
  float move_speed;
  Grid &grid;
  Camera &camera;

  Player(sf::Vector2f pos,
         float direction_deg,
         float move_speed,
         Grid &grid,
         Camera &camera)
      : pos(pos),
        dir_deg(direction_deg),
        move_speed(move_speed),
        grid(grid),
        camera(camera) {};

  void move_forward();
  void move_backward();
  void move_left();
  void move_right();
  void rotate(float direction_deg);

 private:
  void move(float direction_deg);

};

#endif //EPITECH_WOLF3D_PROJECT_SRC_PLAYER_HPP_
