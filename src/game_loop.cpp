#include <SFML/Graphics.hpp>
#include <chrono>
#include "game_loop.hpp"
#include "draw.hpp"
#include "managers/GameManager.hpp"

void run_game(GameManager &game_manager) {
  // load textures (wall, floor)
  sf::Texture wall_texture = TextureManager::get_instance().get_texture(SpriteId::WALL);
  sf::Texture wall_shadow_texture = TextureManager::get_instance().get_texture(SpriteId::WALL_SHADOW);
  sf::Texture floor_texture = TextureManager::get_instance().get_texture(SpriteId::FLOOR);
  sf::Texture ceiling_texture = TextureManager::get_instance().get_texture(SpriteId::CEILING);
  sf::Texture hud_empty_texture = TextureManager::get_instance().get_texture(SpriteId::HUD_EMPTY);

  game_manager.hud.bar_width = game_manager.window.getSize().x;
  game_manager.hud.bar_height =
	  hud_empty_texture.getSize().y * game_manager.window.getSize().x / hud_empty_texture.getSize().x;

  game_manager.hud.scale_factor =
	  (float)game_manager.hud.bar_height
		  / (float)TextureManager::get_instance().get_texture(SpriteId::HUD_EMPTY).getSize().y;

  // assign textures to sprites
  sf::Sprite wall_sprite;
  sf::Sprite wall_shadow_sprite;
  sf::Sprite floor_sprite;
  sf::Sprite ceiling_sprite;
  sf::Sprite hud_empty_sprite;
  wall_sprite.setTexture(wall_texture);
  wall_shadow_sprite.setTexture(wall_shadow_texture);
  floor_sprite.setTexture(floor_texture);
  ceiling_sprite.setTexture(ceiling_texture);
  hud_empty_sprite.setTexture(hud_empty_texture);

  while (game_manager.window.isOpen()) {
	game_manager.render_loop_count++;
	auto start_loop = std::chrono::high_resolution_clock::now();

	/*
	* Handle user inputs
	* */
	game_manager.mouse_and_keyboard.handle_user_inputs(game_manager.window, game_manager.player);

	sf::Event event;
	while (game_manager.window.pollEvent(event)) {
	  if (event.type == sf::Event::Closed ||
		  (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)) {
		game_manager.window.close();
	  }

	  game_manager.mouse_and_keyboard.handle_user_inputs_event_based(game_manager.player, game_manager.window, event);
	}

	render_game_frame(game_manager,
					  wall_sprite,
					  wall_shadow_sprite,
					  floor_sprite,
					  ceiling_sprite,
					  hud_empty_sprite);

	// Stop the frame_timer
	auto end_loop = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_loop - start_loop).count();
//	std::cout << "Total loop execution time: " << duration << " ms" << std::endl << std::endl;
  }
}