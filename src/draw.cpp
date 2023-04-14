#include "managers/GameManager.hpp"
#include "draw.hpp"
#include "raycasting.hpp"

void draw_player_on_minimap(GameManager &game_manager) {
  // the minimap height should respect the window height ratio
  float minimap_height = (float)game_manager.window.getSize().y * game_manager.hud.minimap_to_height_ratio;
  float minimap_tile_size = minimap_height / (float)game_manager.grid.height;

  float circle_radius = minimap_tile_size / 2;
  sf::CircleShape circleShape(circle_radius);
  circleShape.setFillColor(sf::Color::Red);

  circleShape.setPosition(game_manager.player.pos.x / game_manager.grid.tile_size * minimap_tile_size - circle_radius,
						  game_manager.player.pos.y / game_manager.grid.tile_size * minimap_tile_size - circle_radius);
  game_manager.window.draw(circleShape);
}

void draw_minimap_grid(GameManager &game_manager) {
  // the minimap height should respect the window height ratio
  float minimap_height = (float)game_manager.window.getSize().y * game_manager.hud.minimap_to_height_ratio;
  float minimap_tile_size = minimap_height / (float)game_manager.grid.height;

  // draw background
  sf::RectangleShape rectangle(sf::Vector2f((float) game_manager.grid.width * minimap_tile_size,
	  (float)game_manager.grid.height * minimap_tile_size));
  rectangle.setFillColor(sf::Color::Black);
  rectangle.setPosition(0, 0);
  game_manager.window.draw(rectangle);

  // draw walls on the grid
  for (const Tile &tile : game_manager.grid.tiles) {
	if (tile.symbol != Tile::Symbol::WALL) {
	  continue;
	}

	for (const Line &segment : tile.horizontal_segments) {
	  sf::Vertex line[] = {
		  sf::Vertex(sf::Vector2f((segment.start.x / game_manager.grid.tile_size * minimap_tile_size) + 1,
								  (segment.start.y / game_manager.grid.tile_size * minimap_tile_size) + 1)),
		  sf::Vertex(sf::Vector2f((segment.end.x / game_manager.grid.tile_size * minimap_tile_size) + 1,
								  (segment.end.y / game_manager.grid.tile_size * minimap_tile_size) + 1))
	  };
	  game_manager.window.draw(line, 2, sf::Lines);
	}
  }
}

// raycast from the player position in the player direction
// and draw a line from the player position to the intersection point
void draw_player_direction_minimap(GameManager &game_manager) {
  // the minimap height should respect the window height ratio
  float minimap_height = (float)game_manager.window.getSize().y * game_manager.hud.minimap_to_height_ratio;
  float minimap_tile_size = minimap_height / (float)game_manager.grid.height;

  std::optional<Raycast>
	  raycast_wall =
	  raycast(game_manager.player.pos,
			  game_manager.player.dir_deg,
			  game_manager.camera.render_distance,
			  game_manager.grid,
			  Tile::Symbol::WALL);
  if (raycast_wall.has_value()) {
	sf::Vertex line[] = {
		sf::Vertex(sf::Vector2f(game_manager.player.pos.x / game_manager.grid.tile_size * minimap_tile_size,
								game_manager.player.pos.y / game_manager.grid.tile_size * minimap_tile_size)),
		sf::Vertex(raycast_wall.value().intersection_pos / game_manager.grid.tile_size * minimap_tile_size)
	};
	game_manager.window.draw(line, 2, sf::Lines);
  }
}

void draw_minimap(GameManager &game_manager) {
  // draw grid
  draw_minimap_grid(game_manager);

  // draw player direction
  draw_player_direction_minimap(game_manager);

  // draw player
  draw_player_on_minimap(game_manager);
}

