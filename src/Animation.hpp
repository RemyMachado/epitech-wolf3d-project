#ifndef EPITECH_WOLF3D_PROJECT_SRC_ANIMATION_HPP_
#define EPITECH_WOLF3D_PROJECT_SRC_ANIMATION_HPP_

#include <SFML/Graphics.hpp>
#include <utility>
#include "Timer.hpp"

struct AnimationParams {
  const sf::Texture &texture;
  sf::Vector2i frameSize;
  int frameCount;
  sf::Vector2i initial_offset;
  sf::Vector2i frame_offset;
  float frame_interval;
};

class Animation {
  using OnAnimationEnd = std::function<void()>;

 private:
  sf::Sprite sprite;
  Timer timer;
  sf::Vector2i frameSize;
  int frameCount;
  int currentFrame;
  sf::Vector2i initial_offset;
  sf::Vector2i frame_offset;
  OnAnimationEnd on_animation_end;

 public:
  explicit Animation(AnimationParams animation_params, OnAnimationEnd on_animation_end = nullptr)
	  : sprite(animation_params.texture),
		frameSize(animation_params.frameSize),
		frameCount(animation_params.frameCount),
		currentFrame(0),
		initial_offset(animation_params.initial_offset),
		frame_offset(animation_params.frame_offset),
		timer(animation_params.frame_interval),
		on_animation_end(std::move(on_animation_end)) {
	update_texture_rect();
  }

  void update_sprite() {
	if (!timer.check_is_elapsed()) {
	  return;
	}
	currentFrame += 1;
	if (currentFrame >= frameCount) {
	  currentFrame = 0;
	  if (on_animation_end) {
		on_animation_end();
	  }
	}
	update_texture_rect();
  }

  sf::Sprite &getSprite() {
	return sprite;
  }

  void draw(sf::RenderWindow &window) const {
	window.draw(sprite);
  }

 private:
  void update_texture_rect() {
	sf::Vector2i position = initial_offset + frame_offset * currentFrame;

	sprite.setTextureRect(sf::IntRect(position.x, position.y, frameSize.x, frameSize.y));
  }
};

#endif //EPITECH_WOLF3D_PROJECT_SRC_ANIMATION_HPP_
