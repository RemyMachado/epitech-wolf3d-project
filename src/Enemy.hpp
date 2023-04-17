#ifndef EPITECH_WOLF3D_PROJECT_SRC_ENEMY_HPP_
#define EPITECH_WOLF3D_PROJECT_SRC_ENEMY_HPP_

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include "Animation.hpp"
#include "Tile.hpp"
#include "managers/SoundManager.hpp"
#include "Grid.hpp"

class Enemy;
class Player;
class Grid;

struct EnemySetting {
  Tile::Symbol symbol;
  AnimationParams attack_animation_params;
  SpriteId idle_sprite_id;
  float attack_delay;
  float max_additional_random_attack_delay;
  float attack_tile_range;
  float attack_damage;
  SoundId attack_sound_id;
};

struct EnemyDistanceToPlayer {
  std::reference_wrapper<Enemy> enemy_ref;
  float distance;
};

static std::unordered_map<Tile::Symbol, EnemySetting> ENEMY_SETTINGS = {
	{Tile::Symbol::ENEMY_GUARD, {
		Tile::Symbol::ENEMY_GUARD,
		{SPRITE_SETTINGS.at(SpriteId::ENEMY_GUARD_ATTACK), 0.5f},
		SpriteId::ENEMY_GUARD_IDLE,
		1.5f,
		0.5f,
		10,
		5,
		SoundId::PISTOL_ATTACK
	}},
	{Tile::Symbol::ENEMY_DOG, {
		Tile::Symbol::ENEMY_DOG,
		{SPRITE_SETTINGS.at(SpriteId::ENEMY_DOG_ATTACK), 0.6f},
		SpriteId::ENEMY_DOG_IDLE,
		2.0f,
		0.5f,
		1,
		10,
		SoundId::ENEMY_DOG_BARK
	}},
};

class Enemy {
 public:
  sf::Vector2i coords;
  sf::Vector2f pos;
  Tile::Symbol symbol;
  float attack_delay;
  float additional_random_attack_delay;
  Timer attack_timer;
  bool is_attacking = false;

 private:
  const Grid &grid;
  float attack_tile_range;
  float attack_damage;
  /* Sound */
  SoundId attack_sound_id;

  /* Animation */
  sf::Sprite idle_sprite;
  Animation attack_animation;

 public:
  explicit Enemy(const EnemySetting &setting, const Grid &grid, sf::Vector2i initial_coords, sf::Vector2f initial_pos);

  sf::Sprite &get_current_sprite();
  void update(Player &player);

  // print operator
  friend std::ostream &operator<<(std::ostream &os, const Enemy &enemy);

 private:
  void try_attack(Player &player);
  bool is_player_in_range(const Player &player);
  void update_sprites();
  void update_attack_animation();
  void update_idle_animation();
  void end_attack();
};

#endif //EPITECH_WOLF3D_PROJECT_SRC_ENEMY_HPP_
