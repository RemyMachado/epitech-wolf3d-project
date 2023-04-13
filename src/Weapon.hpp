#ifndef EPITECH_WOLF3D_PROJECT_SRC_WEAPON_HPP_
#define EPITECH_WOLF3D_PROJECT_SRC_WEAPON_HPP_

#include <string>
#include "Animation.hpp"
#include "SpriteSetting.hpp"
#include "managers/TextureManager.hpp"
#include "managers/SoundManager.hpp"

// TODO: Weapon configuration from a constant

struct WeaponParams {
  AnimationParams attack_animation_params;
  SpriteId hud_sprite_id;
  SpriteId idle_sprite_id;
  float attack_rate;
  SoundId attack_sound_id;
};

/*
 * The knife class handles the knife of the player
 * It is responsible for:
 * - drawing the knife on the screen
 * - handling the knife's animations
 * - keeping track of the knife's ammo
 * */
class Weapon {
 public:
  bool is_unlocked = true;

 private:
  int ammo = 100000;
  float attack_rate;
  Timer attack_timer;
  bool is_attacking = false;

  /* Sound */
  SoundId attack_sound_id;

  /* Animation */
  sf::Sprite hud_sprite;
  sf::Sprite idle_sprite;
  Animation attack_animation;

 public:
  explicit Weapon(WeaponParams weapon_params)
	  : attack_animation(weapon_params.attack_animation_params, [this]() { end_attack(); }),
		attack_rate(weapon_params.attack_rate),
		attack_timer(weapon_params.attack_rate),
		attack_sound_id(weapon_params.attack_sound_id) {
	SpriteSetting hud_sprite_setting = SPRITE_SETTINGS.at(weapon_params.hud_sprite_id);
	SpriteSetting idle_sprite_setting = SPRITE_SETTINGS.at(weapon_params.idle_sprite_id);

	hud_sprite.setTexture(TextureManager::get_instance().get_texture(weapon_params.hud_sprite_id));
	hud_sprite.setTextureRect(sf::IntRect(
		hud_sprite_setting.initial_offset.x,
		hud_sprite_setting.initial_offset.y,
		hud_sprite_setting.frame_size.x,
		hud_sprite_setting.frame_size.y));

	idle_sprite.setTexture(TextureManager::get_instance().get_texture(weapon_params.idle_sprite_id));
	idle_sprite.setTextureRect(sf::IntRect(
		idle_sprite_setting.initial_offset.x,
		idle_sprite_setting.initial_offset.y,
		idle_sprite_setting.frame_size.x,
		idle_sprite_setting.frame_size.y));
  }

  bool try_attack() {
	if (!is_unlocked || !attack_timer.check_is_elapsed() || ammo <= 0) {
	  return false;
	}
	is_attacking = true;
	ammo -= 1;
	SoundManager::get_instance().play_sound(attack_sound_id);

	return true;
  }

  void update_sprite() {
	if (is_attacking) {
	  attack_animation.update_sprite();
	}
  }

  sf::Sprite &get_current_sprite() {
	if (is_attacking) {
	  return attack_animation.getSprite();
	}
	return idle_sprite;
  }

  sf::Sprite &get_hud_sprite() {
	return hud_sprite;
  }

 private:
  void end_attack() {
	is_attacking = false;
  }
};

#endif //EPITECH_WOLF3D_PROJECT_SRC_WEAPON_HPP_
