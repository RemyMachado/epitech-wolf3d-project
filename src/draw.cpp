#include "GameManager.hpp"
#include "draw.hpp"
#include "raycasting.hpp"

void draw_player_on_minimap(GameManager &game_manager) {
  sf::CircleShape circleShape(game_manager.hud.minimap_player_radius);
  circleShape.setFillColor(sf::Color::Red);

  circleShape.setPosition(game_manager.player.pos.x - game_manager.hud.minimap_player_radius,
                          game_manager.player.pos.y - game_manager.hud.minimap_player_radius);
  game_manager.window.draw(circleShape);
}

void draw_minimap_grid(GameManager &game_manager) {
  // draw background
  sf::RectangleShape rectangle(sf::Vector2f((float) game_manager.grid.width * game_manager.grid.tile_size,
      (float) game_manager.grid.height * game_manager.grid.tile_size));
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
          sf::Vertex(sf::Vector2f(segment.start.x + 1, segment.start.y + 1)),
          sf::Vertex(sf::Vector2f(segment.end.x + 1, segment.end.y + 1))
      };
      game_manager.window.draw(line, 2, sf::Lines);
    }
  }
}

void draw_player_direction_minimap(GameManager &game_manager) {
  // raycast from the player position in the player direction
  // and draw a line from the player position to the intersection point
  std::optional<Raycast>
      raycast_wall =
      raycast(game_manager.player.pos,
              game_manager.player.dir_deg,
              game_manager.camera.render_distance,
              game_manager.grid,
              Tile::Symbol::WALL);
  if (raycast_wall.has_value()) {
    sf::Vertex line[] = {
        sf::Vertex(sf::Vector2f(game_manager.player.pos.x, game_manager.player.pos.y)),
        sf::Vertex(raycast_wall.value().intersection_pos)
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
  int render_width = (int) game_manager.window.getSize().x;
  int render_height = (int) game_manager.window.getSize().y - game_manager.hud.height;
  int half_render_height = render_height / 2;
  int ray_thickness = 4;

  // for each pixel in height, cast a ray and draw a horizontal line
  for (int window_y = half_render_height; window_y < render_height; window_y += ray_thickness) {
    // calculate the angle of the ray
    float vertical_camera_rotation_step_deg = game_manager.camera.fov_vertical_deg / (float) render_height;
    float vertical_ray_angle_deg =
        vertical_camera_rotation_step_deg * (float) window_y - game_manager.camera.fov_vertical_deg / 2;

    // for each pixel of the horizontal line cast an imaginary ray using trigonometry
    for (int window_x = 0; window_x < render_width; window_x += ray_thickness) {
      // calculate the angle of the ray
      float horizontal_camera_rotation_step_deg = game_manager.camera.fov_horizontal_deg / (float) render_width;
      float
          horizontal_ray_angle_deg = game_manager.player.dir_deg - game_manager.camera.fov_horizontal_deg / 2
          + horizontal_camera_rotation_step_deg * (float) window_x;

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
      floor_sprite.setPosition((float) window_x, (float) window_y);
      floor_sprite.setTextureRect(sf::IntRect((int) texture_coordinates.x,
                                              (int) texture_coordinates.y,
                                              ray_thickness,
                                              ray_thickness));
      game_manager.window.draw(floor_sprite);

      // draw the ceiling
      ceiling_sprite.setPosition((float) window_x, render_height - (float) window_y);
      ceiling_sprite.setTextureRect(sf::IntRect((int) texture_coordinates.x,
                                                (int) texture_coordinates.y,
                                                ray_thickness,
                                                ray_thickness));
      game_manager.window.draw(ceiling_sprite);
    }
  }
}

void draw_walls_3d(GameManager &game_manager,
                   sf::Texture &wall_texture,
                   sf::Sprite &wall_sprite) {
  int render_width = (int) game_manager.window.getSize().x;
  int render_height = (int) game_manager.window.getSize().y - game_manager.hud.height;
  int ray_thickness = 6;

  // for each pixel in width, cast a ray and draw a vertical line
  for (int window_x = 0; window_x < render_width; window_x += ray_thickness) {
    // calculate the angle of the ray
    float rotation_step_deg = game_manager.camera.fov_horizontal_deg / render_width;
    float
        ray_angle_deg =
        game_manager.player.dir_deg - game_manager.camera.fov_horizontal_deg / 2 + rotation_step_deg * (float) window_x;
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
      float column_height = (float) render_height * (game_manager.grid.tile_size / raycast_distance_corrected);

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
      for (int y = 0; y < column_height; y += ray_thickness) {
        // calculate the position of the y pixel in the texture
        float texture_pixel_y = (float) y / column_height * wall_texture.getSize().y;

        // calculate the position of the pixel in the window
        float window_y = ((float) render_height - column_height) / 2 + (float) y;

        if (window_y > render_height) {
          break;
        }

        // draw the pixel
        wall_sprite.setPosition((float) window_x, window_y);
        wall_sprite.setTextureRect(sf::IntRect(texture_pixel_x, (int) texture_pixel_y, ray_thickness, ray_thickness));
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
  int field_height = (int) game_manager.window.getSize().y - game_manager.hud.height;
  int half_field_height = field_height / 2;

  // Range-based for loop
  for (const auto &chunk : chunked_raycast_hits) {
	for (const auto &computed_draw_hit : chunk) {
	  if (computed_draw_hit.tile_symbol == Tile::Symbol::WALL) {
		wall_sprite.setPosition((float) computed_draw_hit.pixel_pos.x, (float) computed_draw_hit.pixel_pos.y);
		wall_sprite.setTextureRect(sf::IntRect(computed_draw_hit.texture_percentage_coords.x * wall_texture.getSize().x,
											   computed_draw_hit.texture_percentage_coords.y * wall_texture.getSize().y,
											   raycast_thickness,
											   raycast_thickness));
		game_manager.window.draw(wall_sprite);
	  } else if (computed_draw_hit.tile_symbol == Tile::Symbol::FLOOR) {
		// floor

		floor_sprite.setPosition((float) computed_draw_hit.pixel_pos.x, (float) computed_draw_hit.pixel_pos.y);
		floor_sprite.setTextureRect(sf::IntRect(computed_draw_hit.texture_percentage_coords.x * floor_texture.getSize().x,
												computed_draw_hit.texture_percentage_coords.y * floor_texture.getSize().y,
												raycast_thickness,
												raycast_thickness));
		game_manager.window.draw(floor_sprite);

		// ceiling
		int ceiling_y = half_field_height - (computed_draw_hit.pixel_pos.y - half_field_height);

		ceiling_sprite.setPosition((float) computed_draw_hit.pixel_pos.x, (float) ceiling_y);
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

void draw_raycast_map(std::vector<ComputedDrawHit> &raycast_map, sf::RenderWindow &window, int raycast_thickness, sf::Texture &wall_texture,
                      sf::Sprite &wall_sprite, sf::Texture &floor_texture,
                      sf::Sprite &floor_sprite,
                      sf::Texture &ceiling_texture,
                      sf::Sprite &ceiling_sprite) {
  int half_field_height = window.getSize().y / 2;

  // Range-based for loop
  for (const auto &computed_draw_hit : raycast_map) {
    if (computed_draw_hit.tile_symbol == Tile::Symbol::WALL) {
      wall_sprite.setPosition((float) computed_draw_hit.pixel_pos.x, (float) computed_draw_hit.pixel_pos.y);
      wall_sprite.setTextureRect(sf::IntRect(computed_draw_hit.texture_percentage_coords.x * wall_texture.getSize().x,
                                             computed_draw_hit.texture_percentage_coords.y * wall_texture.getSize().y,
                                             raycast_thickness,
                                             raycast_thickness));
      window.draw(wall_sprite);
    } else if (computed_draw_hit.tile_symbol == Tile::Symbol::FLOOR) {
      // draw floor
      floor_sprite.setPosition((float) computed_draw_hit.pixel_pos.x, (float) computed_draw_hit.pixel_pos.y);
      floor_sprite.setTextureRect(sf::IntRect(computed_draw_hit.texture_percentage_coords.x * floor_texture.getSize().x,
                                              computed_draw_hit.texture_percentage_coords.y * floor_texture.getSize().y,
                                              raycast_thickness,
                                              raycast_thickness));
      window.draw(floor_sprite);

      // draw ceiling
      int ceiling_y = half_field_height - (computed_draw_hit.pixel_pos.y - half_field_height);

      ceiling_sprite.setPosition((float) computed_draw_hit.pixel_pos.x, (float) ceiling_y);
      ceiling_sprite.setTextureRect(sf::IntRect(
          computed_draw_hit.texture_percentage_coords.x * ceiling_texture.getSize().x,
          computed_draw_hit.texture_percentage_coords.y * ceiling_texture.getSize().y,
          raycast_thickness,
          raycast_thickness));
      window.draw(ceiling_sprite);
    }
  }
}

void draw_hud(GameManager &game_manager) {
  int render_width = (int) game_manager.window.getSize().x;

  // draw the background
  sf::RectangleShape background(sf::Vector2f(render_width, game_manager.hud.height));
  background.setFillColor(sf::Color::Blue);
  background.setPosition(0, game_manager.window.getSize().y - game_manager.hud.height);
  game_manager.window.draw(background);
}