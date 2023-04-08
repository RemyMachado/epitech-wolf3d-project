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

void draw_walls_3d(sf::RenderWindow &window, Grid<int> &grid, std::vector<Cube> &grid_cubes) {
  int render_width = (int) window.getSize().x;
  int render_height = (int) window.getSize().y;
  int ray_thickness = 5;

  // for each pixel in width, cast a ray and draw a vertical line
  for (int x = 0; x < render_width; x += ray_thickness) {
    // calculate the angle of the ray
    float rotation_step_deg = grid.fov_deg / render_width;
    float ray_angle_deg = grid.player_direction_deg - grid.fov_deg / 2 + rotation_step_deg * (float) x;
    float ray_angle_diff_deg = ray_angle_deg - grid.player_direction_deg;
    float fish_eye_correction = cosf(degrees_to_radians(ray_angle_diff_deg));

    // cast the ray
    std::optional<Raycast> raycast_wall = raycast(grid.player_pos, ray_angle_deg, grid, grid_cubes, CubeValue::WALL);
    if (raycast_wall.has_value()) {
      float raycast_distance_corrected = raycast_wall.value().distance * fish_eye_correction;

      float line_height = (float) render_height * (grid.cube_size / raycast_distance_corrected);
      sf::Vector2f line_start_pos = sf::Vector2f((float) x,
                                                 ((float) window.getSize().y - (float) render_height) / 2
                                                     + ((float) render_height - line_height) / 2);
      sf::Vector2f line_end_pos =
          sf::Vector2f((float) x,
                       ((float) window.getSize().y - (float) render_height) / 2
                           + (float) render_height - ((float) render_height - line_height) / 2);

      // draw the line
      sf::Vertex line[] = {
          sf::Vertex(line_start_pos, sf::Color::Red),
          sf::Vertex(line_end_pos, sf::Color::Red)
      };

      // draw the line with thickness equal to ray_thickness
      for (int i = 0; i < ray_thickness; i++) {
        if (line[0].position.x + (float) i > window.getSize().x) {
          break;
        }

        sf::Vertex thick_line[] = {
            sf::Vertex(sf::Vector2f(line[0].position.x + (float) i, line[0].position.y), sf::Color::Red),
            sf::Vertex(sf::Vector2f(line[1].position.x + (float) i, line[1].position.y), sf::Color::Red)
        };

        window.draw(thick_line, 2, sf::Lines);
      }
    }
  }
}
