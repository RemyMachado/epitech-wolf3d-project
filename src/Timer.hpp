#ifndef EPITECH_WOLF3D_PROJECT_SRC_TIMER_HPP_
#define EPITECH_WOLF3D_PROJECT_SRC_TIMER_HPP_

#include <SFML/System.hpp>

class Timer {
 private:
  float interval;
  float elapsedTime;
  sf::Clock clock;

 public:
  explicit Timer(float interval) : interval(interval), elapsedTime(0.0f), clock() {}

  bool check_is_elapsed(float additional_time = 0.0f) {
	float deltaTime = clock.restart().asSeconds();

	elapsedTime += deltaTime;

	if (elapsedTime >= interval + additional_time) {
	  reset();
	  return true;
	}

	return false;
  }

  void reset() {
	elapsedTime = 0.0f;
  }

  void setInterval(float newInterval) {
	interval = newInterval;
  }
};

#endif //EPITECH_WOLF3D_PROJECT_SRC_TIMER_HPP_
