#ifndef EPITECH_WOLF3D_PROJECT_SRC_ENEMY_HPP_
#define EPITECH_WOLF3D_PROJECT_SRC_ENEMY_HPP_

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include "Animation.hpp"
#include "Tile.hpp"
#include "managers/SoundManager.hpp"
#include "Grid.hpp"
#include "PathFinder.hpp"
#include "my_math.hpp"

class Enemy;
class Player;
class Grid;
class PathFinder;

struct EnemySetting {
  Tile::Symbol symbol;
  AnimationParams walk_animation_params;
  AnimationParams attack_animation_params;
  AnimationParams death_animation_params;
  AnimationParams hurt_animation_params;
  SpriteId idle_sprite_id;
  Range<float> attack_delay_range;
  float attack_tile_range;
  float attack_damage;
  SoundId attack_sound_id;
  SoundId hurt_sound_id;
  SoundId death_sound_id;
  float health;
  Range<float> move_speed_range;
};

struct EnemyDistanceToPlayer {
  std::reference_wrapper<Enemy> enemy_ref;
  float distance;
};

static std::unordered_map<Tile::Symbol, EnemySetting> ENEMY_SETTINGS = {
	{Tile::Symbol::ENEMY_GUARD, {
		Tile::Symbol::ENEMY_GUARD,
		{SPRITE_SETTINGS.at(SpriteId::ENEMY_GUARD_WALK), 0.3f},
		{SPRITE_SETTINGS.at(SpriteId::ENEMY_GUARD_ATTACK), 0.5f},
		{SPRITE_SETTINGS.at(SpriteId::ENEMY_GUARD_DEATH), 0.5f},
		{SPRITE_SETTINGS.at(SpriteId::ENEMY_GUARD_HURT), 0.5f},
		SpriteId::ENEMY_GUARD_IDLE,
		{1.5f, 2.5f},
		10,
		5,
		SoundId::PISTOL_ATTACK,
		SoundId::ENEMY_GUARD_HURTS,
		SoundId::ENEMY_GUARD_DIES,
		50,
		{0.2f, 0.5f}
	}},
	{Tile::Symbol::ENEMY_DOG, {
		Tile::Symbol::ENEMY_DOG,
		{SPRITE_SETTINGS.at(SpriteId::ENEMY_DOG_WALK), 0.3f},
		{SPRITE_SETTINGS.at(SpriteId::ENEMY_DOG_ATTACK), 0.6f},
		{SPRITE_SETTINGS.at(SpriteId::ENEMY_DOG_DEATH), 0.5f},
		{SPRITE_SETTINGS.at(SpriteId::ENEMY_DOG_DEATH), 0.5f}, // Hurt sprite isn't needed, dog always dies on one hit
		SpriteId::ENEMY_DOG_IDLE,
		{2.0f, 3.0f},
		2.0f,
		10,
		SoundId::ENEMY_DOG_BARK,
		SoundId::ENEMY_DOG_DIES, // Hurt sound isn't needed, dog always dies on one hit
		SoundId::ENEMY_DOG_DIES,
		1,
		{0.5f, 1.0f}
	}},
};

class Enemy {
 public:
  sf::Vector2i tile_coords;
  sf::Vector2f pos;
  Tile::Symbol symbol;
  float attack_delay;
  Timer attack_timer;
  bool is_walking = false;
  bool is_attacking = false;
  bool is_dying = false;
  bool is_hurting = false;
  bool is_dead = false;
  float health;
  float move_speed;

 private:
  sf::Vector2f walk_delta_per_frame;
  const Grid &grid;
  float attack_tile_range;
  float attack_damage;
  /* Sound */
  SoundId attack_sound_id;
  SoundId hurt_sound_id;
  SoundId death_sound_id;

  /* Animation */
  sf::Sprite idle_sprite;
  Animation walk_animation;
  Animation attack_animation;
  Animation hurt_animation;
  Animation death_animation;

 public:
  explicit Enemy(const EnemySetting &setting, const Grid &grid, sf::Vector2i initial_coords, sf::Vector2f initial_pos);
  ~Enemy();

 public:
  sf::Sprite &get_current_sprite();
  void update(Player &player, PathFinder &path_finder);
  void take_damage(float damage);

  // print operator
  friend std::ostream &operator<<(std::ostream &os, const Enemy &enemy);

 private:
  void move(sf::Vector2f new_pos);
  void try_attack(Player &player);
  void try_move(Player &player, PathFinder &path_finder);
  bool is_player_in_range(const Player &player);
  void update_sprites();
  void update_walk_animation();
  void update_attack_animation();
  void update_death_animation();
  void update_hurt_animation();
  void update_idle_animation();
  void update_transitory_walk_position();
  void end_walk_anim();
  void end_attack_anim();
  void end_death_anim();
  void end_hurt_anim();
  bool get_is_busy() const;
  void die();
};

#endif //EPITECH_WOLF3D_PROJECT_SRC_ENEMY_HPP_
