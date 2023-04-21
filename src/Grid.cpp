#include "Grid.hpp"

Grid::Grid(FileData file_data, float tile_size)
    : width(file_data.width),
      height(file_data.height),
      tiles(std::move(file_data.tiles)), tile_size(tile_size) {
}
Tile Grid::get(int x, int y) const {
  return tiles[y * width + x];
}
sf::Vector2f Grid::get_player_initial_pos() const {
  // find the player position in the grid
  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
      if (get(x, y).symbol == Tile::Symbol::PLAYER) {
        return {(float) x * tile_size + tile_size / 2, (float) y * tile_size + tile_size / 2};
      }
    }
  }

  // if the player position is not found, throw an invalid file exception
  throw std::invalid_argument(
      "The grid file is invalid: the player position was not found \'" + std::to_string(Tile::Symbol::PLAYER) + "\'");
}
unsigned int Grid::count_pickups() const {
  unsigned int count = 0;
  for (const auto &tile : tiles) {
    if (std::find(PICKUP_TILES.begin(), PICKUP_TILES.end(), tile.symbol) != PICKUP_TILES.end()) {
      count++;
    }
  }

  /* adds 1 for the final pickup key (appearing at the player initial_position) */
  return count + 1;
}
unsigned int Grid::count_enemies() const {
  unsigned int count = 0;
  for (const auto &tile : tiles) {
    if (std::find(ENEMY_TILES.begin(), ENEMY_TILES.end(), tile.symbol) != ENEMY_TILES.end()) {
      count++;
    }
  }
  return count;
}
std::vector<Pickup> Grid::get_initial_pickups() const {
  std::vector<Pickup> pickups;
  unsigned int pickup_count = count_pickups();

  if (pickup_count == 0) {
    return pickups;
  }
  // if reserve is not provided, the vector will be resized every time an enemy is added and
  // will create issues because of the callback functions pointing to deleted objects
  pickups.reserve(pickup_count);

  // find the pickups positions in the grid
  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
      if (std::find(PICKUP_TILES.begin(), PICKUP_TILES.end(), get(x, y).symbol) != PICKUP_TILES.end()) {
        pickups.emplace_back(PICKUP_SETTINGS.at(get(x, y).symbol), sf::Vector2i(x, y), tile_size);
      }
    }
  }

  return pickups;
}
std::vector<Enemy> Grid::get_initial_enemies() const {
  std::vector<Enemy> enemies;
  unsigned int enemy_count = count_enemies();

  if (enemy_count == 0) {
    return enemies;
  }
  // if reserve is not provided, the vector will be resized every time an enemy is added and
  // will create issues because of the callback functions pointing to deleted objects
  enemies.reserve(enemy_count);

  // find the enemies positions in the grid
  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
      if (std::find(ENEMY_TILES.begin(), ENEMY_TILES.end(), get(x, y).symbol) != ENEMY_TILES.end()) {
        enemies.emplace_back(ENEMY_SETTINGS.at(get(x, y).symbol), *this, sf::Vector2i(x, y),
                             sf::Vector2f{(float) x * tile_size + tile_size / 2,
                                          (float) y * tile_size + tile_size / 2});
      }
    }
  }

  return enemies;
}
sf::Vector2i Grid::pos_to_coords(sf::Vector2f pos) const {
  return {(int) (pos.x / tile_size), (int) (pos.y / tile_size)};
}
sf::Vector2f Grid::get_tile_center(sf::Vector2i coords) const {
  return {(float) coords.x * tile_size + tile_size / 2, (float) coords.y * tile_size + tile_size / 2};
}
void Grid::set_tile_symbol(sf::Vector2i coords, Tile::Symbol new_symbol) {
  tiles[coords.y * width + coords.x].symbol = new_symbol;
}

/*
std::ostream &operator<<(std::ostream &os, const Grid &grid) {
  for (int y = 0; y < grid.height; y++) {
	for (int x = 0; x < grid.width; x++) {
	  os << grid.get(x, y).symbol;
	}
	os << std::endl;
  }
  return os;
}
*/
