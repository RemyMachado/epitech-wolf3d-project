#ifndef EPITECH_WOLF3D_PROJECT_SRC_PATHFINDER_HPP_
#define EPITECH_WOLF3D_PROJECT_SRC_PATHFINDER_HPP_

#include <SFML/System/Vector2.hpp>
#include "Tile.hpp"

static constexpr int UNVISITED_WEIGHT = -1;
class Grid;

/*
 * Uses Breadth-First Search
 *
 * Able to:
 * - weight tiles of the grid
 * - find the shortest path between two points
 * */
class PathFinder {
 private:
  std::vector<std::vector<int>> weights;
  const Grid &grid;

 public:
  explicit PathFinder(const Grid &grid);

 public:
  /*
   * Compute tiles weights from the start tile
   * (clearing previous computations)
   * */
  void compute_weights(const sf::Vector2i &start, const std::vector<Tile::Symbol> &forbidden_tiles);
  /*
   * Allow diagonals in the path iff there is no wall between the two tiles
   * Picking at random when there are multiple paths of the same weight
   * `stop_at_weight` can be used to stop the search at a certain weight (e.g. to keep a distance from the player)
   * */
  [[nodiscard]] std::vector<sf::Vector2i> get_shortest_path(const sf::Vector2i &start,
															const sf::Vector2i &end,
															const std::vector<Tile::Symbol> &forbidden_tiles,
															int stop_at_weight = 0) const;

 private:
  /*
   * Allow diagonals in the path iff there is no wall between the two tiles
   * */
  [[nodiscard]] std::vector<sf::Vector2i> keep_lightest_neighbors(const std::vector<sf::Vector2i> &neighbors) const;
  [[nodiscard]] std::vector<sf::Vector2i> get_walkable_orthogonal_neighbors(const sf::Vector2i &tile,
																			const std::vector<Tile::Symbol> &forbidden_tiles) const;
  [[nodiscard]] std::vector<sf::Vector2i> get_walkable_orthogonal_and_diagonal_neighbors(const sf::Vector2i &tile,
																						 const std::vector<Tile::Symbol> &forbidden_tiles) const;
  [[nodiscard]] bool is_walkable(const sf::Vector2i &tile, const std::vector<Tile::Symbol> &forbidden_tiles) const;
  [[nodiscard]] bool is_inbounds(const sf::Vector2i &tile) const;
  [[nodiscard]] bool can_walk_diagonally(const sf::Vector2i &tile,
										 const sf::Vector2i &diagonal_neighbor,
										 const std::vector<Tile::Symbol> &forbidden_tiles) const;
};

#endif //EPITECH_WOLF3D_PROJECT_SRC_PATHFINDER_HPP_
