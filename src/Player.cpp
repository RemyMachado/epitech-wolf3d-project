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

void Player::try_attack() const {
  this->current_weapon->try_attack();
}

std::unique_ptr<Weapon> Player::create_knife() {
  float knife_attack_rate = 0.4f;
  SpriteSetting knife_attack_sprite_setting = SPRITE_SETTINGS.at(SpriteId::KNIFE_ATTACK);
  AnimationParams knife_attack_animation_params = AnimationParams(
	  {
		  TextureManager::get_instance().get_texture(
			  SpriteId::KNIFE_ATTACK),
		  knife_attack_sprite_setting.frame_size,
		  knife_attack_sprite_setting.frame_count,
		  knife_attack_sprite_setting.initial_offset,
		  knife_attack_sprite_setting.frame_offset,
		  knife_attack_rate / knife_attack_sprite_setting.frame_count
	  }
  );
  WeaponParams knife_params = WeaponParams(
	  {
		  knife_attack_animation_params,
		  SpriteId::KNIFE_HUD,
		  SpriteId::KNIFE_IDLE,
		  knife_attack_rate,
		  SoundId::KNIFE_ATTACK
	  });
  return std::make_unique<Weapon>(knife_params);
}

std::unique_ptr<Weapon> Player::create_pistol() {
  float pistol_attack_rate = 0.8f;
  SpriteSetting pistol_attack_sprite_setting = SPRITE_SETTINGS.at(SpriteId::PISTOL_ATTACK);
  AnimationParams pistol_attack_animation_params = AnimationParams(
	  {
		  TextureManager::get_instance().get_texture(
			  SpriteId::PISTOL_ATTACK),
		  pistol_attack_sprite_setting.frame_size,
		  pistol_attack_sprite_setting.frame_count,
		  pistol_attack_sprite_setting.initial_offset,
		  pistol_attack_sprite_setting.frame_offset,
		  pistol_attack_rate / pistol_attack_sprite_setting.frame_count
	  }
  );
  WeaponParams pistol_params = WeaponParams(
	  {
		  pistol_attack_animation_params,
		  SpriteId::PISTOL_HUD,
		  SpriteId::PISTOL_IDLE,
		  pistol_attack_rate,
		  SoundId::PISTOL_ATTACK
	  });
  return std::make_unique<Weapon>(pistol_params);
}

Player::Player(sf::Vector2f pos, float direction_deg, float move_speed, Grid &grid, Camera &camera)
	: pos(pos),
	  dir_deg(direction_deg),
	  move_speed(move_speed),
	  grid(grid),
	  camera(camera),
	  knife(create_knife()),
	  pistol(create_pistol()),
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
