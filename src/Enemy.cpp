#include "Grid.hpp"
#include "Player.hpp"
#include "Enemy.hpp"
#include "my_math.hpp"
#include "raycasting.hpp"
#include "managers/GameManager.hpp"

std::ostream &operator<<(std::ostream &os, const Enemy &enemy) {
  os << "Enemy - " << (enemy.symbol == Tile::Symbol::ENEMY_DOG ? "DOG" : "GUARD") << "(" << enemy.tile_coords.x << ", "
     << enemy.tile_coords.y << ")";

  return os;
}
void Enemy::update(Player &player, PathFinder &path_finder) {
//  if (is_dead) {
//	return;
//  }
  try_attack(player);
  try_move(player, path_finder);
  update_sprites();
}

sf::Sprite &Enemy::get_current_sprite() {
  if (is_attacking) {
    return attack_animation.getSprite();
  }
  if (is_dying || is_dead) {
    return death_animation.getSprite();
  }
  if (is_hurting && hurt_animation.has_value()) {
    return hurt_animation.value().getSprite();
  }
  if (is_walking) {
    return walk_animation.getSprite();
  }
  return idle_sprite;
}

std::optional<Animation> createOptionalAnimation(const std::optional<AnimationParams> &params,
                                                 const std::function<void()> &onAnimationEnd) {
  if (params.has_value()) {
    return Animation(params.value(), {onAnimationEnd});
  } else {
    return std::nullopt;
  }
}

Enemy::Enemy(const EnemySetting &setting,
             const Grid &grid,
             sf::Vector2i initial_coords,
             sf::Vector2f initial_pos,
             GameManager &game_manager)
    : tile_coords(initial_coords),
      pos(initial_pos),
      symbol(setting.symbol),
      walk_animation(setting.walk_animation_params,
                     {[this]() { this->end_walk_anim(); },
                      [this]() { this->update_transitory_walk_position(); }}),
      attack_animation(setting.attack_animation_params, {[this]() { this->end_attack_anim(); }}),
      death_animation(setting.death_animation_params, {[this]() { this->end_death_anim(); }}),
      hurt_animation(setting.hurt_animation_params.has_value()
                     ? std::optional<Animation>(Animation(setting.hurt_animation_params.value(),
                                                          {[this]() { this->end_hurt_anim(); }}))
                     : std::nullopt),
      attack_delay(random_float(setting.attack_delay_range)),
      attack_timer(attack_delay),
      attack_damage(setting.attack_damage),
      grid(grid),
      attack_tile_range(setting.attack_tile_range),
      health(setting.health),
      attack_sound_id(setting.attack_sound_id),
      hurt_sound_id(setting.hurt_sound_id.has_value() ? std::optional<SoundId>(setting.hurt_sound_id.value())
                                                      : std::nullopt),
      death_sound_id(setting.death_sound_id),
      move_speed(random_float(setting.move_speed_range)),
      game_manager(game_manager),
      score_reward(setting.score_reward) {
  // TODO: generic loader
  SpriteSetting idle_sprite_setting = SPRITE_SETTINGS.at(setting.idle_sprite_id);

  idle_sprite.setTexture(TextureManager::get_instance().get_texture(setting.idle_sprite_id));
  idle_sprite.setTextureRect(sf::IntRect(
      idle_sprite_setting.initial_offset.x,
      idle_sprite_setting.initial_offset.y,
      idle_sprite_setting.frame_size.x,
      idle_sprite_setting.frame_size.y));

  std::cout << "Enemy created: (" << pos.x << ", " << pos.y << ")" << std::endl;
}
void Enemy::update_idle_animation() {
}
void Enemy::update_transitory_walk_position() {
  sf::Vector2f additional_pos = walk_delta_per_frame * move_speed;
  move(pos + additional_pos);
}
void Enemy::update_walk_animation() {
  if (is_walking) {
    walk_animation.update_sprite();
  }
}
void Enemy::update_hurt_animation() {
  if (is_hurting && hurt_animation.has_value()) {
    hurt_animation.value().update_sprite();
  }
}
void Enemy::update_attack_animation() {
  if (is_attacking) {
    attack_animation.update_sprite();
  }
}
void Enemy::update_death_animation() {
  if (is_dying) {
    death_animation.update_sprite();
  }
}
void Enemy::update_sprites() {
  update_idle_animation();
  update_attack_animation();
  update_death_animation();
  update_hurt_animation();
  update_walk_animation();
}
/*
 * Returns true if the player is in range and there is no wall blocking the attack
 * */
