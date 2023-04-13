#include <utility>
#include <unordered_map>
#include <optional>
#include <SFML/System/Vector2.hpp>
#include "raycasting.hpp"
#include "my_math.hpp"
#include "Grid.hpp"
#include "Tile.hpp"
#include "managers/GameManager.hpp"

Tile::Side determine_hit_side(float tile_size, const Tile &tile, const Line &intersected_segment) {
  // Get the center of the intersected segment
  sf::Vector2f segment_center = (intersected_segment.start + intersected_segment.end) / 2.0f;

  // Get the tile center
  sf::Vector2f tile_center = tile.pos + sf::Vector2f{tile_size / 2.0f, tile_size / 2.0f};

  // Calculate the direction vector from the center of the tile to the center of the hit segment
  sf::Vector2f tile_center_to_segment_center = segment_center - tile_center;

  // Normalize the vector
  sf::Vector2f normalized_tile_center_to_segment_center = normalize_vector(tile_center_to_segment_center);

  // Check the direction of the normalized vector to determine the hit side
  if (std::abs(normalized_tile_center_to_segment_center.x) > std::abs(normalized_tile_center_to_segment_center.y)) {
	// The raycast hit an east or west face
	return (normalized_tile_center_to_segment_center.x > 0) ? Tile::Side::EAST : Tile::Side::WEST;
  } else {
	// The raycast hit a north or south face
	return (normalized_tile_center_to_segment_center.y > 0) ? Tile::Side::SOUTH : Tile::Side::NORTH;
  }
}

std::optional<Raycast> raycast(sf::Vector2f origin,
							   float direction_deg,
							   float render_distance,
							   Grid &grid,
							   Tile::Symbol symbol_target) {
  // get closest intersection from pos to dir_deg with symbol_target horizontal_segments
  std::optional<Raycast> closest_raycast = std::nullopt;

  // iterate over each tile
  for (const Tile &tile : grid.tiles) {
	if (tile.symbol != symbol_target) {
	  continue;
	}

	// iterate over each segment of the tile
	for (const Line &horizontal_segment : tile.horizontal_segments) {
	  // get the intersection between the ray and the horizontal_segment
	  sf::Vector2f
		  ray_max_distance_pos = polar_to_cartesian(origin, render_distance, direction_deg);
	  Line ray = {origin, ray_max_distance_pos};

	  std::optional<sf::Vector2f>
		  intersection = get_segments_intersection(ray, horizontal_segment);

	  // if there is an intersection
	  if (intersection.has_value()) {
		float distance_to_intersection = get_magnitude(intersection.value() - origin);

		// if there is no closest intersection yet, or if the current intersection is closer than the closest intersection
		if (!closest_raycast.has_value()
			|| distance_to_intersection < closest_raycast.value().distance) {
		  sf::Vector2f local_intersection =
			  {intersection.value().x - (float)tile.pos.x, intersection.value().y - (float)tile.pos.y};

		  Tile::Side hit_side = determine_hit_side(grid.tile_size, tile, horizontal_segment);

		  // set the closest intersection to the current intersection
		  closest_raycast = Raycast{
			  distance_to_intersection,
			  intersection.value(),
			  local_intersection,
			  hit_side,
			  horizontal_segment,
			  tile,
		  };
		}
	  }
	}
  }

  return closest_raycast;
}

