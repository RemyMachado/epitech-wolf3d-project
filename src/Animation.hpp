#ifndef EPITECH_WOLF3D_PROJECT_SRC_ANIMATION_HPP_
#define EPITECH_WOLF3D_PROJECT_SRC_ANIMATION_HPP_

#include <SFML/Graphics.hpp>
#include <utility>
#include "Timer.hpp"
#include "SpriteSetting.hpp"
#include "managers/TextureManager.hpp"

struct AnimationParams {
  SpriteSetting sprite_setting;
  float duration;
};

class Animation {
  using OnAnimationEnd = std::function<void()>;

 private:
  sf::Sprite sprite;
  Timer frame_timer;
  sf::Vector2i frame_size;
  int frame_count;
  int current_frame;
  sf::Vector2i initial_offset;
  sf::Vector2i frame_offset;
  OnAnimationEnd on_animation_end;

 public:
  explicit Animation(const AnimationParams &animation_params, OnAnimationEnd on_animation_end = nullptr)
	  : sprite(TextureManager::get_instance().get_texture(animation_params.sprite_setting.id)),
		frame_size(animation_params.sprite_setting.frame_size),
		frame_count(animation_params.sprite_setting.frame_count),
		current_frame(0),
		initial_offset(animation_params.sprite_setting.initial_offset),
		frame_offset(animation_params.sprite_setting.frame_offset),
		frame_timer(animation_params.duration / (float)animation_params.sprite_setting.frame_count),
		on_animation_end(std::move(on_animation_end)) {
	update_texture_rect();
  }

  void update_sprite() {
	if (!frame_timer.check_is_elapsed()) {
	  return;
	}
	current_frame += 1;
	if (current_frame >= frame_count) {
	  current_frame = 0;
	  if (on_animation_end) {
		on_animation_end();
	  }
	}
	update_texture_rect();
  }

  sf::Sprite &getSprite() {
	return sprite;
  }

  void reset_animation() {
	current_frame = 0;
  }

  void draw(sf::RenderWindow &window) const {
	window.draw(sprite);
  }

 private:
  void update_texture_rect() {
	sf::Vector2i position = initial_offset + frame_offset * current_frame;

	sprite.setTextureRect(sf::IntRect(position.x, position.y, frame_size.x, frame_size.y));
  }
};

#endif //EPITECH_WOLF3D_PROJECT_SRC_ANIMATION_HPP_
