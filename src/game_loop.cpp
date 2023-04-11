#include <iostream>
#include <chrono>
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
    auto start_loop = std::chrono::high_resolution_clock::now();
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
    auto start_floor = std::chrono::high_resolution_clock::now();
    draw_floor_and_ceiling_3d(window, grid, grid_cubes, floor_texture, floor_sprite, ceiling_texture, ceiling_sprite);
    // Stop the timer
    auto end_floor = std::chrono::high_resolution_clock::now();

    // Calculate the duration
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_floor - start_floor).count();
    std::cout << "--------------------------------------------" << std::endl << "Draw floor execution time: "
              << duration << " ms" << std::endl;

    // 3d walls
    auto start_walls = std::chrono::high_resolution_clock::now();
    draw_walls_3d(window,
                  grid,
                  grid_cubes,
                  wall_texture,
                  wall_sprite);
    // Stop the timer
    auto end_walls = std::chrono::high_resolution_clock::now();

    // Calculate the duration
    duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_walls - start_walls).count();
    std::cout << "Draw walls execution time: " << duration << " ms" << std::endl;

    // minimap
    auto start_minimap = std::chrono::high_resolution_clock::now();
    draw_minimap(window, grid, grid_cubes);
    // Stop the timer
    auto end_minimap = std::chrono::high_resolution_clock::now();

    // Calculate the duration
    duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_minimap - start_minimap).count();
    std::cout << "Draw minimap execution time: " << duration << " ms" << std::endl;

    // draw hud
//    draw_hud(window, grid);

    /*
     * Render
     * */
    auto start_display = std::chrono::high_resolution_clock::now();
    window.display();
    auto end_display = std::chrono::high_resolution_clock::now();

    // Calculate the duration
    duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_display - start_display).count();
    std::cout << "Render Display() execution time: " << duration << " ms" << std::endl;

    // Stop the timer
    auto end_loop = std::chrono::high_resolution_clock::now();

    // Calculate the duration
    duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_loop - start_loop).count();
    std::cout << "Total loop execution time: " << duration << " ms" << std::endl << std::endl;
  }
}