std::vector<ComputedDrawHit> compute_partial_walls_raycast_vec(
														GameManager &game_manager,
														int start_x,
														int end_x,
														int ray_thickness) {
  std::vector<ComputedDrawHit> computed_draw_hit_vec;
  // only need to change the texture_percentage_coords for each raycast
  ComputedDrawHit
	  computed_draw_hit = ComputedDrawHit(Tile::Symbol::WALL, {0, 0});

  int field_height = game_manager.window.getSize().y - game_manager.hud.height;

  // for each pixel in width, cast a ray and draw a vertical line
  for (int screen_x = start_x; screen_x < end_x; screen_x += ray_thickness) {
	// calculate the angle of the ray
	float rotation_step_deg = game_manager.camera.fov_horizontal_deg / (float)game_manager.window.getSize().x;
	float
		ray_angle_deg =
		game_manager.player.dir_deg - game_manager.camera.fov_horizontal_deg / 2 + rotation_step_deg * (float)screen_x;
	float ray_angle_diff_deg = ray_angle_deg - game_manager.player.dir_deg;
	float fish_eye_correction = cosf(degrees_to_radians(ray_angle_diff_deg));

	// cast the ray
	std::optional<Raycast>
		raycast_wall = raycast(game_manager.player.pos,
							   ray_angle_deg,
							   game_manager.camera.render_distance,
							   game_manager.grid,
							   Tile::Symbol::WALL);
	if (raycast_wall.has_value()) {
	  float raycast_distance_corrected = raycast_wall.value().distance * fish_eye_correction;

	  // the height to draw
	  int column_height = (int)((float)field_height * (game_manager.grid.tile_size / raycast_distance_corrected));

	  // calculate the 'x' of texture_percentage_coords
	  if (raycast_wall.value().hit_side == Tile::Side::SOUTH) {
		computed_draw_hit.texture_percentage_coords.x =
			raycast_wall.value().local_intersection.x / game_manager.grid.tile_size;
	  } else if (raycast_wall.value().hit_side == Tile::Side::NORTH) {
		computed_draw_hit.texture_percentage_coords.x =
			(1 - raycast_wall.value().local_intersection.x / game_manager.grid.tile_size);
	  } else if (raycast_wall.value().hit_side == Tile::Side::WEST) {
		computed_draw_hit.texture_percentage_coords.x =
			raycast_wall.value().local_intersection.y / game_manager.grid.tile_size;
	  } else if (raycast_wall.value().hit_side == Tile::Side::EAST) {
		computed_draw_hit.texture_percentage_coords.x =
			(1 - raycast_wall.value().local_intersection.y / game_manager.grid.tile_size);
	  }

	  // for each pixel in height, draw the corresponding pixel of the texture
	  for (int column_y = 0; column_y < column_height; column_y += ray_thickness) {
		// calculate the 'y' of texture_percentage_coords
		computed_draw_hit.texture_percentage_coords.y = (float)column_y / (float)column_height;

		// calculate the position of the pixel in the field
		int field_y = (field_height - column_height) / 2 + column_y;

		computed_draw_hit.pixel_pos = {screen_x, field_y};

		if (field_y > field_height) {
		  break;
		}

		// store the computed raycast for the given pixel
		computed_draw_hit_vec.emplace_back(computed_draw_hit);
	  }
	}
  }

  return computed_draw_hit_vec;
}

std::vector<ComputedDrawHit> compute_partial_floor_raycast_vec(GameManager &game_manager,
														int start_y,
														int end_y,
														int ray_thickness) {
  std::vector<ComputedDrawHit> computed_draw_hit_vec;

  // need to change the texture_percentage_coords for each raycast
  ComputedDrawHit
	  computed_draw_hit = ComputedDrawHit(Tile::Symbol::FLOOR, {0, 0});

  int field_width = game_manager.window.getSize().x;
  int field_height = game_manager.window.getSize().y - game_manager.hud.height;
  int half_field_height = field_height / 2;

  // for each pixel in height, cast a ray and draw a horizontal line
  for (int screen_y = start_y; screen_y < end_y; screen_y += ray_thickness) {
	// calculate the angle of the ray
	float vertical_camera_rotation_step_deg = game_manager.camera.fov_vertical_deg / (float)field_height;
	float vertical_ray_angle_deg =
		vertical_camera_rotation_step_deg * (float)screen_y - game_manager.camera.fov_vertical_deg / 2;

	// for each pixel of the horizontal line cast an imaginary ray using trigonometry
	for (int field_x = 0; field_x < field_width; field_x += ray_thickness) {
	  computed_draw_hit.pixel_pos = {field_x, screen_y};

	  // TODO: fix optimization (not working because walls are scanned left to right and floor is scanned top to bottom)
	  // skip if the pixel has already been computed (by the walls raycast)
//	  if (raycast_vec.find(std::make_pair(field_x, screen_y)) != raycast_vec.end()) {
//		continue;
//	  }

	  // calculate the angle of the ray
	  float horizontal_camera_rotation_step_deg = game_manager.camera.fov_horizontal_deg / (float)field_width;
	  float
		  horizontal_ray_angle_deg = game_manager.player.dir_deg - game_manager.camera.fov_horizontal_deg / 2
		  + horizontal_camera_rotation_step_deg * (float)field_x;

	  // compute distance to the point above the floor intersection
	  // <!> this is not the distance to the floor intersection
	  // <!> care for division by zero (tangent)
	  // tangent(theta) = opposite / adjacent
	  // adjacent is distance_to_point_above_floor
	  // opposite is the camera height
	  // camera_height / tangent(theta) = distance_to_point_above_floor
	  // if tangent is equal to zero, then the distance is infinite, replace it with maximum render distance
	  float distance_to_point_above_floor;
	  float tangent_vertical_ray = tanf(degrees_to_radians(vertical_ray_angle_deg));

	  if (tangent_vertical_ray == 0) {
		distance_to_point_above_floor = game_manager.camera.render_distance;
	  } else {
		distance_to_point_above_floor = game_manager.camera.height / tangent_vertical_ray;
	  }

	  // find floor coordinates by going in horizontal direction for the found distance
	  sf::Vector2f floor_intersection_pos = polar_to_cartesian(game_manager.player.pos,
															   distance_to_point_above_floor,
															   horizontal_ray_angle_deg);

	  if (floor_intersection_pos.x<0 || floor_intersection_pos.x>(float)
		game_manager.grid.width * game_manager.grid.tile_size ||
			floor_intersection_pos.y<0 || floor_intersection_pos.y>(float)
	  game_manager.grid.height * game_manager.grid.tile_size) {
		continue;
	  }

	  // find the texture coordinates
	  sf::Vector2f local_intersection =
		  floor_intersection_pos - sf::Vector2f(
			  floor(floor_intersection_pos.x / game_manager.grid.tile_size) * game_manager.grid.tile_size,
			  floor(floor_intersection_pos.y / game_manager.grid.tile_size) * game_manager.grid.tile_size);

	  // calculate the 'x' & 'y' of texture_percentage_coords
	  computed_draw_hit.texture_percentage_coords.x =
		  local_intersection.x / game_manager.grid.tile_size;
	  computed_draw_hit.texture_percentage_coords.y =
		  local_intersection.y / game_manager.grid.tile_size;


	  // store the computed raycast for the given FLOOR pixel
	  computed_draw_hit_vec.emplace_back(computed_draw_hit);
	}
  }

  return computed_draw_hit_vec;
}

