#include <SFML/Graphics.hpp>
#include <algorithm>
#include "game_loop.hpp"
#include "lines.hpp"
#include "raycasting.hpp"
#include "player.hpp"
#include "draw.hpp"

const int WINDOW_WIDTH = 1024;
const int WINDOW_HEIGHT = 1024;

void run_game(Grid<int> &grid) {
  sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Epitech - Wolf3D");

  // set the mouse cursor to the center of the window
  window.setMouseCursorGrabbed(true);
  // hide the mouse cursor
  window.setMouseCursorVisible(false);

  std::vector<Cube> grid_cubes = get_grid_cubes(grid);

  // load textures (wall, floor)
  sf::Texture wall_texture;
  sf::Texture floor_texture;
  sf::Texture ceiling_texture;
  if (!wall_texture.loadFromFile("./assets/visuals/wall_512.png")) {
    throw std::runtime_error("Could not load wall texture file");
  }
  if (!floor_texture.loadFromFile("./assets/visuals/ground_512.png")) {
    throw std::runtime_error("Could not load floor texture file");
  }
  if (!ceiling_texture.loadFromFile("./assets/visuals/ground_512.png")) {
    throw std::runtime_error("Could not load ceiling texture file");
  }



  // assign textures to sprites
  sf::Sprite wall_sprite;
  sf::Sprite floor_sprite;
  sf::Sprite ceiling_sprite;
  wall_sprite.setTexture(wall_texture);
  floor_sprite.setTexture(floor_texture);
  ceiling_sprite.setTexture(ceiling_texture);

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

    // 3d floor
    draw_floor_and_ceiling_3d(window, grid, grid_cubes, floor_texture, floor_sprite, ceiling_texture, ceiling_sprite);

    // 3d walls
    draw_walls_3d(window,
                  grid,
                  grid_cubes,
                  wall_texture,
                  wall_sprite,
                  floor_texture,
                  floor_sprite,
                  ceiling_texture,
                  ceiling_sprite);

    // minimap
    draw_minimap(window, grid, grid_cubes);

    /*
     * Render
     * */
    window.display();
  }
}