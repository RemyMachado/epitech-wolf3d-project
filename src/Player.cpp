#include <cmath>
#include "raycasting.hpp"
#include "Player.hpp"
#include "my_math.hpp"
#include "Camera.hpp"
#include "Grid.hpp"

/*
 * Handles wall sliding by staying away from the wall from a minimum distance.
 * */
void Player::move(float direction_deg) {
  float minimum_distance_from_wall = this->grid.tile_size * body_radius;

  sf::Vector2f new_ghost_pos = polar_to_cartesian(this->pos, this->move_speed, direction_deg);

  float delta_x = new_ghost_pos.x - this->pos.x;
  float delta_y = new_ghost_pos.y - this->pos.y;

  float x_dir_deg = delta_x > 0 ? 0 : 180;

  std::optional<Raycast>
	  raycast_wall_x = raycast(this->pos, x_dir_deg, camera.render_distance, this->grid, Tile::Symbol::WALL);

  if (raycast_wall_x.has_value()) {
	// if going too close to a wall in the x-axis
	if (raycast_wall_x.value().distance < (std::abs(delta_x) + minimum_distance_from_wall)) {
	  // need to keep a small distance from the wall to avoid clipping
	  if (delta_x > 0) {
		this->pos.x += raycast_wall_x.value().distance - minimum_distance_from_wall; // working
	  } else {
		this->pos.x -= raycast_wall_x.value().distance - minimum_distance_from_wall;
	  }
	} else {
	  this->pos.x += delta_x;
	}
  } else {
	this->pos.x += delta_x;
  }

  float y_dir_deg = delta_y > 0 ? 90 : -90;

  std::optional<Raycast>
	  raycast_wall_y = raycast(this->pos, y_dir_deg, camera.render_distance, this->grid, Tile::Symbol::WALL);

  if (raycast_wall_y.has_value()) {
	if (raycast_wall_y.value().distance < (std::abs(delta_y) + minimum_distance_from_wall)) {
	  // need to keep a small distance from the wall to avoid clipping
	  if (delta_y > 0) {
		this->pos.y += raycast_wall_y.value().distance - minimum_distance_from_wall;
	  } else {
		this->pos.y -= raycast_wall_y.value().distance - minimum_distance_from_wall;
	  }
	} else {
	  this->pos.y += delta_y;
	}
  } else {
	this->pos.y += delta_y;
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

void Player::try_attack() const {
  this->current_weapon->try_attack();
}

Player::Player(sf::Vector2f pos, float direction_deg, float move_speed, Grid &grid, Camera &camera)
	: pos(pos),
	  dir_deg(direction_deg),
	  move_speed(move_speed),
	  grid(grid),
	  camera(camera),
	  knife(std::make_unique<Weapon>(WEAPON_SETTINGS.at(WeaponId::KNIFE))),
	  pistol(std::make_unique<Weapon>(WEAPON_SETTINGS.at(WeaponId::PISTOL))),
	  current_weapon(knife.get()),
	  weapon_switch_timer(0.2f) {}

void Player::switch_weapon(Weapon *new_weapon) {
  if (current_weapon == new_weapon ||
	  !new_weapon->is_unlocked ||
	  !this->weapon_switch_timer.check_is_elapsed()) {
	return;
  }

  current_weapon = new_weapon;
}

void Player::update_current_weapon_sprite() const {
  this->current_weapon->update_sprite();
}

void Player::select_knife() {
  this->switch_weapon(this->knife.get());
}

void Player::select_pistol() {
  this->switch_weapon(this->pistol.get());
}
