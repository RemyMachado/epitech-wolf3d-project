#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>
#include "game_loop.hpp"
#include "lines.hpp"

const int WINDOW_WIDTH = 1920;
const int WINDOW_HEIGHT = 1080;

void run_game() {
    float circleRadius = 5.f;
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "SFML works!");
    sf::CircleShape circleShape(circleRadius);
    circleShape.setFillColor(sf::Color::White);

    // hide the mouse cursor
    window.setMouseCursorVisible(false);

    srand(time(NULL));

    // draw 5 random segments on the screen that don't change between frames
    sf::VertexArray lines(sf::Lines, 10);
    for (int i = 0; i < 10; i += 2) {
        lines[i].position = sf::Vector2f(rand() % WINDOW_WIDTH, rand() % WINDOW_HEIGHT);
        lines[i + 1].position = sf::Vector2f(rand() % WINDOW_WIDTH, rand() % WINDOW_HEIGHT);
        lines[i].color = sf::Color::Red;
        lines[i + 1].color = sf::Color::Red;
    }


    while (window.isOpen()) {
        // print mouse coordinates
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);

        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed ||
                (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape))
                window.close();
        }

        window.clear();
        // draw circleShape centered at mouse position
        circleShape.setPosition(mousePos.x - circleRadius, mousePos.y - circleRadius);
        window.draw(circleShape);
        window.draw(lines);

        // draw rays coming out from mouse position in every 10 degrees angle
        for (int i = 0; i < 360; i += 10) {
            // retrieve the first point of intersection between the ray and the lines using `get_segments_intersecting`
            std::vector<sf::Vector2f> intersections;
            for (int j = 0; j < 10; j += 2) {
                std::optional<sf::Vector2f> intersection = get_segments_intersection(
                        sf::Vector2f(mousePos.x, mousePos.y),
                        sf::Vector2f(mousePos.x + 10000 * cos(i * 3.14159265 / 180),
                                     mousePos.y + 10000 * sin(i * 3.14159265 / 180)),
                        lines[j].position,
                        lines[j + 1].position
                );
                if (intersection.has_value()) {
                    intersections.push_back(intersection.value());
                }
            }
            // keep the closest point of intersection
            std::optional<sf::Vector2f> closestIntersection;
            for (auto intersection: intersections) {
                if (!closestIntersection.has_value()) {
                    closestIntersection = intersection;
                } else {
                    if (sqrt(pow(mousePos.x - intersection.x, 2) + pow(mousePos.y - intersection.y, 2)) <
                        sqrt(pow(mousePos.x - closestIntersection.value().x, 2) +
                             pow(mousePos.y - closestIntersection.value().y, 2))) {
                        closestIntersection = intersection;
                    }
                }
            }
            // draw line from mouse position to the closest point of intersection
            if (closestIntersection.has_value()) {
                sf::Vertex line[] =
                        {
                                sf::Vertex(sf::Vector2f(mousePos.x, mousePos.y)),
                                sf::Vertex(sf::Vector2f(closestIntersection.value().x, closestIntersection.value().y))
                        };
                window.draw(line, 2, sf::Lines);
            } else {
                sf::Vertex line[] =
                        {
                                sf::Vertex(sf::Vector2f(mousePos.x, mousePos.y)),
                                sf::Vertex(sf::Vector2f(mousePos.x + 10000 * cos(i * 3.14159265 / 180),
                                                        mousePos.y + 10000 * sin(i * 3.14159265 / 180)))
                        };
                window.draw(line, 2, sf::Lines);
            }
        }

        window.display();
    }
}