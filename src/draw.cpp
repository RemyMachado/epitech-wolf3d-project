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
                       std::vector<CubeRaycastSegments> &grid_raycast_segments) {
  // draw grid
  for (const CubeRaycastSegments &cube_raycast_segments : grid_raycast_segments) {
    for (Line raycast_segment : cube_raycast_segments) {
      sf::Vertex line[] = {
          sf::Vertex(sf::Vector2f(raycast_segment.start_pos.x + 1, raycast_segment.start_pos.y + 1)),
          sf::Vertex(sf::Vector2f(raycast_segment.end_pos.x + 1, raycast_segment.end_pos.y + 1))
      };
      window.draw(line, 2, sf::Lines);
    }
  }
}

void draw_player_direction_minimap(sf::RenderWindow &window,
                                   Grid<int> &grid,
                                   std::vector<CubeRaycastSegments> &grid_raycast_segments) {
  // raycast from the player position in the player direction
  // and draw a line from the player position to the intersection point
  std::optional<float>
      intersection_distance = raycast(grid.player_pos, grid.player_direction_deg, grid, grid_raycast_segments);
  if (intersection_distance.has_value()) {
    sf::Vertex line[] = {
        sf::Vertex(sf::Vector2f(grid.player_pos.x, grid.player_pos.y)),
        sf::Vertex(polar_to_cartesian(grid.player_pos, intersection_distance.value(), grid.player_direction_deg))
    };
    window.draw(line, 2, sf::Lines);
  }
}

void draw_minimap(sf::RenderWindow &window, Grid<int> &grid, std::vector<CubeRaycastSegments> &grid_raycast_segments) {
  // draw grid
  draw_minimap_grid(window, grid, grid_raycast_segments);

  // draw player direction
  draw_player_direction_minimap(window, grid, grid_raycast_segments);

  // draw player
  draw_player_on_minimap(window, grid);
}

void draw_walls_3d(sf::RenderWindow &window, Grid<int> &grid, std::vector<CubeRaycastSegments> &grid_raycast_segments) {
  int render_width = (int) window.getSize().x / 2;
  int render_height = (int) window.getSize().y / 2;
  int ray_thickness = 5;
  // for each pixel in width, cast a ray and draw a vertical line
  for (int x = 0; x < render_width; x += ray_thickness) {
    // calculate the angle of the ray
    float rotation_step_deg = grid.fov_deg / render_width;
    float ray_angle_deg = grid.player_direction_deg - grid.fov_deg / 2 + rotation_step_deg * (float) x;
    float ray_angle_diff_deg = ray_angle_deg - grid.player_direction_deg;
    float fish_eye_correction = cosf(degrees_to_radians(ray_angle_diff_deg));

    // cast the ray
    std::optional<float> intersection_distance2 = raycast(grid.player_pos, ray_angle_deg, grid, grid_raycast_segments);
    if (intersection_distance2.has_value()) {
      intersection_distance2 = intersection_distance2.value() * fish_eye_correction;

      // calculate the height of the line
//        float line_height = (render_height / intersection_distance2.value()) * 0.5f;

      float line_height = (float) render_height * (grid.cube_size / intersection_distance2.value());
      sf::Vector2f line_start_pos = sf::Vector2f((float) window.getSize().x / 2 + (float) x,
                                                 ((float) window.getSize().y - (float) render_height) / 2
                                                     + ((float) render_height - line_height) / 2);
      sf::Vector2f line_end_pos =
          sf::Vector2f((float) window.getSize().x / 2 + (float) x,
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
