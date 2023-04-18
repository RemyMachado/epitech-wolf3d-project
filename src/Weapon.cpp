#include "Weapon.hpp"

Weapon::Weapon(const WeaponSetting &weapon_setting)
	: attack_animation(weapon_setting.attack_animation_params, [this]() { end_attack(); }),
	  attack_delay(weapon_setting.attack_delay),
	  attack_timer(weapon_setting.attack_delay),
	  attack_sound_id(weapon_setting.attack_sound_id),
	  ammo(weapon_setting.ammo) {
  // TODO: generic loader
  SpriteSetting hud_sprite_setting = SPRITE_SETTINGS.at(weapon_setting.hud_sprite_id);
  SpriteSetting idle_sprite_setting = SPRITE_SETTINGS.at(weapon_setting.idle_sprite_id);

  hud_sprite.setTexture(TextureManager::get_instance().get_texture(weapon_setting.hud_sprite_id));
  hud_sprite.setTextureRect(sf::IntRect(
	  hud_sprite_setting.initial_offset.x,
	  hud_sprite_setting.initial_offset.y,
	  hud_sprite_setting.frame_size.x,
	  hud_sprite_setting.frame_size.y));

  idle_sprite.setTexture(TextureManager::get_instance().get_texture(weapon_setting.idle_sprite_id));
  idle_sprite.setTextureRect(sf::IntRect(
	  idle_sprite_setting.initial_offset.x,
	  idle_sprite_setting.initial_offset.y,
	  idle_sprite_setting.frame_size.x,
	  idle_sprite_setting.frame_size.y));
}
bool Weapon::try_attack() {
  if (!is_unlocked || !attack_timer.check_is_elapsed() || ammo <= 0) {
	return false;
  }
  is_attacking = true;
  ammo -= 1;
  attack_animation.reset_animation();
  SoundManager::get_instance().play_sound(attack_sound_id);

  return true;
}
void Weapon::update_sprite() {
  if (is_attacking) {
	attack_animation.update_sprite();
  }
}
sf::Sprite &Weapon::get_current_sprite() {
  if (is_attacking) {
	return attack_animation.getSprite();
  }
  return idle_sprite;
}
sf::Sprite &Weapon::get_hud_sprite() {
  return hud_sprite;
}
void Weapon::end_attack() {
  is_attacking = false;
}
