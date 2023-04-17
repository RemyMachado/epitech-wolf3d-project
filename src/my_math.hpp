#ifndef EPITECH_WOLF3D_PROJECT_MY_MATH_HPP
#define EPITECH_WOLF3D_PROJECT_MY_MATH_HPP

#include <cmath>
#include <SFML/System/Vector2.hpp>

class Line;

float degrees_to_radians(float degrees);
float radians_to_degrees(float radians);
float get_magnitude(sf::Vector2f vec);
sf::Vector2f polar_to_cartesian(sf::Vector2f origin, float magnitude, float angle_deg);
sf::Vector2f normalize_vector(const sf::Vector2f &vector);
sf::Vector2f normalize_line(const Line &line);
float dot_product(const sf::Vector2f &a, const sf::Vector2f &b);
float get_angle_between_vectors_deg(const sf::Vector2f &a, const sf::Vector2f &b);
float get_distance_between_points(const sf::Vector2f &a, const sf::Vector2f &b);
sf::Vector2f compute_dir_vector(float angle_deg);
sf::Vector2f get_difference_vector(const sf::Vector2f &start, const sf::Vector2f &end);

#endif //EPITECH_WOLF3D_PROJECT_MY_MATH_HPP
