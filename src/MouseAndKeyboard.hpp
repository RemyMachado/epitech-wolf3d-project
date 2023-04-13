#ifndef EPITECH_WOLF3D_PROJECT_SRC_MOUSEANDKEYBOARD_HPP_
#define EPITECH_WOLF3D_PROJECT_SRC_MOUSEANDKEYBOARD_HPP_

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>
#include <SFML/Graphics.hpp>

class Player;

class MouseAndKeyboard {
 public:
  bool recenter_mouse = false;
  float mouse_rotation_speed_deg = 0.02f;
  float keyboard_rotation_speed_deg = 5;

  MouseAndKeyboard() = default;

  static void handle_user_inputs(sf::RenderWindow &window, Player &player);
  void handle_user_inputs_event_based(Player &player, sf::RenderWindow &window, sf::Event &event);

 private:
  static void handle_player_attack(Player &player);
  static void handle_player_movement(Player &player);
  void handle_player_rotation(Player &player,
							  sf::RenderWindow &window,
							  sf::Event &event);
};

#endif //EPITECH_WOLF3D_PROJECT_SRC_MOUSEANDKEYBOARD_HPP_
