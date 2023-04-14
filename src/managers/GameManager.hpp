#ifndef EPITECH_WOLF3D_PROJECT_SRC_GAMEMANAGER_HPP_
#define EPITECH_WOLF3D_PROJECT_SRC_GAMEMANAGER_HPP_

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include "../Grid.hpp"
#include "../Player.hpp"
#include "../Camera.hpp"
#include "../Hud.hpp"
#include "../MouseAndKeyboard.hpp"
#include "../my_math.hpp"

class GameManager {
 public:
  sf::RenderWindow window;
  Grid grid;
  Player player;
  Camera camera;
  Hud hud;
  MouseAndKeyboard mouse_and_keyboard;

  GameManager(char *filename, float tile_size, sf::Vector2i screen_size) :
	  window(sf::VideoMode(screen_size.x,
						   screen_size.y),
			 "Epitech - Wolf3D"),
	  grid(parse_file(filename, tile_size), tile_size),
	  camera(grid.tile_size / 2,
			 get_magnitude({static_cast<float>(grid.width),
							static_cast<float>(grid.height)})
				 * grid.tile_size),
	  player(grid.get_player_initial_pos(),
			 0,
			 grid.tile_size * 0.05f, grid, camera),
	  hud(),
	  mouse_and_keyboard() {
	// set the mouse cursor to the center of the window
	window.setMouseCursorGrabbed(true);
	// hide the mouse cursor
	window.setMouseCursorVisible(false);
  }
};

#endif //EPITECH_WOLF3D_PROJECT_SRC_GAMEMANAGER_HPP_
