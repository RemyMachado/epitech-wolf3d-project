#ifndef EPITECH_WOLF3D_PROJECT_SRC_TILE_HPP_
#define EPITECH_WOLF3D_PROJECT_SRC_TILE_HPP_

#include <SFML/System/Vector2.hpp>
#include <unordered_set>
#include <vector>
#include "lines.hpp"

class Tile {
 public:
  enum Symbol : char;
  Tile::Symbol symbol; // the symbol of the tile in the input file (W, P, ...)
  sf::Vector2i coords; // x, y on the row and column
  sf::Vector2f pos; // taking tile size into account
  std::vector<Line> horizontal_segments;

  enum Side {
	NORTH,
	EAST,
	SOUTH,
	WEST,
  };

  /*
   * <!>
   * Don't forget to add the symbols to the `valid_symbols` set!
   * */
  enum Symbol : char {
	FLOOR = ' ',
	WALL = 'W',
	PLAYER = 'P',
	ENEMY_DOG = 'D',
	ENEMY_GUARD = 'G',
  };

  static const std::unordered_set<char> &get_valid_symbols() {
	static const std::unordered_set<char> valid_symbols = {
		Symbol::FLOOR, Symbol::WALL, Symbol::PLAYER, Symbol::ENEMY_DOG, Symbol::ENEMY_GUARD
	};
	return valid_symbols;
  }

  Tile(Tile::Symbol symbol, sf::Vector2i coords, float tile_size)
	  : symbol(symbol), coords(coords) {
	this->pos = {(float)coords.x * tile_size, (float)coords.y * tile_size};

	Line north = {
		{(float)coords.x * tile_size, (float)coords.y * tile_size},
		{(float)(coords.x + 1) * tile_size, (float)coords.y * tile_size}
	};
	Line east = {
		{(float)(coords.x + 1) * tile_size, (float)coords.y * tile_size},
		{(float)(coords.x + 1) * tile_size, ((float)coords.y + 1) * tile_size}
	};
	Line south = {
		{(float)coords.x * tile_size, ((float)coords.y + 1) * tile_size},
		{(float)(coords.x + 1) * tile_size, ((float)coords.y + 1) * tile_size}
	};
	Line west = {
		{(float)coords.x * tile_size, (float)coords.y * tile_size},
		{(float)coords.x * tile_size, ((float)coords.y + 1) * tile_size}
	};

	this->horizontal_segments = {north, east, south, west};
  }

  static bool is_valid_symbol(char c) {
	const std::unordered_set<char> &valid_symbols = get_valid_symbols();
	return valid_symbols.count(c) > 0;
  }
};

#endif //EPITECH_WOLF3D_PROJECT_SRC_TILE_HPP_
