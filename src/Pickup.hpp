#ifndef EPITECH_WOLF3D_PROJECT_SRC_PICKUP_HPP_
#define EPITECH_WOLF3D_PROJECT_SRC_PICKUP_HPP_

#include "SpriteSetting.hpp"
#include "Tile.hpp"

class Player;
class Pickup;

enum class PickupId {
  HEALTH,
  AMMO,
  THOMPSON,
  MACHINE_GUN,
};

struct PickupSetting {
  Tile::Symbol symbol;
  SpriteId sprite_id;
  SoundId pickup_sound_id;
  float value;
};

static std::unordered_map<Tile::Symbol, PickupSetting> PICKUP_SETTINGS = {
    {Tile::Symbol::PICKUP_HEALTH, {
        Tile::Symbol::PICKUP_HEALTH,
        SpriteId::PICKUP_HEALTH,
        SoundId::PICKUP_HEALTH,
        30,
    }},
    {Tile::Symbol::PICKUP_AMMO, {
        Tile::Symbol::PICKUP_AMMO,
        SpriteId::PICKUP_AMMO,
        SoundId::AMMO,
        8,
    }},
    {Tile::Symbol::PICKUP_THOMPSON, {
        Tile::Symbol::PICKUP_THOMPSON,
        SpriteId::PICKUP_THOMPSON,
        SoundId::PICKUP_THOMPSON,
        -1,
    }},
    {Tile::Symbol::PICKUP_MACHINE_GUN, {
        Tile::Symbol::PICKUP_MACHINE_GUN,
        SpriteId::PICKUP_MACHINE_GUN,
        SoundId::PICKUP_MACHINE_GUN,
        -1,
    }},
};

struct PickupDistanceToPlayer {
  std::reference_wrapper<Pickup> pickup_ref;
  float distance;
};

class Pickup {
 public:
  bool is_disabled = false;

 private:
  Tile::Symbol symbol;
  sf::Vector2i tile_coords;
  sf::Sprite sprite;
  SoundId pickup_sound_id;
  float value;
  float tile_size;

 public:
  Pickup(const PickupSetting &setting, const sf::Vector2i &tile_coords, float tile_size);

 public:
  sf::Sprite &get_sprite();
  void update(Player &player);
  sf::Vector2f get_pos() const;

 private:
  bool get_is_player_in_range(Player &player);
  bool on_pickup(Player &player);
};

#endif //EPITECH_WOLF3D_PROJECT_SRC_PICKUP_HPP_