void draw_floor_and_ceiling_3d(GameManager &game_manager,
							   sf::Texture &floor_texture,
							   sf::Sprite &floor_sprite,
							   sf::Texture &ceiling_texture,
							   sf::Sprite &ceiling_sprite) {
  int render_width = (int)game_manager.window.getSize().x;
  int render_height = (int)game_manager.window.getSize().y - game_manager.hud.bar_height;
  int half_render_height = render_height / 2;
  int ray_thickness = 4;

  // for each pixel in height, cast a ray and draw a horizontal line
  for (int window_y = half_render_height; window_y < render_height; window_y += ray_thickness) {
	// calculate the angle of the ray
	float vertical_camera_rotation_step_deg = game_manager.camera.fov_vertical_deg / (float)render_height;
	float vertical_ray_angle_deg =
		vertical_camera_rotation_step_deg * (float)window_y - game_manager.camera.fov_vertical_deg / 2;

	// for each pixel of the horizontal line cast an imaginary ray using trigonometry
	for (int window_x = 0; window_x < render_width; window_x += ray_thickness) {
	  // calculate the angle of the ray
	  float horizontal_camera_rotation_step_deg = game_manager.camera.fov_horizontal_deg / (float)render_width;
	  float
		  horizontal_ray_angle_deg = game_manager.player.dir_deg - game_manager.camera.fov_horizontal_deg / 2
		  + horizontal_camera_rotation_step_deg * (float)window_x;

	  // compute distance to the point above the floor intersection
	  // <!> this is not the distance to the floor intersection
	  // <!> care for division by zero (tangent)
	  // tangent(theta) = opposite / adjacent
	  // adjacent is distance_to_point_above_floor
	  // opposite is the camera height
	  // camera_height / tangent(theta) = distance_to_point_above_floor
	  // if tangent is equal to zero, then the distance is infinite, replace it with maximum render distance
	  float distance_to_point_above_floor;
	  float tangent_vertical_ray = tanf(degrees_to_radians(vertical_ray_angle_deg));

	  if (tangent_vertical_ray == 0) {
		distance_to_point_above_floor = game_manager.camera.render_distance;
	  } else {
		distance_to_point_above_floor = game_manager.camera.height / tangent_vertical_ray;
	  }

	  // find floor coordinates by going in horizontal direction for the found distance
	  sf::Vector2f floor_intersection_pos = polar_to_cartesian(game_manager.player.pos,
															   distance_to_point_above_floor,
															   horizontal_ray_angle_deg);

	  if (floor_intersection_pos.x<0 || floor_intersection_pos.x>(float)
		game_manager.grid.width * game_manager.grid.tile_size ||
			floor_intersection_pos.y<0 || floor_intersection_pos.y>(float)
	  game_manager.grid.height * game_manager.grid.tile_size) {
		continue;
	  }

	  // find the texture coordinates
	  sf::Vector2f local_intersection =
		  floor_intersection_pos - sf::Vector2f(
			  floor(floor_intersection_pos.x / game_manager.grid.tile_size) * game_manager.grid.tile_size,
			  floor(floor_intersection_pos.y / game_manager.grid.tile_size) * game_manager.grid.tile_size);
	  sf::Vector2f texture_coordinates =
		  sf::Vector2f(local_intersection.x / game_manager.grid.tile_size * floor_texture.getSize().x,
					   local_intersection.y / game_manager.grid.tile_size
						   * floor_texture.getSize().y);

	  // draw the floor
	  floor_sprite.setPosition((float)window_x, (float)window_y);
	  floor_sprite.setTextureRect(sf::IntRect((int)texture_coordinates.x,
											  (int)texture_coordinates.y,
											  ray_thickness,
											  ray_thickness));
	  game_manager.window.draw(floor_sprite);

	  // draw the ceiling
	  /*ceiling_sprite.setPosition((float) window_x, render_height - (float) window_y);
	  ceiling_sprite.setTextureRect(sf::IntRect((int) texture_coordinates.x,
												(int) texture_coordinates.y,
												ray_thickness,
												ray_thickness));
	  game_manager.window.draw(ceiling_sprite);*/
	}
  }
}

