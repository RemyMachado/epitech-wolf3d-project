#include "my_math.hpp"

float degrees_to_radians(float degrees) {
  return degrees * M_PI / 180;
}

float magnitude(sf::Vector2f vec) {
  return sqrtf(vec.x * vec.x + vec.y * vec.y);
}

sf::Vector2f polar_to_cartesian(sf::Vector2f origin, float magnitude, float angle_deg) {
  float angle_rad = degrees_to_radians(angle_deg);
  return {origin.x + magnitude * cosf(angle_rad), origin.y + magnitude * sinf(angle_rad)};
}
