#ifndef EPITECH_WOLF3D_PROJECT_SRC_HUD_HPP_
#define EPITECH_WOLF3D_PROJECT_SRC_HUD_HPP_

class Hud {
 public:
  int bar_width; // equal to screen bar_width
  int bar_height; // computed by the HUD texture ratio bar_width/bar_height
  float minimap_to_height_ratio = 0.2f;

  explicit Hud()
	  : bar_width(0), bar_height(0) {}
};

#endif //EPITECH_WOLF3D_PROJECT_SRC_HUD_HPP_
