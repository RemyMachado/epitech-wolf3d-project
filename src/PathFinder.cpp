
#include <queue>
#include "Grid.hpp"
#include "PathFinder.hpp"

PathFinder::PathFinder(const Grid &grid) : grid(grid) {}

void PathFinder::compute_weights(const sf::Vector2i &start, const std::vector<Tile::Symbol> &forbidden_tiles) {
  // clear previous computations
  weights.clear();
  weights.resize(grid.height, std::vector<int>(grid.width, UNVISITED_WEIGHT));

  // init visit_queue
  std::queue<sf::Vector2i> visit_queue;
  visit_queue.push(start);
  weights[start.y][start.x] = 0;

  // BFS
  while (!visit_queue.empty()) {
	sf::Vector2i current = visit_queue.front();
	int current_weight = weights[current.y][current.x];
	visit_queue.pop();

	for (const auto &neighbor : get_walkable_orthogonal_neighbors(current, forbidden_tiles)) {
	  // cut if already visited
	  if (weights[neighbor.y][neighbor.x] != UNVISITED_WEIGHT) {
		continue;
	  }

	  weights[neighbor.y][neighbor.x] = current_weight + 1;
	  visit_queue.push(neighbor);
	}
  }
}
bool PathFinder::can_walk_diagonally(const sf::Vector2i &tile,
									 const sf::Vector2i &diagonal_neighbor,
									 const std::vector<Tile::Symbol> &forbidden_tiles) const {
  return is_walkable(diagonal_neighbor, forbidden_tiles) &&
	  is_walkable({tile.x, diagonal_neighbor.y}, forbidden_tiles) &&
	  is_walkable({diagonal_neighbor.x, tile.y}, forbidden_tiles);
}
std::vector<sf::Vector2i> PathFinder::get_walkable_orthogonal_neighbors(const sf::Vector2i &tile,
																		const std::vector<Tile::Symbol> &forbidden_tiles) const {
  std::vector<sf::Vector2i> neighbors;
  static const std::array<sf::Vector2i, 4> directions = {
	  {
		  {0, -1},   // North
		  {1, 0},   // East
		  {0, 1},  // South
		  {-1, 0}   // West
	  }};

  for (const auto &direction : directions) {
	sf::Vector2i neighbor_tile = tile + direction;

	if (is_walkable(neighbor_tile, forbidden_tiles)) {
	  neighbors.push_back(neighbor_tile);
	}
  }

  return neighbors;
}
std::vector<sf::Vector2i> PathFinder::get_walkable_orthogonal_and_diagonal_neighbors(const sf::Vector2i &tile,
																					 const std::vector<Tile::Symbol> &forbidden_tiles) const {
  std::vector<sf::Vector2i> neighbors = get_walkable_orthogonal_neighbors(tile, forbidden_tiles);

  static const std::array<sf::Vector2i, 4> diagonal_directions = {
	  {
		  {1, -1},   // North-East
		  {1, 1},   // South-East
		  {-1, 1},  // South-West
		  {-1, -1}   // North-West
	  }};

  for (const auto &diagonal_direction : diagonal_directions) {
	sf::Vector2i diagonal_neighbor_tile = tile + diagonal_direction;

	if (can_walk_diagonally(tile, diagonal_neighbor_tile, forbidden_tiles)) {
	  neighbors.push_back(diagonal_neighbor_tile);
	}
  }

  return neighbors;
}
bool PathFinder::is_inbounds(const sf::Vector2i &tile) const {
  return tile.x >= 0 && tile.x < grid.width && tile.y >= 0 && tile.y < grid.height;
}
bool PathFinder::is_walkable(const sf::Vector2i &tile, const std::vector<Tile::Symbol> &forbidden_tiles) const {
  return is_inbounds(tile) &&
	  std::find(forbidden_tiles.begin(), forbidden_tiles.end(),
				grid.get(tile.x, tile.y).symbol) == forbidden_tiles.end();
}
std::vector<sf::Vector2i> PathFinder::keep_lightest_neighbors(const std::vector<sf::Vector2i> &neighbors) const {
  std::vector<sf::Vector2i> lightest_neighbors;
  int lightest_weight = UNVISITED_WEIGHT;

  for (const auto &neighbor : neighbors) {
	int weight = weights[neighbor.y][neighbor.x];

	if (lightest_weight == UNVISITED_WEIGHT) {
	  lightest_weight = weight;
	  lightest_neighbors.push_back(neighbor);
	  continue;
	}

	if (weight < lightest_weight) {
	  lightest_weight = weight;
	  lightest_neighbors.clear();
	  lightest_neighbors.push_back(neighbor);
	} else if (weight == lightest_weight) {
	  lightest_neighbors.push_back(neighbor);
	}
  }

  return lightest_neighbors;
}

std::vector<sf::Vector2i> PathFinder::get_shortest_path(const sf::Vector2i &start,
														const sf::Vector2i &end,
														const std::vector<Tile::Symbol> &forbidden_tiles,
														int stop_at_weight) const {
  std::vector<sf::Vector2i> shortest_path;

  if (start == end ||
	  weights[start.y][start.x] == UNVISITED_WEIGHT ||
	  weights[end.y][end.x] == UNVISITED_WEIGHT) {
	return shortest_path;
  }

  sf::Vector2i current_tile = start;
  while (current_tile != end) {
	int current_weight = weights[current_tile.y][current_tile.x];

	if (current_weight <= stop_at_weight) {
	  return shortest_path;
	}

	std::vector<sf::Vector2i>
		walkable_neighbors = get_walkable_orthogonal_and_diagonal_neighbors(current_tile, forbidden_tiles);
	std::vector<sf::Vector2i> lightest_neighbors = keep_lightest_neighbors(walkable_neighbors);

	// choose a random neighbor if there are multiple lightest ones
	current_tile = lightest_neighbors[rand() % lightest_neighbors.size()];
	shortest_path.push_back(current_tile);
  }

  return shortest_path;
}

