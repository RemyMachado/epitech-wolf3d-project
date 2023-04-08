#include <SFML/Graphics.hpp>
#include <algorithm>
#include <cmath>
#include "game_loop.hpp"
#include "lines.hpp"
#include "raycasting.hpp"
#include "player.hpp"
#include "draw.hpp"

const int WINDOW_WIDTH = 1920;
const int WINDOW_HEIGHT = 1080;
const int TEXTURE_WIDTH = 64;
const int TEXTURE_HEIGHT = 64;

void run_game(Grid<int> &grid) {
  sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Epitech - Wolf3D");

  // set the mouse cursor to the center of the window
  window.setMouseCursorGrabbed(true);
  // hide the mouse cursor
  window.setMouseCursorVisible(false);

  std::vector<Cube> grid_cubes = get_grid_cubes(grid);

  // load wall texture
  sf::Texture wall_texture;
  if (!wall_texture.loadFromFile("./assets/visuals/wall_512.png")) {
    throw std::runtime_error("Could not load wall texture file");
  }

//   assign wall texture to sprite
  sf::Sprite wall_sprite;
  wall_sprite.setTexture(wall_texture);

  while (window.isOpen()) {
    /*
     * Handle user inputs
     * */
    handle_player_movement(grid, grid_cubes);

    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed ||
          (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)) {
        window.close();
      }
      handle_player_camera_rotation(grid, window, event);
    }

    /*
     * Clear the window for drawing
     * */
    window.clear();

    /*
     * Draw
     * */

    // 3d walls
    draw_walls_3d(window, grid, grid_cubes, wall_texture, wall_sprite);

    // minimap
    draw_minimap(window, grid, grid_cubes);

    /*
     * Render
     * */
    window.display();
  }
}