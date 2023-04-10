#include <iostream>
#include "draw.hpp"
#include "raycasting.hpp"
#include "Grid.hpp"

void draw_player_on_minimap(sf::RenderWindow &window, Grid<int> &grid) {
  sf::CircleShape circleShape(grid.player_circle_radius_minimap);
  circleShape.setFillColor(sf::Color::Red);

  circleShape.setPosition(grid.player_pos.x - grid.player_circle_radius_minimap,
                          grid.player_pos.y - grid.player_circle_radius_minimap);
  window.draw(circleShape);
}

void draw_minimap_grid(sf::RenderWindow &window,
                       Grid<int> &grid,
                       std::vector<Cube> &grid_cubes) {
  // draw background
  sf::RectangleShape rectangle(sf::Vector2f(grid.width * grid.cube_size, grid.height * grid.cube_size));
  rectangle.setFillColor(sf::Color::Black);
  rectangle.setPosition(0, 0);
  window.draw(rectangle);

  // draw walls on the grid
  for (const Cube &cube : grid_cubes) {
    if (cube.value != CubeValue::WALL) {
      continue;
    }

    for (const Line &segment : cube.segments) {
      sf::Vertex line[] = {
          sf::Vertex(sf::Vector2f(segment.start.x + 1, segment.start.y + 1)),
          sf::Vertex(sf::Vector2f(segment.end.x + 1, segment.end.y + 1))
      };
      window.draw(line, 2, sf::Lines);
    }
  }
}

void draw_player_direction_minimap(sf::RenderWindow &window,
                                   Grid<int> &grid,
                                   std::vector<Cube> &grid_cubes) {
  // raycast from the player position in the player direction
  // and draw a line from the player position to the intersection point
  std::optional<Raycast>
      raycast_wall = raycast(grid.player_pos, grid.player_direction_deg, grid, grid_cubes, CubeValue::WALL);
  if (raycast_wall.has_value()) {
    sf::Vertex line[] = {
        sf::Vertex(sf::Vector2f(grid.player_pos.x, grid.player_pos.y)),
        sf::Vertex(raycast_wall.value().intersection_pos)
    };
    window.draw(line, 2, sf::Lines);
  }
}

void draw_minimap(sf::RenderWindow &window, Grid<int> &grid, std::vector<Cube> &grid_cubes) {
  // draw grid
  draw_minimap_grid(window, grid, grid_cubes);

  // draw player direction
  draw_player_direction_minimap(window, grid, grid_cubes);

  // draw player
  draw_player_on_minimap(window, grid);
}

void draw_floor_and_ceiling_3d(sf::RenderWindow &window,
                               Grid<int> &grid,
                               std::vector<Cube> &grid_cubes,
                               sf::Texture &floor_texture,
                               sf::Sprite &floor_sprite,
                               sf::Texture &ceiling_texture,
                               sf::Sprite &ceiling_sprite) {
  int render_width = (int) window.getSize().x;
  int render_height = (int) window.getSize().y;
  float half_render_height = (float) render_height / 2;
  int ray_thickness = 4;

  // for each pixel in height, cast a ray and draw a horizontal line
  for (int window_y = half_render_height; window_y < render_height; window_y += ray_thickness) {
    // calculate the angle of the ray
    float vertical_camera_rotation_step_deg = grid.fov_vertical_deg / (float) render_height;
    float vertical_ray_angle_deg =
        vertical_camera_rotation_step_deg * (float) window_y - grid.fov_vertical_deg / 2;

    // for each pixel of the horizontal line cast an imaginary ray using trigonometry
    for (int window_x = 0; window_x < render_width; window_x += ray_thickness) {
      // calculate the angle of the ray
      float horizontal_camera_rotation_step_deg = grid.fov_horizontal_deg / (float) render_width;
      float
          horizontal_ray_angle_deg = grid.player_direction_deg - grid.fov_horizontal_deg / 2
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
        distance_to_point_above_floor = grid.render_distance;
      } else {
        distance_to_point_above_floor = grid.camera_height / tangent_vertical_ray;
      }

      // find floor coordinates by going in horizontal direction for the found distance
      sf::Vector2f floor_intersection_pos = polar_to_cartesian(grid.player_pos,
                                                               distance_to_point_above_floor,
                                                               horizontal_ray_angle_deg);

      if (floor_intersection_pos.x < 0 || floor_intersection_pos.x > grid.width * grid.cube_size ||
          floor_intersection_pos.y < 0 || floor_intersection_pos.y > grid.height * grid.cube_size) {
        continue;
      }

      // find the texture coordinates
      sf::Vector2f local_intersection =
          floor_intersection_pos - sf::Vector2f(floor(floor_intersection_pos.x / grid.cube_size) * grid.cube_size,
                                                floor(floor_intersection_pos.y / grid.cube_size) * grid.cube_size);
      sf::Vector2f texture_coordinates = sf::Vector2f(local_intersection.x / grid.cube_size * floor_texture.getSize().x,
                                                      local_intersection.y / grid.cube_size
                                                          * floor_texture.getSize().y);

      // draw the floor
      floor_sprite.setPosition((float) window_x, (float) window_y);
      floor_sprite.setTextureRect(sf::IntRect((int) texture_coordinates.x,
                                              (int) texture_coordinates.y,
                                              ray_thickness,
                                              ray_thickness));
      window.draw(floor_sprite);

      // draw the ceiling
      ceiling_sprite.setPosition((float) window_x, render_height - (float) window_y);
      ceiling_sprite.setTextureRect(sf::IntRect((int) texture_coordinates.x,
                                                (int) texture_coordinates.y,
                                                ray_thickness,
                                                ray_thickness));
      window.draw(ceiling_sprite);
    }
  }
}

