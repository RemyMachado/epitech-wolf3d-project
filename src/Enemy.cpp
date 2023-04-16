
#include "Enemy.hpp"

std::ostream &operator<<(std::ostream &os, const Enemy &enemy) {
  os << "Enemy - " << (enemy.symbol == Tile::Symbol::ENEMY_DOG ? "DOG" : "GUARD") << "(" << enemy.coords.x << ", "
	 << enemy.coords.y << ")";

  return os;
}

