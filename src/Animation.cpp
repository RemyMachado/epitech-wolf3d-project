#include "Animation.hpp"

Animation::Animation(const AnimationParams &animation_params, AnimationCallbacks animation_callbacks)
	: sprite(TextureManager::get_instance().get_texture(animation_params.sprite_setting.id)),
	  frame_size(animation_params.sprite_setting.frame_size),
	  frame_count(animation_params.sprite_setting.frame_count),
	  current_frame(0),
	  initial_offset(animation_params.sprite_setting.initial_offset),
	  frame_offset(animation_params.sprite_setting.frame_offset),
	  frame_timer(animation_params.duration / (float)animation_params.sprite_setting.frame_count),
	  on_animation_update(std::move(animation_callbacks.on_animation_update)),
	  on_animation_end(std::move(animation_callbacks.on_animation_end)),
	  sprite_id(animation_params.sprite_setting.id) {
  update_texture_rect();
}
void Animation::update_sprite() {
  if (is_paused || !frame_timer.check_is_elapsed()) {
	return;
  }
  if (on_animation_update) {
	on_animation_update();
  }
  if (current_frame >= frame_count) {
	current_frame = 0;
	if (on_animation_end) {
	  on_animation_end();
	}
  }
  // can be paused during on_animation_end
  if (is_paused) {
	return;
  }
  update_texture_rect();
  current_frame += 1;
}
sf::Sprite &Animation::getSprite() {
  return sprite;
}
void Animation::reset_animation() {
  current_frame = 0;
}
void Animation::draw(sf::RenderWindow &window) const {
  window.draw(sprite);
}

sf::IntRect Animation::get_texture_rect(int frame) const {
  sf::Vector2i position = initial_offset + frame_offset * frame;

  return {position.x, position.y, frame_size.x, frame_size.y};
}
sf::IntRect Animation::get_last_frame_texture_rect() const {
  return get_texture_rect(frame_count - 1);
}
void Animation::update_texture_rect() {
  sprite.setTextureRect(get_texture_rect(current_frame));
}
void Animation::force_texture_rect(const sf::IntRect &rect) {
  sprite.setTextureRect(rect);
  pause_animation();
}
void Animation::pause_animation() {
  is_paused = true;
}
void Animation::resume_animation() {
  is_paused = false;
}
int Animation::get_frame_count() const {
  return frame_count;
}
