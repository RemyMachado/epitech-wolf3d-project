#include <cmath>
#include "raycasting.hpp"
#include "Player.hpp"
#include "my_math.hpp"
#include "Camera.hpp"
#include "Grid.hpp"

void Player::move(float direction_deg) {
  std::optional<Raycast>
      raycast_wall = raycast(this->pos, direction_deg, this->camera.render_distance, this->grid, Tile::Symbol::WALL);

  if (raycast_wall.has_value()) {
    this->pos = polar_to_cartesian(this->pos,
                                   std::min(raycast_wall.value().distance - (grid.tile_size / 100),
                                            this->move_speed),
                                   direction_deg);
  } else {
    this->pos = polar_to_cartesian(this->pos,
                                   this->move_speed,
                                   direction_deg);
  }
}

void Player::move_forward() {
  this->move(this->dir_deg);
}

void Player::move_backward() {
  this->move(this->dir_deg + 180);
}

void Player::move_left() {
  this->move(this->dir_deg - 90);
}

void Player::move_right() {
  this->move(this->dir_deg + 90);
}

void Player::rotate(float direction_deg) {
  this->dir_deg += direction_deg;
  this->dir_deg = std::fmod(this->dir_deg, 360.f);
}
