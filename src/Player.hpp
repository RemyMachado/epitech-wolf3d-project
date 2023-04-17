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
  std::unique_ptr<Weapon> pistol;
  Weapon *current_weapon;
  Timer weapon_switch_timer;
  float body_radius = 0.2f;
  float lives = 3;
  float health = 100; // max is 100

  Player(sf::Vector2f pos,
		 float direction_deg,
		 float move_speed,
		 Grid &grid,
		 Camera &camera);

  sf::Sprite &get_hud_face_sprite();
  void update();
  void try_attack() const;
  void take_damage(float damage);
  void select_knife();
  void select_pistol();
  void move_forward();
  void move_backward();
  void move_left();
  void move_right();
  void rotate(float direction_deg);
  sf::Vector2f get_pos() const;
  sf::Vector2f get_dir_vector() const;

 private:
  sf::Sprite hud_face_sprite;

 private:
  int get_hud_face_animation_frame() const;
  void update_hud_face_sprite();
  void update_current_weapon_sprite() const;
  void update_sprites();
  void switch_weapon(Weapon *new_weapon);
  void move(float direction_deg);
};

#endif //EPITECH_WOLF3D_PROJECT_SRC_PLAYER_HPP_
