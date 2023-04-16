#include "my_math.hpp"
#include "lines.hpp"

float degrees_to_radians(float degrees) {
  return degrees * M_PI / 180;
}

float get_magnitude(sf::Vector2f vec) {
  return sqrtf(vec.x * vec.x + vec.y * vec.y);
}

float dot_product(const sf::Vector2f &a, const sf::Vector2f &b) {
  return a.x * b.x + a.y * b.y;
}

sf::Vector2f normalize_vector(const sf::Vector2f &vector) {
  float magnitude = get_magnitude(vector);

  return {vector.x / magnitude, vector.y / magnitude};
}

sf::Vector2f normalize_line(const Line &line) {
  return normalize_vector(line.end - line.start);
}

sf::Vector2f polar_to_cartesian(sf::Vector2f origin, float magnitude, float angle_deg) {
  float angle_rad = degrees_to_radians(angle_deg);
  return {origin.x + magnitude * cosf(angle_rad), origin.y + magnitude * sinf(angle_rad)};
}

float get_angle_between_vectors(const sf::Vector2f &a, const sf::Vector2f &b) {
  float dot = dot_product(a, b);
  float magnitude_a = get_magnitude(a);
  float magnitude_b = get_magnitude(b);

  return acosf(dot / degrees_to_radians(magnitude_a * magnitude_b));
}

float get_distance_between_points(const sf::Vector2f &a, const sf::Vector2f &b) {
  return sqrtf(powf(a.x - b.x, 2) + powf(a.y - b.y, 2));
}
