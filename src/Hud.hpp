#ifndef EPITECH_WOLF3D_PROJECT_SRC_HUD_HPP_
#define EPITECH_WOLF3D_PROJECT_SRC_HUD_HPP_

#include <SFML/Graphics/Rect.hpp>

/*
 * Hud original:
 * - bar_width: 302px
 * - bar_height: 33px
 * */

struct HudRelativeCenterRatioSettings {
  sf::Vector2f level;
  sf::Vector2f score;
  sf::Vector2f lives;
  sf::Vector2f face;
  sf::Vector2f health;
  sf::Vector2f ammo;
  sf::Vector2f weapon;
};

/*
 * The center points percentages of each area of the HUD relative to the original texture size.
 * */
static HudRelativeCenterRatioSettings HUD_RELATIVE_CENTER_RATIO_SETTINGS = {
	.level = {16 / (float)302, 21 / (float)33},
	.score = {62 / (float)302, 21 / (float)33},
	.lives = {107.5f / (float)302, 21 / (float)33},
	.face = {140 / (float)302, 16.5f / (float)33},
	.health = {170.5f / (float)302, 21 / (float)33},
	.ammo = {213 / (float)302, 21 / (float)33},
	.weapon = {270.5f / (float)302, 16.5f / (float)33},
};

class Hud {
 public:
  int bar_width; // equal to screen bar_width
  int bar_height; // computed by the HUD texture ratio with/height
  float scale_factor; // computed by the HUD texture ratio new/original
  float minimap_to_height_ratio = 0.2f;

  explicit Hud()
	  : bar_width(0), bar_height(0) {}
};

#endif //EPITECH_WOLF3D_PROJECT_SRC_HUD_HPP_
