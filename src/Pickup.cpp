#include "Player.hpp"
#include "Pickup.hpp"
#include "managers/GameManager.hpp"

Pickup::Pickup(const PickupSetting &setting, const sf::Vector2i &tile_coords, float tile_size)
    : symbol(setting.symbol),
      tile_coords(tile_coords),
      sprite(TextureManager::get_instance().get_texture(setting.sprite_id)),
      pickup_sound_id(setting.pickup_sound_id),
      value(setting.value),
      tile_size(tile_size) {
  SpriteSetting sprite_setting = SPRITE_SETTINGS.at(setting.sprite_id);
  sprite.setTextureRect(sf::IntRect(
      sprite_setting.initial_offset.x,
      sprite_setting.initial_offset.y,
      sprite_setting.frame_size.x,
      sprite_setting.frame_size.y));
}

bool Pickup::on_pickup(Player &player, GameManager &game_manager) {
  switch (symbol) {
    case Tile::Symbol::PICKUP_HEALTH: {
      if (player.health == 100.0f) {
        return false;
      }
      player.health = std::min(player.health + value, 100.0f);
      SoundManager::get_instance().play_sound(pickup_sound_id);
      break;
    }
    case Tile::Symbol::PICKUP_AMMO: {
      player.pistol->ammo += value;
      player.thompson->ammo += value;
      player.machine_gun->ammo += value;
      if (!player.pistol->is_unlocked) {
        player.pistol->is_unlocked = true;
        player.select_pistol();
      }
      SoundManager::get_instance().play_sound(pickup_sound_id);
      break;
    }
    case Tile::Symbol::PICKUP_THOMPSON: {
      player.thompson->is_unlocked = true;
      player.select_thompson();
      SoundManager::get_instance().play_sound(pickup_sound_id);
      break;
    }
    case Tile::Symbol::PICKUP_MACHINE_GUN: {
      player.machine_gun->is_unlocked = true;
      player.select_machine_gun();
      SoundManager::get_instance().play_sound(pickup_sound_id);
      break;
    }
    case Tile::Symbol::PICKUP_WINNING_KEY: {
      game_manager.is_game_won = true;
      SoundManager::get_instance().play_sound(pickup_sound_id);
      break;
    }
  }

  return true;
}
void Pickup::update(Player &player, GameManager &game_manager) {
  if (!is_disabled && get_is_player_in_range(player)) {
    bool did_pickup = on_pickup(player, game_manager);
    is_disabled = did_pickup;
  }
}
bool Pickup::get_is_player_in_range(Player &player) {
  sf::Vector2i player_tile_coords = player.get_tile_coords();

  return player_tile_coords == tile_coords;
}
sf::Sprite &Pickup::get_sprite() {
  return sprite;
}
sf::Vector2f Pickup::get_pos() const {
  return {(float) tile_coords.x * tile_size + tile_size / 2, (float) tile_coords.y * tile_size + tile_size / 2};
}
