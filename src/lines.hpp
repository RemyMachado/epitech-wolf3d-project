#include <SFML/System/Vector2.hpp>
#include <optional>

#ifndef EPITECH_WOLF3D_PROJECT_LINES_HPP
#define EPITECH_WOLF3D_PROJECT_LINES_HPP

struct Line {
  sf::Vector2f start;
  sf::Vector2f end;
};

std::optional<sf::Vector2f>
get_segments_intersection(Line line1, Line line2);

#endif //EPITECH_WOLF3D_PROJECT_LINES_HPP