std::vector<ComputedDrawHit> &compute_walls_raycast_vec(std::vector<ComputedDrawHit> &raycast_vec,
														GameManager &game_manager,
														int field_width,
														int field_height,
														int ray_thickness) {
  // only need to change the texture_percentage_coords for each raycast
  ComputedDrawHit
	  computed_draw_hit = ComputedDrawHit(Tile::Symbol::WALL, {0, 0});

  // for each pixel in width, cast a ray and draw a vertical line
  for (int field_x = 0; field_x < field_width; field_x += ray_thickness) {
	// calculate the angle of the ray
	float rotation_step_deg = game_manager.camera.fov_horizontal_deg / (float)field_width;
	float
		ray_angle_deg =
		game_manager.player.dir_deg - game_manager.camera.fov_horizontal_deg / 2 + rotation_step_deg * (float)field_x;
	float ray_angle_diff_deg = ray_angle_deg - game_manager.player.dir_deg;
	float fish_eye_correction = cosf(degrees_to_radians(ray_angle_diff_deg));

	// cast the ray
	std::optional<Raycast>
		raycast_wall = raycast(game_manager.player.pos,
							   ray_angle_deg,
							   game_manager.camera.render_distance,
							   game_manager.grid,
							   Tile::Symbol::WALL);
	if (raycast_wall.has_value()) {
	  float raycast_distance_corrected = raycast_wall.value().distance * fish_eye_correction;

	  // the height to draw
	  int column_height = (int)((float)field_height * (game_manager.grid.tile_size / raycast_distance_corrected));

	  // calculate the 'x' of texture_percentage_coords
	  if (raycast_wall.value().hit_side == Tile::Side::SOUTH) {
		computed_draw_hit.texture_percentage_coords.x =
			raycast_wall.value().local_intersection.x / game_manager.grid.tile_size;
	  } else if (raycast_wall.value().hit_side == Tile::Side::NORTH) {
		computed_draw_hit.texture_percentage_coords.x =
			(1 - raycast_wall.value().local_intersection.x / game_manager.grid.tile_size);
	  } else if (raycast_wall.value().hit_side == Tile::Side::WEST) {
		computed_draw_hit.texture_percentage_coords.x =
			raycast_wall.value().local_intersection.y / game_manager.grid.tile_size;
	  } else if (raycast_wall.value().hit_side == Tile::Side::EAST) {
		computed_draw_hit.texture_percentage_coords.x =
			(1 - raycast_wall.value().local_intersection.y / game_manager.grid.tile_size);
	  }

	  // for each pixel in height, draw the corresponding pixel of the texture
	  for (int column_y = 0; column_y < column_height; column_y += ray_thickness) {
		// calculate the 'y' of texture_percentage_coords
		computed_draw_hit.texture_percentage_coords.y = (float)column_y / (float)column_height;

		// calculate the position of the pixel in the field
		int field_y = (field_height - column_height) / 2 + column_y;

		computed_draw_hit.pixel_pos = {field_x, field_y};

		if (field_y > field_height) {
		  break;
		}

		/*// double loop for ray_thickness
		for (int ray_thickness_x = 0; ray_thickness_x < ray_thickness; ray_thickness_x++) {
		  for (int ray_thickness_y = 0; ray_thickness_y < ray_thickness; ray_thickness_y++) {
			// store the computed raycast for the given pixel
			raycast_vec.emplace(std::make_pair(field_x + ray_thickness_x, field_y + ray_thickness_y),
								computed_draw_hit);
		  }
		}*/

		// store the computed raycast for the given pixel
		raycast_vec.emplace_back(computed_draw_hit);
	  }
	}
  }

  return raycast_vec;
}