void draw_walls_3d(GameManager &game_manager,
				   sf::Texture &wall_texture,
				   sf::Sprite &wall_sprite) {
  int render_width = (int)game_manager.window.getSize().x;
  int render_height = (int)game_manager.window.getSize().y - game_manager.hud.bar_height;
  int ray_thickness = 6;

  // for each pixel in width, cast a ray and draw a vertical line
  for (int window_x = 0; window_x < render_width; window_x += ray_thickness) {
	// calculate the angle of the ray
	float rotation_step_deg = game_manager.camera.fov_horizontal_deg / render_width;
	float
		ray_angle_deg =
		game_manager.player.dir_deg - game_manager.camera.fov_horizontal_deg / 2 + rotation_step_deg * (float)window_x;
	float ray_angle_diff_deg = ray_angle_deg - game_manager.player.dir_deg;
	float fish_eye_correction = cosf(degrees_to_radians(ray_angle_diff_deg));

	// cast the ray
	std::optional<Raycast>
		raycast_wall = raycast(game_manager.player.pos,
							   ray_angle_deg,
							   game_manager.camera.render_distance,
							   game_manager.grid,
							   Tile::Symbol::WALL);
	if (raycast_wall.has_value()) {
	  float raycast_distance_corrected = raycast_wall.value().distance * fish_eye_correction;

	  // the height to draw
	  float column_height = (float)render_height * (game_manager.grid.tile_size / raycast_distance_corrected);
	  float column_render_height =
		  std::fmin((float)render_height * (game_manager.grid.tile_size / raycast_distance_corrected), 10000);
//	  std::cout << "column_height: " << column_render_height << std::endl;

	  // calculate the position of the window_x pixel in the texture using hit side
	  float texture_pixel_x = 0;

	  if (raycast_wall.value().hit_side == Tile::Side::SOUTH) {
		texture_pixel_x =
			raycast_wall.value().local_intersection.x / game_manager.grid.tile_size * wall_texture.getSize().x;
	  } else if (raycast_wall.value().hit_side == Tile::Side::NORTH) {
		texture_pixel_x =
			(1 - raycast_wall.value().local_intersection.x / game_manager.grid.tile_size) * wall_texture.getSize().x;
	  } else if (raycast_wall.value().hit_side == Tile::Side::WEST) {
		texture_pixel_x =
			raycast_wall.value().local_intersection.y / game_manager.grid.tile_size * wall_texture.getSize().x;
	  } else if (raycast_wall.value().hit_side == Tile::Side::EAST) {
		texture_pixel_x =
			(1 - raycast_wall.value().local_intersection.y / game_manager.grid.tile_size) * wall_texture.getSize().x;
	  }

	  // for each pixel in height, draw the corresponding pixel of the texture
	  for (int y = 0; y < column_render_height; y += ray_thickness) {
		// calculate the position of the y pixel in the texture
		float texture_pixel_y = (float)y / column_height * wall_texture.getSize().y;

		// calculate the position of the pixel in the window
		float window_y = ((float)render_height - column_height) / 2 + (float)y;

		if (window_y > render_height) {
		  break;
		}

		// draw the pixel
		wall_sprite.setPosition((float)window_x, window_y);
		wall_sprite.setTextureRect(sf::IntRect(texture_pixel_x, (int)texture_pixel_y, ray_thickness, ray_thickness));
		game_manager.window.draw(wall_sprite);
	  }
	}
  }
}

void draw_chunked_raycast_hits(std::vector<std::vector<ComputedDrawHit>>
							   &chunked_raycast_hits, GameManager &game_manager,
							   int raycast_thickness,
							   sf::Texture &wall_texture,
							   sf::Sprite &wall_sprite,
							   sf::Texture &floor_texture,
							   sf::Sprite &floor_sprite,
							   sf::Texture &ceiling_texture,
							   sf::Sprite &ceiling_sprite) {
  int field_height = (int)game_manager.window.getSize().y - game_manager.hud.bar_height;
  int half_field_height = field_height / 2;

  // Range-based for loop
  for (const auto &chunk : chunked_raycast_hits) {
	for (const auto &computed_draw_hit : chunk) {
	  if (computed_draw_hit.tile_symbol == Tile::Symbol::WALL) {
		wall_sprite.setPosition((float)computed_draw_hit.pixel_pos.x, (float)computed_draw_hit.pixel_pos.y);
		wall_sprite.setTextureRect(sf::IntRect(computed_draw_hit.texture_percentage_coords.x * wall_texture.getSize().x,
											   computed_draw_hit.texture_percentage_coords.y * wall_texture.getSize().y,
											   raycast_thickness,
											   raycast_thickness));
		game_manager.window.draw(wall_sprite);
	  } else if (computed_draw_hit.tile_symbol == Tile::Symbol::FLOOR) {
		// floor

		floor_sprite.setPosition((float)computed_draw_hit.pixel_pos.x, (float)computed_draw_hit.pixel_pos.y);
		floor_sprite.setTextureRect(sf::IntRect(
			computed_draw_hit.texture_percentage_coords.x * floor_texture.getSize().x,
			computed_draw_hit.texture_percentage_coords.y * floor_texture.getSize().y,
			raycast_thickness,
			raycast_thickness));
		game_manager.window.draw(floor_sprite);

		// ceiling
		int ceiling_y = half_field_height - (computed_draw_hit.pixel_pos.y - half_field_height);

		ceiling_sprite.setPosition((float)computed_draw_hit.pixel_pos.x, (float)ceiling_y);
		ceiling_sprite.setTextureRect(sf::IntRect(
			computed_draw_hit.texture_percentage_coords.x * ceiling_texture.getSize().x,
			computed_draw_hit.texture_percentage_coords.y * ceiling_texture.getSize().y,
			raycast_thickness,
			raycast_thickness));

		game_manager.window.draw(ceiling_sprite);
	  }
	}
  }
}

