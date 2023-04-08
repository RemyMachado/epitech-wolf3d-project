#include <cmath>
#include <SFML/System/Vector2.hpp>
#include "lines.hpp"

#ifndef EPITECH_WOLF3D_PROJECT_MY_MATH_HPP
#define EPITECH_WOLF3D_PROJECT_MY_MATH_HPP

float degrees_to_radians(float degrees);
float get_magnitude(sf::Vector2f vec);
sf::Vector2f polar_to_cartesian(sf::Vector2f origin, float magnitude, float angle_deg);
sf::Vector2f normalize_vector(const sf::Vector2f &vector);
sf::Vector2f normalize_line(const Line &line);
float dot_product(const sf::Vector2f &a, const sf::Vector2f &b);

#endif //EPITECH_WOLF3D_PROJECT_MY_MATH_HPP
