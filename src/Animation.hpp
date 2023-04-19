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

struct AnimationCallbacks {
  std::function<void()> on_animation_end = nullptr;
  std::function<void()> on_animation_update = nullptr;
};

class Animation {
 private:
  SpriteId sprite_id;
  sf::Sprite sprite;
  Timer frame_timer;
  sf::Vector2i frame_size;
  int frame_count;
  int current_frame;
  sf::Vector2i initial_offset;
  sf::Vector2i frame_offset;
  std::function<void()> on_animation_update;
  std::function<void()> on_animation_end;
  bool is_paused = false;

 public:
  explicit Animation(const AnimationParams &animation_params, AnimationCallbacks animation_callbacks = {});

 public:
  void update_sprite();
  void pause_animation();
  void resume_animation();
  int get_frame_count() const;
  sf::Sprite &getSprite();
  void reset_animation();
  void draw(sf::RenderWindow &window) const;
  void force_texture_rect(const sf::IntRect &rect);
  sf::IntRect get_last_frame_texture_rect() const;

 private:
  sf::IntRect get_texture_rect(int frame) const;
  void update_texture_rect();
};

#endif //EPITECH_WOLF3D_PROJECT_SRC_ANIMATION_HPP_
