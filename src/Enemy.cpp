#include "Grid.hpp"
#include "Player.hpp"
#include "Enemy.hpp"
#include "my_math.hpp"
#include "raycasting.hpp"

std::ostream &operator<<(std::ostream &os, const Enemy &enemy) {
  os << "Enemy - " << (enemy.symbol == Tile::Symbol::ENEMY_DOG ? "DOG" : "GUARD") << "(" << enemy.coords.x << ", "
	 << enemy.coords.y << ")";

  return os;
}
void Enemy::update(Player &player) {
  try_attack(player);
  update_sprites();
}

sf::Sprite &Enemy::get_current_sprite() {
  if (is_attacking) {
	return attack_animation.getSprite();
  }
  if (is_dying || is_dead) {
	return death_animation.getSprite();
  }
  if (is_hurting) {
	return hurt_animation.getSprite();
  }
  return idle_sprite;
}

Enemy::Enemy(const EnemySetting &setting, const Grid &grid, sf::Vector2i initial_coords, sf::Vector2f initial_pos)
	: coords(initial_coords),
	  pos(initial_pos),
	  symbol(setting.symbol),
	  attack_animation(setting.attack_animation_params, [this]() { this->end_attack_anim(); }),
	  death_animation(setting.death_animation_params, [this]() { this->end_death_anim(); }),
	  hurt_animation(setting.hurt_animation_params, [this]() { this->end_hurt_anim(); }),
	  attack_delay(setting.attack_delay),
	  attack_timer(setting.attack_delay),
	  attack_damage(setting.attack_damage),
	  grid(grid),
	  attack_tile_range(setting.attack_tile_range),
	  health(setting.health),
	  attack_sound_id(setting.attack_sound_id),
	  hurt_sound_id(setting.hurt_sound_id),
	  death_sound_id(setting.death_sound_id) {
  // generate a random number between 0 and 1
  float random_number = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
  // add a random number between 0 and max_additional_random_attack_delay
  additional_random_attack_delay = random_number * setting.max_additional_random_attack_delay;

  // TODO: generic loader
  SpriteSetting idle_sprite_setting = SPRITE_SETTINGS.at(setting.idle_sprite_id);

  idle_sprite.setTexture(TextureManager::get_instance().get_texture(setting.idle_sprite_id));
  idle_sprite.setTextureRect(sf::IntRect(
	  idle_sprite_setting.initial_offset.x,
	  idle_sprite_setting.initial_offset.y,
	  idle_sprite_setting.frame_size.x,
	  idle_sprite_setting.frame_size.y));

  std::cout << "Enemy created: (" << pos.x << ", " << pos.y << ")" << std::endl;
}
void Enemy::update_idle_animation() {
}
void Enemy::update_hurt_animation() {
  if (is_hurting) {
	hurt_animation.update_sprite();
  }
}
void Enemy::update_attack_animation() {
  if (is_attacking) {
	attack_animation.update_sprite();
  }
}
void Enemy::update_death_animation() {
  if (is_dying) {
	death_animation.update_sprite();
  }
}
void Enemy::update_sprites() {
  update_idle_animation();
  update_attack_animation();
  update_death_animation();
  update_hurt_animation();
}
/*
 * Returns true if the player is in range and there is no wall blocking the attack
 * */
bool Enemy::is_player_in_range(const Player &player) {
  // retrieve distance to player,
  // if the player is in range, check with a raycast if there is a wall blocking the attack

  Polar polar_vector_to_player = cartesian_to_polar(pos, player.get_pos());

  if (polar_vector_to_player.magnitude > attack_tile_range * grid.tile_size) {
	return false;
  }
  auto raycast_result =
	  raycast(pos, polar_vector_to_player.angle_deg, polar_vector_to_player.magnitude, grid, Tile::Symbol::WALL);

  return !raycast_result.has_value();
}
void Enemy::try_attack(Player &player) {
  if (get_is_busy() || !attack_timer.check_is_elapsed(additional_random_attack_delay)) {
	return;
  }
  if (!is_player_in_range(player)) {
	return;
  }
  is_attacking = true;
  attack_animation.reset_animation();
  SoundManager::get_instance().play_sound(attack_sound_id);
  player.take_damage(attack_damage);
}

Enemy::~Enemy() {
  std::cout << "Enemy destroyed: (" << pos.x << ", " << pos.y << ")" << std::endl;
}
bool Enemy::get_is_busy() const {
  return is_attacking || is_dying || is_hurting || is_dead;
}
void Enemy::take_damage(float damage) {
  health -= damage;
  if (health <= 0) {
	die();
  } else {
	is_hurting = true;
	is_attacking = false;
	hurt_animation.reset_animation();
	SoundManager::get_instance().play_sound(hurt_sound_id);
  }
}
void Enemy::die() {
  this->health = 0;
  this->is_dying = true;
  end_attack_anim();
  end_hurt_anim();
  SoundManager::get_instance().play_sound(death_sound_id);
}
void Enemy::end_attack_anim() {
  is_attacking = false;
}
void Enemy::end_death_anim() {
  is_dying = false;
  is_hurting = false;
  is_attacking = false;
  is_dead = true;
  death_animation.force_texture_rect(death_animation.get_last_frame_texture_rect());
}
void Enemy::end_hurt_anim() {
  is_hurting = false;
}
