
#include "Enemy.hpp"

std::ostream &operator<<(std::ostream &os, const Enemy &enemy) {
  os << "Enemy - " << (enemy.symbol == Tile::Symbol::ENEMY_DOG ? "DOG" : "GUARD") << "(" << enemy.coords.x << ", "
	 << enemy.coords.y << ")";

  return os;
}
void Enemy::update() {
  try_attack();
  update_sprites();
}

sf::Sprite &Enemy::get_current_sprite() {
  if (is_attacking) {
	return attack_animation.getSprite();
  }
  return idle_sprite;
}
Enemy::Enemy(const EnemySetting &setting, sf::Vector2i initial_coords, sf::Vector2f initial_pos)
	: coords(initial_coords),
	  pos(initial_pos),
	  symbol(setting.symbol),
	  attack_animation(setting.attack_animation_params, [this]() { this->end_attack(); }),
	  attack_delay(setting.attack_delay),
	  attack_timer(setting.attack_delay),
	  attack_sound_id(setting.attack_sound_id) {
  // TODO: generic loader
  SpriteSetting idle_sprite_setting = SPRITE_SETTINGS.at(setting.idle_sprite_id);

  idle_sprite.setTexture(TextureManager::get_instance().get_texture(setting.idle_sprite_id));
  idle_sprite.setTextureRect(sf::IntRect(
	  idle_sprite_setting.initial_offset.x,
	  idle_sprite_setting.initial_offset.y,
	  idle_sprite_setting.frame_size.x,
	  idle_sprite_setting.frame_size.y));
}
void Enemy::update_idle_animation() {
}
void Enemy::update_attack_animation() {
  if (is_attacking) {
	attack_animation.update_sprite();
  }
}
void Enemy::update_sprites() {
  update_idle_animation();
  update_attack_animation();
}
// TODO: check if the player is in range
void Enemy::try_attack() {
  if (is_attacking || !attack_timer.check_is_elapsed()) {
	return;
  }
  is_attacking = true;
  attack_animation.reset_animation();
  SoundManager::get_instance().play_sound(attack_sound_id);
}
void Enemy::end_attack() {
  is_attacking = false;
}

