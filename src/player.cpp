#include "player.hpp"
#include "raycasting.hpp"

void move_player_respect_collision(Grid<int> &grid,
                                   float direction_deg,
                                   std::vector<CubeRaycastSegments> &grid_raycast_segments) {
  std::optional<float> distance_to_wall = raycast(grid.player_pos, direction_deg, grid, grid_raycast_segments);

  if (distance_to_wall.has_value()) {
    grid.player_pos = polar_to_cartesian(grid.player_pos,
                                         std::min(distance_to_wall.value() - 1, grid.player_move_speed),
                                         direction_deg);
  } else {
    grid.player_pos = polar_to_cartesian(grid.player_pos,
                                         grid.player_move_speed,
                                         direction_deg);
  }
}

void handle_player_movement(Grid<int> &grid, std::vector<CubeRaycastSegments> &grid_raycast_segments) {
  float move_direction_deg;

  // move the grid.player_pos with wasd keys allowing multiple keys to be pressed at once
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
    move_direction_deg = grid.player_direction_deg - 90;
    move_player_respect_collision(grid, move_direction_deg, grid_raycast_segments);
  }
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
    move_direction_deg = grid.player_direction_deg + 90;
    move_player_respect_collision(grid, move_direction_deg, grid_raycast_segments);
  }
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
    move_direction_deg = grid.player_direction_deg;
    move_player_respect_collision(grid, move_direction_deg, grid_raycast_segments);
  }
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
    move_direction_deg = grid.player_direction_deg + 180;
    move_player_respect_collision(grid, move_direction_deg, grid_raycast_segments);
  }
}

void handle_player_camera_rotation(Grid<int> &grid, sf::RenderWindow &window, sf::Event &event) {
  if (event.type == sf::Event::MouseMoved) {
    if (!grid.recenter_mouse) {
      sf::Vector2i current_mouse_position = sf::Mouse::getPosition(window);
      sf::Vector2i window_center((int) window.getSize().x / 2, (int) window.getSize().y / 2);
      int mouse_delta_x = current_mouse_position.x - window_center.x;

      grid.player_direction_deg += (float) mouse_delta_x * grid.mouse_rotation_speed_deg;

      // Set the mouse position to the center of the window
      sf::Mouse::setPosition(window_center, window);

      // Enable recenter_mouse to avoid updating grid.player_direction_deg while re-centering
      grid.recenter_mouse = true;
    } else {
      // Reset recenter_mouse for the next mouse movement event
      grid.recenter_mouse = false;
    }
  }
  // rotate the grid.player_direction_deg with left and right arrow keys
  if (event.type == sf::Event::KeyPressed) {
    if (event.key.code == sf::Keyboard::Left) {
      grid.player_direction_deg -= grid.keyboard_rotation_speed_deg;
    }
    if (event.key.code == sf::Keyboard::Right) {
      grid.player_direction_deg += grid.keyboard_rotation_speed_deg;
    }
  }
}
