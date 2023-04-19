#include <iostream>
#include "my_math.hpp"
#include "lines.hpp"

float degrees_to_radians(float degrees) {
  return degrees * M_PI / 180;
}

float radians_to_degrees(float radians) {
  return radians * 180 / M_PI;
}

float get_magnitude(sf::Vector2f vec) {
  return sqrtf(vec.x * vec.x + vec.y * vec.y);
}

float dot_product(const sf::Vector2f &a, const sf::Vector2f &b) {
  return a.x * b.x + a.y * b.y;
}

float cross_product_2d(const sf::Vector2f &a, const sf::Vector2f &b) {
  return a.x * b.y - a.y * b.x;
}

/*
 * Returns a vector with the same direction as the given vector but with a magnitude of 1.
 * */
sf::Vector2f normalize_vector(const sf::Vector2f &vector) {
  float magnitude = get_magnitude(vector);

  return {vector.x / magnitude, vector.y / magnitude};
}

sf::Vector2f normalize_line(const Line &line) {
  return normalize_vector(line.end - line.start);
}

Polar cartesian_to_polar(const sf::Vector2f &origin, const sf::Vector2f &point) {
  float x = point.x - origin.x;
  float y = point.y - origin.y;

  float magnitude = sqrtf(x * x + y * y);
  float angle_rad = atan2f(y, x);

  return {magnitude, radians_to_degrees(angle_rad)};
}

sf::Vector2f polar_to_cartesian(sf::Vector2f origin, float magnitude, float angle_deg) {
  float angle_rad = degrees_to_radians(angle_deg);
  return {origin.x + magnitude * cosf(angle_rad), origin.y + magnitude * sinf(angle_rad)};
}

/*
 * Returns the angle between two vectors in degrees [-180, 180].
 * The angle is positive if the second vector is to the left of the initial vector.
 * */
float get_angle_between_vectors_deg(const sf::Vector2f &a, const sf::Vector2f &b) {
  sf::Vector2f normal_a = normalize_vector(a);
  sf::Vector2f normal_b = normalize_vector(b);
  float dot = dot_product(normal_a, normal_b);
  float magnitude_a = get_magnitude(normal_a);
  float magnitude_b = get_magnitude(normal_b);

  // Compute the cross product's z-component to determine the sign of the angle
  float cross_z = cross_product_2d(normal_a, normal_b);

  float angle_deg = radians_to_degrees(acosf(dot / magnitude_a * magnitude_b));

  // If the z-component of the cross product is positive, the second vector is to the left of the initial vector
  if (cross_z > 0) {
	return angle_deg;
  } else { // The second vector is to the right of the initial vector
	return -angle_deg;
  }
}

sf::Vector2f get_difference_vector(const sf::Vector2f &start, const sf::Vector2f &end) {
  return {end.x - start.x, end.y - start.y};
}

float get_distance_between_points(const sf::Vector2f &a, const sf::Vector2f &b) {
  return get_magnitude(get_difference_vector(a, b));
}

sf::Vector2f compute_dir_vector(float angle_deg) {
  return {std::cos(degrees_to_radians(angle_deg)), std::sin(degrees_to_radians(angle_deg))};
}

float random_float(Range<float> range) {
  float random_value = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);

  return range.min + random_value * (range.max - range.min);
}
float positive_angle_deg(float angle_deg) {
  return std::fmod(angle_deg + 360, 360);
}
