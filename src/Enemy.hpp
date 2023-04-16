#ifndef EPITECH_WOLF3D_PROJECT_SRC_ENEMY_HPP_
#define EPITECH_WOLF3D_PROJECT_SRC_ENEMY_HPP_

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include "Animation.hpp"
#include "Tile.hpp"

class Enemy;

struct EnemyDistanceToPlayer {
  std::reference_wrapper<Enemy> enemy;
  float distance;
};

class Enemy {
 public:
  sf::Vector2i coords;
  sf::Vector2f pos;
  Tile::Symbol symbol;

  /* Animation */
  sf::Sprite idle_sprite;

  Enemy(sf::Vector2i coords, sf::Vector2f pos, Tile::Symbol symbol) : coords(coords), pos(pos), symbol(symbol) {};

  Enemy(const Enemy &other) = default;
  Enemy(Enemy &&other) = default;

  // print operator
  friend std::ostream &operator<<(std::ostream &os, const Enemy &enemy);
};

#endif //EPITECH_WOLF3D_PROJECT_SRC_ENEMY_HPP_
