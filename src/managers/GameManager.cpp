#include "GameManager.hpp"

GameManager::GameManager(char *filename, float tile_size, sf::Vector2i screen_size) :
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
	mouse_and_keyboard(),
	enemies(grid.get_initial_enemies()) {
  // set the mouse cursor to the center of the window
  window.setMouseCursorGrabbed(true);
  // hide the mouse cursor
  window.setMouseCursorVisible(false);
}
void GameManager::update() {
  // Update player and enemy states.
  player.update();
  for (Enemy &enemy : enemies) {
	enemy.update(player);
  }
}
