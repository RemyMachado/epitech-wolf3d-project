#ifndef EPITECH_WOLF3D_PROJECT_SRC_GAMEMANAGER_HPP_
#define EPITECH_WOLF3D_PROJECT_SRC_GAMEMANAGER_HPP_

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include "../Grid.hpp"
#include "../Player.hpp"
#include "../Camera.hpp"
#include "../Hud.hpp"
#include "../MouseAndKeyboard.hpp"
#include "../my_math.hpp"

class GameManager {
 public:
  sf::RenderWindow window;
  Grid grid;
  Player player;
  Camera camera;
  Hud hud;
  MouseAndKeyboard mouse_and_keyboard;
  std::vector<Enemy> enemies;
  int current_level = 1;
  int score = 72;
  unsigned int render_loop_count = 0;

 public:
  GameManager(char *filename, float tile_size, sf::Vector2i screen_size);

  bool check_is_game_over(sf::Sprite &hud_empty_sprite);
  void update();
};

#endif //EPITECH_WOLF3D_PROJECT_SRC_GAMEMANAGER_HPP_
