#ifndef EPITECH_WOLF3D_PROJECT_SRC_PLAYER_HPP_
#define EPITECH_WOLF3D_PROJECT_SRC_PLAYER_HPP_

#include <SFML/System/Vector2.hpp>
#include "Weapon.hpp"

class Grid;
class Camera;

class Player {
 public:
  sf::Vector2f pos;
  float dir_deg;
  float move_speed;
  Grid &grid;
  Camera &camera;
  std::unique_ptr<Weapon> knife;

  Player(sf::Vector2f pos,
		 float direction_deg,
		 float move_speed,
		 Grid &grid,
		 Camera &camera)
	  : pos(pos),
		dir_deg(direction_deg),
		move_speed(move_speed),
		grid(grid),
		camera(camera) {
	float attack_rate = 0.2f;
	SpriteSetting knife_attack_sprite_setting = SPRITE_SETTINGS.at(SpriteId::KNIFE_ATTACK);
	AnimationParams knife_attack_animation_params = AnimationParams(
		{
			TextureManager::get_instance().get_texture(
				SpriteId::KNIFE_ATTACK),
			knife_attack_sprite_setting.frame_size,
			knife_attack_sprite_setting.frame_count,
			knife_attack_sprite_setting.initial_offset,
			knife_attack_sprite_setting.frame_offset,
			attack_rate / knife_attack_sprite_setting.frame_count
		}
	);
	WeaponParams knife_params = WeaponParams(
		{
			knife_attack_animation_params,
			SpriteId::KNIFE_HUD,
			SpriteId::KNIFE_IDLE,
			attack_rate
		});
	knife = std::make_unique<Weapon>(knife_params);
  };

  void try_attack() const;
  void move_forward();
  void move_backward();
  void move_left();
  void move_right();
  void rotate(float direction_deg);

 private:
  void move(float direction_deg);
};

#endif //EPITECH_WOLF3D_PROJECT_SRC_PLAYER_HPP_
