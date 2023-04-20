#include "../draw.hpp"
#include "GameManager.hpp"

GameManager::GameManager(char *filename, float tile_size, sf::Vector2i screen_size) :
    window(sf::VideoMode(screen_size.x,
                         screen_size.y),
           "Epitech - Wolf3D"),
    grid(parse_file(filename, tile_size), tile_size),
    camera(grid.tile_size / 2,
           get_magnitude({static_cast<float>(grid.width),
                          static_cast<float>(grid.height)})
               * grid.tile_size),
    player(grid.get_player_initial_pos(),
           0,
           grid.tile_size * 0.05f, grid, camera),
    hud(),
    mouse_and_keyboard(),
    enemies(grid.get_initial_enemies()),
    pickups(grid.get_initial_pickups()),
    path_finder(grid) {
  // set the mouse cursor to the center of the window
  window.setMouseCursorGrabbed(true);
  // hide the mouse cursor
  window.setMouseCursorVisible(false);

  SoundManager::get_instance().play_music_background();

  // initialize the pathfinder
  path_finder.compute_weights(player.get_tile_coords(), UNWALKABLE_TILES);
}
void GameManager::update() {
  // Update the pathfinder weights
  path_finder.compute_weights(player.get_tile_coords(), UNWALKABLE_TILES);

  // Update player, pickups and enemy states.
  player.update();
  for (Pickup &pickup : pickups) {
    pickup.update(player);
  }
  for (Enemy &enemy : enemies) {
    enemy.update(player, path_finder);
  }
}
bool GameManager::check_is_game_over(sf::Sprite &hud_empty_sprite) {
  if (player.get_is_dead()) {
    render_game_over_screen(*this, hud_empty_sprite);
    return true;
  }

  return false;
}
