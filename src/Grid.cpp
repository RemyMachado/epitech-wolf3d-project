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
		return {(float)x * tile_size + tile_size / 2, (float)y * tile_size + tile_size / 2};
	  }
	}
  }

  // if the player position is not found, throw an invalid file exception
  throw std::invalid_argument(
	  "The grid file is invalid: the player position was not found \'" + std::to_string(Tile::Symbol::PLAYER) + "\'");
}

unsigned int Grid::count_enemies() const {
  unsigned int count = 0;
  for (const auto &tile : tiles) {
	if (tile.symbol == Tile::Symbol::ENEMY_DOG || tile.symbol == Tile::Symbol::ENEMY_GUARD) {
	  count++;
	}
  }
  return count;
}

std::vector<Enemy> Grid::get_initial_enemies() const {
  std::vector<Enemy> enemies;
  unsigned int enemy_count = count_enemies();

  if (enemy_count == 0) {
	return enemies;
  }
  // if reserve is not provided, the vector will be resized every time an enemy is added and
  // will create issues because of the callback functions pointing to deleted objects
  enemies.reserve(count_enemies());

  // find the enemies positions in the grid
  for (int y = 0; y < height; y++) {
	for (int x = 0; x < width; x++) {
	  if (get(x, y).symbol == Tile::Symbol::ENEMY_DOG || get(x, y).symbol == Tile::Symbol::ENEMY_GUARD) {
		enemies.emplace_back(ENEMY_SETTINGS.at(get(x, y).symbol), *this, sf::Vector2i(x, y),
							 sf::Vector2f{(float)x * tile_size + tile_size / 2, (float)y * tile_size + tile_size / 2});
	  }
	}
  }

  return enemies;
}
sf::Vector2i Grid::pos_to_coords(sf::Vector2f pos) const {
  return {(int)(pos.x / tile_size), (int)(pos.y / tile_size)};
}
sf::Vector2f Grid::get_tile_center(sf::Vector2i coords) const {
  return {(float)coords.x * tile_size + tile_size / 2, (float)coords.y * tile_size + tile_size / 2};
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
