#include "raycasting.hpp"
#include <iostream>
#include <chrono>
#include <SFML/Graphics.hpp>
#include "game_loop.hpp"
#include "draw.hpp"
#include "GameManager.hpp"

void run_game(GameManager &game_manager) {
  int raycast_thickness = 6;

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

  while (game_manager.window.isOpen()) {
	auto start_loop = std::chrono::high_resolution_clock::now();
	std::vector<ComputedDrawHit> raycast_vec;

	raycast_vec = compute_floor_raycast_vec(raycast_vec,
											game_manager,
											game_manager.window.getSize().x,
											game_manager.window.getSize().y,
											raycast_thickness);

	int raycast_floor_size = raycast_vec.size();
	raycast_vec = compute_walls_raycast_vec(raycast_vec,
											game_manager,
											game_manager.window.getSize().x,
											game_manager.window.getSize().y,
											raycast_thickness);
	int raycast_wall_size = raycast_vec.size() - raycast_floor_size;

	// cout the sizes
	std::cout << "raycast_floor_size: " << raycast_floor_size << std::endl;
	std::cout << "raycast_wall_size: " << raycast_wall_size << std::endl;
	std::cout << "total size: " << raycast_vec.size() << std::endl;

	/*
	* Handle user inputs
	* */
	game_manager.mouse_and_keyboard.handle_user_inputs(game_manager.player);

	sf::Event event;
	while (game_manager.window.pollEvent(event)) {
	  if (event.type == sf::Event::Closed ||
		  (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)) {
		game_manager.window.close();
	  }

	  game_manager.mouse_and_keyboard.handle_user_inputs_event_based(game_manager.player, game_manager.window, event);
	}

	/*
	 * Clear the window for drawing
	 * */
	game_manager.window.clear();

	/*
	 * Draw
	 * */

	// 3d floor
	auto start_floor = std::chrono::high_resolution_clock::now();
	draw_raycast_map(raycast_vec,
					 game_manager.window,
					 raycast_thickness,
					 wall_texture,
					 wall_sprite,
					 floor_texture,
					 floor_sprite,
					 ceiling_texture,
					 ceiling_sprite);
/*
    draw_floor_and_ceiling_3d(game_manager,
                              floor_texture,
                              floor_sprite,
                              ceiling_texture,
                              ceiling_sprite);
*/
	// Stop the timer
	auto end_floor = std::chrono::high_resolution_clock::now();

	// Calculate the duration
	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_floor - start_floor).count();
	std::cout << "--------------------------------------------" << std::endl << "Draw floor execution time: "
			  << duration << " ms" << std::endl;

	// 3d walls
	auto start_walls = std::chrono::high_resolution_clock::now();
/*
    draw_walls_3d(game_manager, wall_texture, wall_sprite);
*/
	// Stop the timer
	auto end_walls = std::chrono::high_resolution_clock::now();

	// Calculate the duration
	duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_walls - start_walls).count();
	std::cout << "Draw walls execution time: " << duration << " ms" << std::endl;

	// minimap
	auto start_minimap = std::chrono::high_resolution_clock::now();
	draw_minimap(game_manager);
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
	game_manager.window.display();
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