void draw_raycast_map(std::vector<ComputedDrawHit> &raycast_map,
					  sf::RenderWindow &window,
					  int raycast_thickness,
					  sf::Texture &wall_texture,
					  sf::Sprite &wall_sprite,
					  sf::Texture &floor_texture,
					  sf::Sprite &floor_sprite,
					  sf::Texture &ceiling_texture,
					  sf::Sprite &ceiling_sprite) {
  int half_field_height = window.getSize().y / 2;

  // Range-based for loop
  for (const auto &computed_draw_hit : raycast_map) {
	if (computed_draw_hit.tile_symbol == Tile::Symbol::WALL) {
	  wall_sprite.setPosition((float)computed_draw_hit.pixel_pos.x, (float)computed_draw_hit.pixel_pos.y);
	  wall_sprite.setTextureRect(sf::IntRect(computed_draw_hit.texture_percentage_coords.x * wall_texture.getSize().x,
											 computed_draw_hit.texture_percentage_coords.y * wall_texture.getSize().y,
											 raycast_thickness,
											 raycast_thickness));
	  window.draw(wall_sprite);
	} else if (computed_draw_hit.tile_symbol == Tile::Symbol::FLOOR) {
	  // draw floor
	  floor_sprite.setPosition((float)computed_draw_hit.pixel_pos.x, (float)computed_draw_hit.pixel_pos.y);
	  floor_sprite.setTextureRect(sf::IntRect(computed_draw_hit.texture_percentage_coords.x * floor_texture.getSize().x,
											  computed_draw_hit.texture_percentage_coords.y * floor_texture.getSize().y,
											  raycast_thickness,
											  raycast_thickness));
	  window.draw(floor_sprite);

	  // draw ceiling
	  int ceiling_y = half_field_height - (computed_draw_hit.pixel_pos.y - half_field_height);

	  ceiling_sprite.setPosition((float)computed_draw_hit.pixel_pos.x, (float)ceiling_y);
	  ceiling_sprite.setTextureRect(sf::IntRect(
		  computed_draw_hit.texture_percentage_coords.x * ceiling_texture.getSize().x,
		  computed_draw_hit.texture_percentage_coords.y * ceiling_texture.getSize().y,
		  raycast_thickness,
		  raycast_thickness));
	  window.draw(ceiling_sprite);
	}
  }
}

