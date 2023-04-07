#include <cmath>
#include <SFML/System/Vector2.hpp>

#ifndef EPITECH_WOLF3D_PROJECT_MY_MATH_HPP
#define EPITECH_WOLF3D_PROJECT_MY_MATH_HPP

float degrees_to_radians(float degrees);
float magnitude(sf::Vector2f vec);
sf::Vector2f polar_to_cartesian(sf::Vector2f origin, float magnitude, float angle_deg);

#endif //EPITECH_WOLF3D_PROJECT_MY_MATH_HPP
