#ifndef EPITECH_WOLF3D_PROJECT_SRC_CAMERA_HPP_
#define EPITECH_WOLF3D_PROJECT_SRC_CAMERA_HPP_

class Camera {
 public:
  float height;
  float render_distance; // equals the diagonal of the grid
  float fov_horizontal_deg;
  float fov_vertical_deg;

  Camera(float height, float render_distance, float fov_horizontal_deg = 90, float fov_vertical_deg = 60)
      : height(height),
        render_distance(render_distance),
        fov_horizontal_deg(fov_horizontal_deg),
        fov_vertical_deg(fov_vertical_deg) {};
};

#endif //EPITECH_WOLF3D_PROJECT_SRC_CAMERA_HPP_
