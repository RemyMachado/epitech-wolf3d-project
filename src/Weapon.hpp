#ifndef EPITECH_WOLF3D_PROJECT_SRC_WEAPON_HPP_
#define EPITECH_WOLF3D_PROJECT_SRC_WEAPON_HPP_

#include <string>
#include "Animation.hpp"
#include "SpriteSetting.hpp"
#include "managers/TextureManager.hpp"
#include "managers/SoundManager.hpp"

class Player;
class Enemy;

enum class WeaponId {
  KNIFE,
  PISTOL,
  THOMPSON,
  MACHINE_GUN,
};

struct WeaponSetting {
  WeaponId id;
  bool is_unlocked;
  AnimationParams attack_animation_params;
  SpriteId hud_sprite_id;
  SpriteId idle_sprite_id;
  float attack_delay;
  SoundId attack_sound_id;
  int initial_ammo;
  float move_speed_multiplier;
  float attack_tile_range;
  float attack_damage;
  float attack_angle_deg;
};

static std::unordered_map<WeaponId, WeaponSetting> WEAPON_SETTINGS = {
    {WeaponId::KNIFE, {
        WeaponId::KNIFE,
        true,
        {SPRITE_SETTINGS.at(SpriteId::KNIFE_ATTACK), 0.2f},
        SpriteId::KNIFE_HUD,
        SpriteId::KNIFE_IDLE,
        0.4f,
        SoundId::KNIFE_ATTACK,
        10000,
        1.6f,
        2.0f,
        20.0f,
        60.0f
    }},
    {WeaponId::PISTOL, {
        WeaponId::PISTOL,
        true,
        {SPRITE_SETTINGS.at(SpriteId::PISTOL_ATTACK), 0.3f},
        SpriteId::PISTOL_HUD,
        SpriteId::PISTOL_IDLE,
        0.8f,
        SoundId::PISTOL_ATTACK,
        16,
        1.0f,
        1000.0f,
        40.0f,
        4.0f
    }},
    {WeaponId::THOMPSON, {
        WeaponId::THOMPSON,
        true,
        {SPRITE_SETTINGS.at(SpriteId::THOMPSON_ATTACK), 0.3f},
        SpriteId::THOMPSON_HUD,
        SpriteId::THOMPSON_IDLE,
        0.3f,
        SoundId::THOMPSON_ATTACK,
        32,
        0.8f,
        1000.0f,
        20.0f,
        4.0f
    }},
    {WeaponId::MACHINE_GUN, {
        WeaponId::MACHINE_GUN,
        true,
        {SPRITE_SETTINGS.at(SpriteId::MACHINE_GUN_ATTACK), 0.1f},
        SpriteId::MACHINE_GUN_HUD,
        SpriteId::MACHINE_GUN_IDLE,
        0.1f,
        SoundId::MACHINE_GUN_ATTACK,
        64,
        0.6f,
        1000.0f,
        10.0f,
        4.0f
    }},
};

class Weapon {
 public:
  WeaponId id;
  bool is_unlocked;
  int ammo;
  float move_speed_multiplier;
  float attack_tile_range;
  float attack_damage;
  float attack_angle_deg;

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

  bool try_attack(Player &player, std::vector<Enemy> &enemies);
  void update_sprite();
  sf::Sprite &get_current_sprite();
  sf::Sprite &get_hud_sprite();

 private:
  bool is_enemy_in_attack_range(Player &player, const Enemy &Enemy) const;
  void end_attack();
};

#endif //EPITECH_WOLF3D_PROJECT_SRC_WEAPON_HPP_
