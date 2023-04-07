#include <SFML/System/Vector2.hpp>

#ifndef EPITECH_WOLF3D_PROJECT_LINES_HPP
#define EPITECH_WOLF3D_PROJECT_LINES_HPP

std::optional<sf::Vector2f>
get_segments_intersection(sf::Vector2f start_pos1, sf::Vector2f end_pos1, sf::Vector2f start_pos2,
                          sf::Vector2f end_pos2);

#endif //EPITECH_WOLF3D_PROJECT_LINES_HPP