bool Enemy::is_player_in_range(const Player &player) {
  // retrieve distance to player,
  // if the player is in range, check with a raycast if there is a wall blocking the attack

  Polar polar_vector_to_player = cartesian_to_polar(pos, player.get_pos());

  if (polar_vector_to_player.magnitude > attack_tile_range * grid.tile_size) {
    return false;
  }
  auto raycast_result =
      raycast(pos, polar_vector_to_player.angle_deg, polar_vector_to_player.magnitude, grid, UNWALKABLE_TILES);

  return !raycast_result.has_value();
}
void Enemy::try_move(Player &player, PathFinder &path_finder) {
  if (get_is_busy()) {
    return;
  }

  int stop_at_weight = std::max((int) attack_tile_range / 2, 1);
  auto shortest_path =
      path_finder.get_shortest_path(tile_coords,
                                    player.get_tile_coords(),
                                    UNWALKABLE_TILES,
                                    stop_at_weight);

  if (!shortest_path.empty()) {
    is_walking = true;
    walk_animation.reset_animation();
    sf::Vector2f target_walk_pos = grid.get_tile_center(shortest_path.front());
    sf::Vector2f vector_to_target = target_walk_pos - pos;

    // divide vector_to_target by the number of frame
    // to get the vector to move per frame
    walk_delta_per_frame = vector_to_target / (float) walk_animation.get_frame_count();
  }
}
void Enemy::try_attack(Player &player) {
  if (get_is_busy() || !attack_timer.check_is_elapsed()) {
    return;
  }
  if (!is_player_in_range(player)) {
    return;
  }
  is_attacking = true;
  attack_animation.reset_animation();
  SoundManager::get_instance().play_sound(attack_sound_id);
  player.take_damage(attack_damage);
}

Enemy::~Enemy() {
  std::cout << "______________________________________________________________________"
               "Enemy destroyed: (" << pos.x << ", " << pos.y << ")" << std::endl;
}
bool Enemy::get_is_busy() const {
  return is_walking || is_attacking || is_dying || is_hurting || is_dead;
}
void Enemy::take_damage(float damage, Player &player) {
  health -= damage;
  if (health <= 0) {
    die();
    player.score += score_reward;
    return;
  }
  if (hurt_animation.has_value()) {
    is_hurting = true;
    is_attacking = false;
    is_walking = false;
    hurt_animation.value().reset_animation();
  }
  if (hurt_sound_id.has_value()) {
    SoundManager::get_instance().play_sound(hurt_sound_id.value());
  }
}
void Enemy::die() {
  this->health = 0;
  if (is_dying || is_dead) {
    return;
  }
  this->is_dying = true;
  end_attack_anim();
  end_hurt_anim();
  SoundManager::get_instance().play_sound(death_sound_id);
  if (symbol == Tile::Symbol::ENEMY_MECHA_H) {
    game_manager.pickups.emplace_back(PICKUP_SETTINGS.at(Tile::Symbol::PICKUP_WINNING_KEY),
                                      tile_coords,
                                      grid.tile_size);
  }
}
void Enemy::end_attack_anim() {
  is_attacking = false;
}
void Enemy::end_death_anim() {
  is_dying = false;
  is_hurting = false;
  is_attacking = false;
  is_walking = false;
  is_dead = true;
  death_animation.force_texture_rect(death_animation.get_last_frame_texture_rect());
}
void Enemy::end_hurt_anim() {
  is_hurting = false;
}
void Enemy::end_walk_anim() {
  is_walking = false;
}
void Enemy::move(sf::Vector2f new_pos) {
  tile_coords = grid.pos_to_coords(new_pos);
  pos = new_pos;
}