std::vector<ComputedDrawHit> &compute_floor_raycast_vec(std::vector<ComputedDrawHit> &raycast_vec,
														GameManager &game_manager,
														int field_width,
														int field_height,
														int ray_thickness) {
  // need to change the texture_percentage_coords for each raycast
  ComputedDrawHit
	  computed_draw_hit = ComputedDrawHit(Tile::Symbol::FLOOR, {0, 0});

  int half_field_height = field_height / 2;

  // for each pixel in height, cast a ray and draw a horizontal line
  for (int field_y = half_field_height; field_y < field_height; field_y += ray_thickness) {
	// calculate the angle of the ray
	float vertical_camera_rotation_step_deg = game_manager.camera.fov_vertical_deg / (float)field_height;
	float vertical_ray_angle_deg =
		vertical_camera_rotation_step_deg * (float)field_y - game_manager.camera.fov_vertical_deg / 2;

	// for each pixel of the horizontal line cast an imaginary ray using trigonometry
	for (int field_x = 0; field_x < field_width; field_x += ray_thickness) {
	  computed_draw_hit.pixel_pos = {field_x, field_y};

	  // TODO: fix optimization (not working because walls are scanned left to right and floor is scanned top to bottom)
	  // skip if the pixel has already been computed (by the walls raycast)
//	  if (raycast_vec.find(std::make_pair(field_x, field_y)) != raycast_vec.end()) {
//		continue;
//	  }

	  // calculate the angle of the ray
	  float horizontal_camera_rotation_step_deg = game_manager.camera.fov_horizontal_deg / (float)field_width;
	  float
		  horizontal_ray_angle_deg = game_manager.player.dir_deg - game_manager.camera.fov_horizontal_deg / 2
		  + horizontal_camera_rotation_step_deg * (float)field_x;

	  // compute distance to the point above the floor intersection
	  // <!> this is not the distance to the floor intersection
	  // <!> care for division by zero (tangent)
	  // tangent(theta) = opposite / adjacent
	  // adjacent is distance_to_point_above_floor
	  // opposite is the camera height
	  // camera_height / tangent(theta) = distance_to_point_above_floor
	  // if tangent is equal to zero, then the distance is infinite, replace it with maximum render distance
	  float distance_to_point_above_floor;
	  float tangent_vertical_ray = tanf(degrees_to_radians(vertical_ray_angle_deg));

	  if (tangent_vertical_ray == 0) {
		distance_to_point_above_floor = game_manager.camera.render_distance;
	  } else {
		distance_to_point_above_floor = game_manager.camera.height / tangent_vertical_ray;
	  }

	  // find floor coordinates by going in horizontal direction for the found distance
	  sf::Vector2f floor_intersection_pos = polar_to_cartesian(game_manager.player.pos,
															   distance_to_point_above_floor,
															   horizontal_ray_angle_deg);

	  if (floor_intersection_pos.x<0 || floor_intersection_pos.x>(float)
		game_manager.grid.width * game_manager.grid.tile_size ||
			floor_intersection_pos.y<0 || floor_intersection_pos.y>(float)
	  game_manager.grid.height * game_manager.grid.tile_size) {
		continue;
	  }

	  // find the texture coordinates
	  sf::Vector2f local_intersection =
		  floor_intersection_pos - sf::Vector2f(
			  floor(floor_intersection_pos.x / game_manager.grid.tile_size) * game_manager.grid.tile_size,
			  floor(floor_intersection_pos.y / game_manager.grid.tile_size) * game_manager.grid.tile_size);

	  // calculate the 'x' & 'y' of texture_percentage_coords
	  computed_draw_hit.texture_percentage_coords.x =
		  local_intersection.x / game_manager.grid.tile_size;
	  computed_draw_hit.texture_percentage_coords.y =
		  local_intersection.y / game_manager.grid.tile_size;


	  // store the computed raycast for the given FLOOR pixel
	  raycast_vec.emplace_back(computed_draw_hit);
	}
  }

  return raycast_vec;
}