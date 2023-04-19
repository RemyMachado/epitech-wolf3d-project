#include "Weapon.hpp"
#include "Enemy.hpp"
#include "Player.hpp"
#include "my_math.hpp"
#include "raycasting.hpp"

Weapon::Weapon(const WeaponSetting &weapon_setting)
	: id(weapon_setting.id),
	  attack_animation(weapon_setting.attack_animation_params, {[this]() { end_attack(); }}),
	  attack_delay(weapon_setting.attack_delay),
	  attack_timer(weapon_setting.attack_delay),
	  attack_sound_id(weapon_setting.attack_sound_id),
	  ammo(weapon_setting.ammo),
	  move_speed_multiplier(weapon_setting.move_speed_multiplier),
	  attack_tile_range(weapon_setting.attack_tile_range),
	  attack_damage(weapon_setting.attack_damage),
	  attack_angle_deg(weapon_setting.attack_angle_deg) {
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
/*
 * Returns true if the enemy is in range and there is no wall blocking the attack
 * */
bool Weapon::is_enemy_in_attack_range(Player &player, const Enemy &enemy) const {
  Polar polar_vector_to_enemy = cartesian_to_polar(player.pos, enemy.pos);

  // range check
  if (polar_vector_to_enemy.magnitude > attack_tile_range * player.grid.tile_size) {
	return false;
  }

  // wall check
  auto raycast_result =
	  raycast(player.pos,
			  polar_vector_to_enemy.angle_deg,
			  polar_vector_to_enemy.magnitude,
			  player.grid,
			  Tile::Symbol::WALL);

  if (raycast_result.has_value()) {
	return false;
  }

  // direction check
  float angle_diff_deg = std::abs(player.dir_deg - polar_vector_to_enemy.angle_deg);
  if (angle_diff_deg > attack_angle_deg) {
	return false;
  }

  return true;
}
bool Weapon::try_attack(Player &player, std::vector<Enemy> &enemies) {
  if (!is_unlocked || !attack_timer.check_is_elapsed() || ammo <= 0) {
	return false;
  }
  // run attack even if no enemies are hit
  is_attacking = true;
  ammo -= 1;
  attack_animation.reset_animation();
  SoundManager::get_instance().play_sound(attack_sound_id);

  // check if any enemies are hit
  for (auto &enemy : enemies) {
	if (enemy.is_dead) {
	  continue;
	}
	if (is_enemy_in_attack_range(player, enemy)) {
	  enemy.take_damage(attack_damage);
	}
  }

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