void draw_hud_bar_level(GameManager &game_manager) {
  // draw the level with the numbers sprite sheet and the corresponding SpriteId
  sf::Texture
	  numbers_texture = TextureManager::get_instance().get_texture(static_cast<SpriteId>(game_manager.current_level));
  sf::Sprite digit_sprite = sf::Sprite(numbers_texture);
  SpriteSetting digit_sprite_settings = SPRITE_SETTINGS.at(SpriteId(game_manager.current_level));

  digit_sprite.setTextureRect(sf::IntRect(digit_sprite_settings.initial_offset.x,
										  digit_sprite_settings.initial_offset.y,
										  digit_sprite_settings.frame_size.x,
										  digit_sprite_settings.frame_size.y));

  sf::Vector2i hud_bar_start_pos = sf::Vector2i(0, game_manager.window.getSize().y - game_manager.hud.bar_height);
  sf::Vector2f level_sprite_center_ratio_pos = HUD_RELATIVE_CENTER_RATIO_SETTINGS.level;

  float scale_factor =
	  (float)game_manager.hud.bar_height / TextureManager::get_instance().get_texture(SpriteId::HUD_EMPTY).getSize().y;
  digit_sprite.setScale(scale_factor, scale_factor);

  // center the sprite in the hud bar
//  digit_sprite.setPosition(hud_bar_start_pos.x + HUD_RELATIVE_CENTER_RATIO_SETTINGS.level.x * game_manager.hud.bar_width,
//						   hud_bar_start_pos.y + HUD_RELATIVE_CENTER_RATIO_SETTINGS.level.y * game_manager.hud.bar_height);
  digit_sprite.setPosition(hud_bar_start_pos.x + level_sprite_center_ratio_pos.x * game_manager.hud.bar_width
							   - (digit_sprite_settings.frame_size.x * scale_factor) / 2,
						   hud_bar_start_pos.y + level_sprite_center_ratio_pos.y * game_manager.hud.bar_height
							   - (digit_sprite_settings.frame_size.y * scale_factor) / 2);

  game_manager.window.draw(digit_sprite);
}

void draw_hud_bar(GameManager &game_manager, sf::Texture &hud_texture, sf::Sprite &hud_sprite) {
  float width_to_height_ratio = (float)hud_texture.getSize().x / (float)hud_texture.getSize().y;
  int render_width = game_manager.hud.bar_height * width_to_height_ratio;

  // draw the hud
  hud_sprite.setPosition(0, game_manager.window.getSize().y - game_manager.hud.bar_height);
  hud_sprite.setScale((float)render_width / (float)hud_texture.getSize().x,
					  (float)game_manager.hud.bar_height / (float)hud_texture.getSize().y);
  game_manager.window.draw(hud_sprite);

  draw_hud_bar_level(game_manager);
}

void draw_hud(GameManager &game_manager, sf::Texture &hud_texture, sf::Sprite &hud_sprite) {
  draw_minimap(game_manager);
  draw_hud_bar(game_manager, hud_texture, hud_sprite);
}

void draw_equipped_weapon(GameManager &game_manager, sf::Texture &weapon_texture, sf::Sprite &weapon_sprite) {
  float width_to_height_ratio = (float)weapon_texture.getSize().x / (float)weapon_texture.getSize().y;
  int render_width = game_manager.hud.bar_height * width_to_height_ratio;

  // draw the hud
  weapon_sprite.setPosition(game_manager.window.getSize().x - render_width,
							game_manager.window.getSize().y - game_manager.hud.bar_height);
  weapon_sprite.setScale((float)render_width / (float)weapon_texture.getSize().x,
						 (float)game_manager.hud.bar_height / (float)weapon_texture.getSize().y);
  game_manager.window.draw(weapon_sprite);
}

void render_game_frame(GameManager &game_manager,
					   sf::Texture &wall_texture,
					   sf::Sprite &wall_sprite,
					   sf::Texture &floor_texture,
					   sf::Sprite &floor_sprite,
					   sf::Texture &ceiling_texture,
					   sf::Sprite &ceiling_sprite,
					   sf::Texture &hud_empty_texture,
					   sf::Sprite &hud_empty_sprite) {
  /*
  * Clear the window for drawing
  * */
  game_manager.window.clear();

  /*
   * Draw
   * */

  // 3d floor
  draw_floor_and_ceiling_3d(game_manager,
							floor_texture,
							floor_sprite,
							ceiling_texture,
							ceiling_sprite);
  // 3d walls
  draw_walls_3d(game_manager, wall_texture, wall_sprite);

  // draw player weapon
  game_manager.player.update_current_weapon_sprite();
  sf::Sprite &current_weapon_sprite = game_manager.player.current_weapon->get_current_sprite();
  current_weapon_sprite.setScale(6, 6);
  current_weapon_sprite.setPosition(
	  game_manager.window.getSize().x / 2 - current_weapon_sprite.getGlobalBounds().width / 2,
	  game_manager.window.getSize().y - game_manager.hud.bar_height - current_weapon_sprite.getGlobalBounds().height);
  game_manager.window.draw(current_weapon_sprite);

  // draw hud (bar & minimap)
  draw_hud(game_manager, hud_empty_texture, hud_empty_sprite);

  /*
   * display
   * */
  game_manager.window.display();
}
