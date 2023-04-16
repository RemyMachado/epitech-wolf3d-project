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

// TODO: move_speed multiplier (heavier weapons make the player move slower)
class Weapon {
 public:
  bool is_unlocked = true;
  int ammo = 100; // TODO: 10000+ for unlimited ammo and don't show ammo HUD above 1000

 private:
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
  explicit Weapon(const WeaponSetting &weapon_setting);

  bool try_attack();
  void update_sprite();
  sf::Sprite &get_current_sprite();
  sf::Sprite &get_hud_sprite();

 private:
  void end_attack();
};

#endif //EPITECH_WOLF3D_PROJECT_SRC_WEAPON_HPP_
