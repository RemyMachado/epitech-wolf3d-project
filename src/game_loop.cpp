#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>
#include "game_loop.hpp"
#include "lines.hpp"
#include "raycasting.hpp"

const int WINDOW_WIDTH = 1920;
const int WINDOW_HEIGHT = 1080;

void run_game(Grid<int> &grid) {
  float circleRadius = 5.f;
  sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "SFML works!");
  sf::CircleShape circleShape(circleRadius);
  circleShape.setFillColor(sf::Color::White);

  // hide the mouse cursor
  window.setMouseCursorVisible(false);
  float move_speed = grid.cube_size * 0.1f;
  float rotation_speed_deg = 5;

  while (window.isOpen()) {
    // print mouse coordinates
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed ||
          (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape))
        window.close();

      // move the grid.player_pos with wasd keys
      if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::A) {
          grid.player_pos = polar_to_cartesian(grid.player_pos, move_speed, grid.player_direction_deg - 90);
        }
        if (event.key.code == sf::Keyboard::D) {
          grid.player_pos = polar_to_cartesian(grid.player_pos, move_speed, grid.player_direction_deg + 90);
        }
        if (event.key.code == sf::Keyboard::W) {
          grid.player_pos = polar_to_cartesian(grid.player_pos, move_speed, grid.player_direction_deg);
        }
        if (event.key.code == sf::Keyboard::S) {
          grid.player_pos = polar_to_cartesian(grid.player_pos, move_speed, grid.player_direction_deg + 180);
        }
      }

      // rotate the grid.player_direction_deg with left and right arrow keys
      if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::Left) {
          grid.player_direction_deg -= rotation_speed_deg;
        }
        if (event.key.code == sf::Keyboard::Right) {
          grid.player_direction_deg += rotation_speed_deg;
        }
      }
    }

    window.clear();
    // draw circleShape centered at mouse position
    circleShape.setPosition(grid.player_pos.x - circleRadius, grid.player_pos.y - circleRadius);
    window.draw(circleShape);

    // draw grid using `get_grid_raycast_segments`
    std::vector<CubeRaycastSegments> grid_raycast_segments = get_grid_raycast_segments(grid);

    for (const CubeRaycastSegments &cube_raycast_segments : grid_raycast_segments) {
      for (Line raycast_segment : cube_raycast_segments) {
        sf::Vertex line[] = {
            sf::Vertex(sf::Vector2f(raycast_segment.start_pos.x + 1, raycast_segment.start_pos.y + 1)),
            sf::Vertex(sf::Vector2f(raycast_segment.end_pos.x + 1, raycast_segment.end_pos.y + 1))
        };
        window.draw(line, 2, sf::Lines);
      }
    }

    // raycast from the player position in the player direction
    // and draw a line from the player position to the intersection point
    std::optional<float> intersection_distance = raycast(grid.player_pos, grid.player_direction_deg, grid);
    if (intersection_distance.has_value()) {
      sf::Vertex line[] = {
          sf::Vertex(sf::Vector2f(grid.player_pos.x, grid.player_pos.y)),
          sf::Vertex(polar_to_cartesian(grid.player_pos, intersection_distance.value(), grid.player_direction_deg))
      };
      window.draw(line, 2, sf::Lines);
    }

    // draw 3d view with raycast
    int render_width = 400;
    int render_height = 400;
    // for each pixel in width, cast a ray and draw a vertical line
    for (int x = 0; x < render_width; x++) {
      // calculate the angle of the ray
      float rotation_step_deg = grid.fov_deg / render_width;
      float ray_angle_deg = grid.player_direction_deg + rotation_step_deg * (float) x - grid.fov_deg / 2;
      // cast the ray
      std::optional<float> intersection_distance2 = raycast(grid.player_pos, ray_angle_deg, grid);
      if (intersection_distance2.has_value()) {
        // calculate the height of the line
//        float line_height = (render_height / intersection_distance2.value()) * 0.5f;

        float line_height = render_height * (grid.cube_size / intersection_distance2.value());

        // draw the line
        sf::Vertex line[] = {
            sf::Vertex(sf::Vector2f(WINDOW_WIDTH / 2 + x, ((float) render_height - line_height) / 2), sf::Color::Red),
            sf::Vertex(sf::Vector2f(WINDOW_WIDTH / 2 + x, render_height - ((float) render_height - line_height) / 2),
                       sf::Color::Red)
        };
        window.draw(line, 2, sf::Lines);
      }
    }

    window.display();
  }
}