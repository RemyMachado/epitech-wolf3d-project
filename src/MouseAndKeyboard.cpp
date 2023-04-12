#include "MouseAndKeyboard.hpp"
#include "Player.hpp"

void MouseAndKeyboard::handle_player_movement(Player &player) {
  // allowing multiple keys to be pressed at once
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
	player.move_left();
  }
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
	player.move_right();
  }
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
	player.move_forward();
  }
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
	player.move_backward();
  }
}

void MouseAndKeyboard::handle_player_rotation(Player &player,
											  sf::RenderWindow &window,
											  sf::Event &event) {
  /*
   * Mouse rotation
   * */
  if (event.type == sf::Event::MouseMoved) {
	if (!this->recenter_mouse) {
	  sf::Vector2i current_mouse_position = sf::Mouse::getPosition(window);
	  sf::Vector2i window_center((int)window.getSize().x / 2, (int)window.getSize().y / 2);
	  int mouse_delta_x = current_mouse_position.x - window_center.x;

	  player.rotate((float)mouse_delta_x * this->mouse_rotation_speed_deg);

	  // Set the mouse position to the center of the window
	  sf::Mouse::setPosition(window_center, window);

	  // Enable recenter_mouse to avoid updating grid.player_direction_deg while re-centering
	  this->recenter_mouse = true;
	} else {
	  // Reset recenter_mouse for the next mouse movement event
	  this->recenter_mouse = false;
	}
  }

  /*
   * Keyboard rotation
   * */
  if (event.type == sf::Event::KeyPressed) {
	if (event.key.code == sf::Keyboard::Left) {
	  player.rotate(-this->keyboard_rotation_speed_deg);
	}
	if (event.key.code == sf::Keyboard::Right) {
	  player.rotate(this->keyboard_rotation_speed_deg);
	}
  }
}

void move_window_position(sf::RenderWindow &window, int moveSpeed) {
  // Move the window based on arrow key input
  sf::Vector2i currentPosition = window.getPosition();
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::J)) {
	currentPosition.x -= moveSpeed;
  }
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::L)) {
	currentPosition.x += moveSpeed;
  }
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::I)) {
	currentPosition.y -= moveSpeed;
  }
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::K)) {
	currentPosition.y += moveSpeed;
  }

  window.setPosition(currentPosition);
}

void MouseAndKeyboard::handle_user_inputs(sf::RenderWindow &window, Player &player) {
  handle_player_movement(player);
  move_window_position(window, 50);
}

void MouseAndKeyboard::handle_user_inputs_event_based(Player &player, sf::RenderWindow &window, sf::Event &event) {
  handle_player_rotation(player, window, event);
}
