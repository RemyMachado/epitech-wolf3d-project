#ifndef EPITECH_WOLF3D_PROJECT_SRC_HUD_HPP_
#define EPITECH_WOLF3D_PROJECT_SRC_HUD_HPP_

class Hud {
 public:
  int height;
  float minimap_player_radius;

  explicit Hud(int height = 200, float minimap_player_radius = 5)
      : height(height), minimap_player_radius(minimap_player_radius) {}
};

#endif //EPITECH_WOLF3D_PROJECT_SRC_HUD_HPP_
