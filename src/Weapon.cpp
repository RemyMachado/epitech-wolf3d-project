#include <algorithm>
#include "Weapon.hpp"
#include "Enemy.hpp"
#include "Player.hpp"
#include "my_math.hpp"
#include "raycasting.hpp"

Weapon::Weapon(const WeaponSetting &weapon_setting)
    : id(weapon_setting.id),
      is_unlocked(weapon_setting.is_unlocked),
      attack_animation(weapon_setting.attack_animation_params, {[this]() { end_attack(); }}),
      attack_delay(weapon_setting.attack_delay),
      attack_timer(weapon_setting.attack_delay),
      attack_sound_id(weapon_setting.attack_sound_id),
      ammo(weapon_setting.initial_ammo),
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
              UNWALKABLE_TILES);

  if (raycast_result.has_value()) {
    return false;
  }

  // direction check
  float angle_diff_deg = std::abs(
      positive_angle_deg(player.dir_deg) - positive_angle_deg(polar_vector_to_enemy.angle_deg));
  if (angle_diff_deg > attack_angle_deg) {
    return false;
  }

  return true;
}
bool Weapon::try_attack(Player &player, std::vector<Enemy> &enemies) {
  std::vector<unsigned int> enemies_index_in_range;

  if (!is_unlocked || !attack_timer.check_is_elapsed() || ammo <= 0) {
    return false;
  }
  // run attack even if no enemies are hit
  is_attacking = true;
  ammo -= 1;
  attack_animation.reset_animation();
  SoundManager::get_instance().play_sound(attack_sound_id);

  // Try to break doors
  if (id == WeaponId::KNIFE) {
    // raycast in the direction of the player
    auto raycast_result =
        raycast(player.pos,
                player.dir_deg,
                attack_tile_range * player.grid.tile_size,
                player.grid,
                UNWALKABLE_TILES);

    if (raycast_result.has_value() && raycast_result.value().tile.symbol == Tile::DOOR) {
      SoundManager::get_instance().play_sound(SoundId::DOOR_BREAK);
      player.grid.set_tile_symbol(raycast_result.value().tile.coords, Tile::FLOOR);
      return true;
    }
  }

  // check if any enemies are in range
  for (unsigned int i = 0; i < enemies.size(); i++) {
    if (enemies[i].is_dead) {
      continue;
    }
    if (is_enemy_in_attack_range(player, enemies[i])) {
      enemies_index_in_range.emplace_back(i);
    }
  }

  // attack only the closest enemies
  unsigned int closest_enemy_index = -1;
  if (!enemies_index_in_range.empty()) {
    // can't sort players with std::sort due to reference errors with enemies objects getting destroyed unexpectedly
    // So we check distance by hand using indices

    for (const unsigned int enemy_index_in_range : enemies_index_in_range) {
      if (closest_enemy_index == -1) {
        closest_enemy_index = enemy_index_in_range;
        continue;
      }
      float distance_to_cur_enemy_in_range =
          get_distance_between_points(player.pos, enemies[enemy_index_in_range].pos);
      float distance_to_closest_enemy =
          get_distance_between_points(player.pos, enemies[closest_enemy_index].pos);
      if (distance_to_cur_enemy_in_range <
          distance_to_closest_enemy) {
        closest_enemy_index = enemy_index_in_range;
      }
    }

    enemies[closest_enemy_index].take_damage(attack_damage, player);
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
