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
          grid.player_pos.x -= move_speed;
        }
        if (event.key.code == sf::Keyboard::D) {
          grid.player_pos.x += move_speed;
        }
        if (event.key.code == sf::Keyboard::W) {
          grid.player_pos.y -= move_speed;
        }
        if (event.key.code == sf::Keyboard::S) {
          grid.player_pos.y += move_speed;
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

    // draw grid
    for (int y = 0; y < grid.height; y++) {
      for (int x = 0; x < grid.width; x++) {
        if (grid.get(x, y) == 0) continue;
        // draw rectangles of the grid, but don't fill them
        sf::RectangleShape rectangle(sf::Vector2f(grid.cube_size, grid.cube_size));
        rectangle.setFillColor(sf::Color::Transparent);
        rectangle.setOutlineColor(sf::Color::White);
        rectangle.setOutlineThickness(1);
        rectangle.setPosition((x * grid.cube_size) + 1, (y * grid.cube_size) + 1);
        window.draw(rectangle);
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

    window.display();
  }
}