void draw_walls_3d(sf::RenderWindow &window,
                   Grid<int> &grid,
                   std::vector<Cube> &grid_cubes,
                   sf::Texture &wall_texture,
                   sf::Sprite &wall_sprite, sf::Texture &floor_texture,
                   sf::Sprite &floor_sprite, sf::Texture &ceiling_texture,
                   sf::Sprite &ceiling_sprite) {
  int render_width = (int) window.getSize().x;
  int render_height = (int) window.getSize().y;
  int ray_thickness = 6;

  // for each pixel in width, cast a ray and draw a vertical line
  for (int window_x = 0; window_x < render_width; window_x += ray_thickness) {
    // calculate the angle of the ray
    float rotation_step_deg = grid.fov_horizontal_deg / render_width;
    float
        ray_angle_deg = grid.player_direction_deg - grid.fov_horizontal_deg / 2 + rotation_step_deg * (float) window_x;
    float ray_angle_diff_deg = ray_angle_deg - grid.player_direction_deg;
    float fish_eye_correction = cosf(degrees_to_radians(ray_angle_diff_deg));

    // cast the ray
    std::optional<Raycast> raycast_wall = raycast(grid.player_pos, ray_angle_deg, grid, grid_cubes, CubeValue::WALL);
    if (raycast_wall.has_value()) {
      float raycast_distance_corrected = raycast_wall.value().distance * fish_eye_correction;

      // the height to draw
      float column_height = (float) render_height * (grid.cube_size / raycast_distance_corrected);

      // calculate the position of the window_x pixel in the texture using hit side
      float texture_pixel_x = 0;

      if (raycast_wall.value().hit_side == CubeSide::SOUTH) {
        texture_pixel_x = raycast_wall.value().local_intersection.x / grid.cube_size * wall_texture.getSize().x;
      } else if (raycast_wall.value().hit_side == CubeSide::NORTH) {
        texture_pixel_x = (1 - raycast_wall.value().local_intersection.x / grid.cube_size) * wall_texture.getSize().x;
      } else if (raycast_wall.value().hit_side == CubeSide::WEST) {
        texture_pixel_x = raycast_wall.value().local_intersection.y / grid.cube_size * wall_texture.getSize().x;
      } else if (raycast_wall.value().hit_side == CubeSide::EAST) {
        texture_pixel_x = (1 - raycast_wall.value().local_intersection.y / grid.cube_size) * wall_texture.getSize().x;
      }

      // for each pixel in height, draw the corresponding pixel of the texture
      for (int y = 0; y < column_height; y += ray_thickness) {
        // calculate the position of the y pixel in the texture
        float texture_pixel_y = (float) y / column_height * wall_texture.getSize().y;

        // calculate the position of the pixel in the window
        float window_y = ((float) render_height - column_height) / 2 + (float) y;

        // draw the pixel
        wall_sprite.setPosition((float) window_x, window_y);
        wall_sprite.setTextureRect(sf::IntRect(texture_pixel_x, (int) texture_pixel_y, ray_thickness, ray_thickness));
        window.draw(wall_sprite);
      }
    }
  }
}