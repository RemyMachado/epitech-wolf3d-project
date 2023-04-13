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

  Player(sf::Vector2f pos,
		 float direction_deg,
		 float move_speed,
		 Grid &grid,
		 Camera &camera);

  void update_current_weapon_sprite() const;
  void try_attack() const;
  void select_knife();
  void select_pistol();
  void move_forward();
  void move_backward();
  void move_left();
  void move_right();
  void rotate(float direction_deg);

 private:
  void switch_weapon(Weapon *new_weapon);
  void move(float direction_deg);
  static std::unique_ptr<Weapon> create_knife();
  static std::unique_ptr<Weapon> create_pistol();
};

#endif //EPITECH_WOLF3D_PROJECT_SRC_PLAYER_HPP_
