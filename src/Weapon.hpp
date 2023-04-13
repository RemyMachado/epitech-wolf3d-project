#ifndef EPITECH_WOLF3D_PROJECT_SRC_WEAPON_HPP_
#define EPITECH_WOLF3D_PROJECT_SRC_WEAPON_HPP_

#include <string>
#include "Animation.hpp"
#include "SpriteSetting.hpp"
#include "managers/TextureManager.hpp"
#include "managers/SoundManager.hpp"

enum class WeaponId {
  KNIFE,
  PISTOL
};

struct WeaponSetting {
  WeaponId id;
  AnimationParams attack_animation_params;
  SpriteId hud_sprite_id;
  SpriteId idle_sprite_id;
  float attack_delay;
  SoundId attack_sound_id;
};

static std::unordered_map<WeaponId, WeaponSetting> WEAPON_SETTINGS = {
	{WeaponId::KNIFE, {
		WeaponId::KNIFE,
		{SPRITE_SETTINGS.at(SpriteId::KNIFE_ATTACK), 0.2f},
		SpriteId::KNIFE_HUD,
		SpriteId::KNIFE_IDLE,
		0.4f,
		SoundId::KNIFE_ATTACK
	}},
	{WeaponId::PISTOL, {
		WeaponId::PISTOL,
		{SPRITE_SETTINGS.at(SpriteId::PISTOL_ATTACK), 0.3f},
		SpriteId::PISTOL_HUD,
		SpriteId::PISTOL_IDLE,
		0.8f,
		SoundId::PISTOL_ATTACK
	}}
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
  float attack_delay;
  Timer attack_timer;
  bool is_attacking = false;

  /* Sound */
  SoundId attack_sound_id;

  /* Animation */
  sf::Sprite hud_sprite;
  sf::Sprite idle_sprite;
  Animation attack_animation;

 public:
  explicit Weapon(const WeaponSetting &weapon_setting)
	  : attack_animation(weapon_setting.attack_animation_params, [this]() { end_attack(); }),
		attack_delay(weapon_setting.attack_delay),
		attack_timer(weapon_setting.attack_delay),
		attack_sound_id(weapon_setting.attack_sound_id) {
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

  bool try_attack() {
	if (!is_unlocked || !attack_timer.check_is_elapsed() || ammo <= 0) {
	  return false;
	}
	is_attacking = true;
	ammo -= 1;
	attack_animation.reset_animation();
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
