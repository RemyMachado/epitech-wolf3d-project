#include <iostream>
#include <optional>
#include "game_loop.hpp"
#include "lines.hpp"

int main() {
    std::cout << "Hello, World!!!" << std::endl;

    // test function from lines.hpp
    sf::Vector2f start_pos1(3, 1);
    sf::Vector2f end_pos1(7, 2);
    sf::Vector2f start_pos2(0, 5);
    sf::Vector2f end_pos2(10, 5);

    // print pos1 - pos2 using minus operator on sf::Vector2f
    sf::Vector2f result = start_pos1 - end_pos1;
    std::cout << "result: (" << result.x << ", " << result.y << ")" << std::endl;

//    std::optional<sf::Vector2f> intersection = get_segments_intersecting(start_pos1, end_pos1, start_pos2, end_pos2);
//    if (intersection.has_value()) {
//        std::cout << "Intersection found at (" << intersection.value().x << ", " << intersection.value().y << ")"
//                  << std::endl;
//    } else {
//        std::cout << "No intersection found" << std::endl;
//    }

    run_game();

    return EXIT_SUCCESS;
}
