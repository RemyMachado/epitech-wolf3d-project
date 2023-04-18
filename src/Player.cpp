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
  float move_speed_with_weapon = this->move_speed * this->current_weapon->move_speed_multiplier;

  float minimum_distance_from_wall = this->grid.tile_size * body_radius;

  sf::Vector2f new_ghost_pos = polar_to_cartesian(this->pos, move_speed_with_weapon, direction_deg);

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
	  weapon_switch_timer(0.2f) {
  SpriteSetting hud_face_sprite_setting = SPRITE_SETTINGS.at(SpriteId::HUD_FACES);
  hud_face_sprite.setTexture(TextureManager::get_instance().get_texture(SpriteId::HUD_FACES));
  int current_face_frame = this->get_hud_face_animation_frame();
  hud_face_sprite.setTextureRect(sf::IntRect(
	  hud_face_sprite_setting.initial_offset.x + current_face_frame * hud_face_sprite_setting.frame_offset.x,
	  hud_face_sprite_setting.initial_offset.y + current_face_frame * hud_face_sprite_setting.frame_offset.y,
	  hud_face_sprite_setting.frame_size.x,
	  hud_face_sprite_setting.frame_size.y));
}

void Player::switch_weapon(Weapon *new_weapon) {
  if (current_weapon == new_weapon ||
	  !new_weapon->is_unlocked ||
	  !this->weapon_switch_timer.check_is_elapsed()) {
	return;
  }

  current_weapon = new_weapon;
}

int Player::get_hud_face_animation_frame() const {
  SpriteSetting hud_face_sprite_setting = SPRITE_SETTINGS.at(SpriteId::HUD_FACES);
  int alive_frame_count = hud_face_sprite_setting.frame_count - 1; // because last frame is dead face

  float health_per_frame = (float)100 / (float)alive_frame_count;

  int current_health_frame = alive_frame_count - this->health / health_per_frame;

  return current_health_frame;
}

void Player::update_hud_face_sprite() {
  SpriteSetting hud_face_sprite_setting = SPRITE_SETTINGS.at(SpriteId::HUD_FACES);
  int current_face_frame = this->get_hud_face_animation_frame();

  hud_face_sprite.setTextureRect(sf::IntRect(
	  hud_face_sprite_setting.initial_offset.x + current_face_frame * hud_face_sprite_setting.frame_offset.x,
	  hud_face_sprite_setting.initial_offset.y + current_face_frame * hud_face_sprite_setting.frame_offset.y,
	  hud_face_sprite_setting.frame_size.x,
	  hud_face_sprite_setting.frame_size.y));
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
void Player::update_sprites() {
  this->update_current_weapon_sprite();
  this->update_hud_face_sprite();
}
void Player::update() {
  if (get_is_dead()) {
	return;
  }
  update_sprites();
}
sf::Sprite &Player::get_hud_face_sprite() {
  return this->hud_face_sprite;
}
sf::Vector2f Player::get_dir_vector() const {
  return compute_dir_vector(this->dir_deg);
}
sf::Vector2f Player::get_pos() const {
  return this->pos;
}
void Player::take_damage(float damage) {
  this->health -= damage;
  if (this->health <= 0) {
	die();
  } else {
	// pick a random pain sound
	int random_pain_sound = rand() % 2;
	std::cout << random_pain_sound << std::endl;
	if (random_pain_sound == 0) {
	  SoundManager::get_instance().play_sound(SoundId::PLAYER_SMALL_PAIN);
	} else {
	  SoundManager::get_instance().play_sound(SoundId::PLAYER_BIG_PAIN);
	}
  }
}
bool Player::get_is_dead() const {
  return this->health <= 0;
}
void Player::die() {
  this->health = 0;
  lives--;
  SoundManager::get_instance().play_sound(SoundId::PLAYER_DIES);
}
