
#include "Enemy.hpp"

std::ostream &operator<<(std::ostream &os, const Enemy &enemy) {
  os << "Enemy - " << (enemy.symbol == Tile::Symbol::ENEMY_DOG ? "DOG" : "GUARD") << "(" << enemy.coords.x << ", "
	 << enemy.coords.y << ")";

  return os;
}
void Enemy::update_sprites() {
  // TODO:
  if (symbol == Tile::Symbol::ENEMY_DOG) {
	SpriteSetting sprite_setting = SPRITE_SETTINGS.at(SpriteId::ENEMY_DOG_IDLE);
	idle_sprite = sf::Sprite(TextureManager::get_instance().get_texture(SpriteId::ENEMY_DOG_IDLE));
	idle_sprite.setTextureRect(sf::IntRect(sprite_setting.initial_offset.x,
										   sprite_setting.initial_offset.y,
										   sprite_setting.frame_size.x,
										   sprite_setting.frame_size.y));
  } else {
	SpriteSetting sprite_setting = SPRITE_SETTINGS.at(SpriteId::ENEMY_GUARD_IDLE);
	idle_sprite = sf::Sprite(TextureManager::get_instance().get_texture(SpriteId::ENEMY_GUARD_IDLE));
	idle_sprite.setTextureRect(sf::IntRect(sprite_setting.initial_offset.x,
										   sprite_setting.initial_offset.y,
										   sprite_setting.frame_size.x,
										   sprite_setting.frame_size.y));
  }
}

sf::Sprite &Enemy::get_current_sprite() {
  return idle_sprite;
}

