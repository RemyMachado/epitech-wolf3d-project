#include "movement.hpp"
#include "my_math.hpp"

sf::Vector2f move(sf::Vector2f pos, float direction_deg, float distance) {
    float direction_rad = degrees_to_radians(direction_deg);

    return {pos.x + distance * cosf(direction_rad), pos.y + distance * sinf(direction_rad)};
}
