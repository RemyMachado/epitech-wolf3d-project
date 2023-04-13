#ifndef EPITECH_WOLF3D_PROJECT_SRC_SPRITESETTING_HPP_
#define EPITECH_WOLF3D_PROJECT_SRC_SPRITESETTING_HPP_

#include <SFML/System/Vector2.hpp>
#include <unordered_map>
#include <SFML/Graphics/Text.hpp>

enum class SpriteId {
  // Environment
  WALL,
  FLOOR,
  CEILING,

  // Hud
  HUD_EMPTY,

  // Weapons
  KNIFE_IDLE,
  PISTOL_IDLE,
  KNIFE_ATTACK,
  PISTOL_ATTACK,
  KNIFE_HUD,
  PISTOL_HUD,
  SHOTGUN,
  MACHINE_GUN,
  RIFLE,
  BULLET,
  ENEMY,
  HEALTH,
  AMMO,
  KEY,
  DOOR,
  EXIT,
  TEXT
};

struct SpriteSetting {
  std::string texture_path;
  sf::Vector2i frame_size;
  int frame_count;
  sf::Vector2i initial_offset;
  sf::Vector2i frame_offset;
};

static std::unordered_map<SpriteId, SpriteSetting> SPRITE_SETTINGS = {
	/*
	 * ENVIRONMENT
	 * */
	{SpriteId::WALL, SpriteSetting{
		"assets/textures/walls/redbrick.png",
		sf::Vector2i(64, 64),
		1,
		sf::Vector2i(0, 0),
		sf::Vector2i(0, 0)
	}},
	{SpriteId::FLOOR, SpriteSetting{
		"assets/textures/floor/colorstone.png",
		sf::Vector2i(64, 64),
		1,
		sf::Vector2i(0, 0),
		sf::Vector2i(0, 0)
	}},
	{SpriteId::CEILING, SpriteSetting{
		"assets/textures/ceiling/greystone.png",
		sf::Vector2i(64, 64),
		1,
		sf::Vector2i(0, 0),
		sf::Vector2i(0, 0)
	}},

	/*
	 * HUD
	 * */
	{SpriteId::HUD_EMPTY, SpriteSetting{
		"assets/textures/hud/empty-hud.png",
		sf::Vector2i(302, 33),
		1,
		sf::Vector2i(0, 0),
		sf::Vector2i(0, 0)
	}},

	/*
	 * WEAPONS IDLE
	 * */
	{SpriteId::KNIFE_IDLE, SpriteSetting{
		"assets/textures/weapons/weapons-sprite-sheet.png",
		sf::Vector2i(64, 64),
		1,
		sf::Vector2i(0, 0),
		sf::Vector2i(0, 0)
	}},
	{SpriteId::PISTOL_IDLE, SpriteSetting{
		"assets/textures/weapons/weapons-sprite-sheet.png",
		sf::Vector2i(64, 64),
		1,
		sf::Vector2i(0, 64 + 1),
		sf::Vector2i(0, 0)
	}},

	/*
	 * WEAPONS ATTACK
	 * */
	{SpriteId::KNIFE_ATTACK, SpriteSetting{
		"assets/textures/weapons/weapons-sprite-sheet.png",
		sf::Vector2i(64, 64),
		4,
		sf::Vector2i(64 + 1, 0),
		sf::Vector2i(64 + 1, 0)
	}},
	{SpriteId::PISTOL_ATTACK, SpriteSetting{
		"assets/textures/weapons/weapons-sprite-sheet.png",
		sf::Vector2i(64, 64),
		4,
		sf::Vector2i(64 + 1, 64 + 1),
		sf::Vector2i(64 + 1, 0)
	}},

	/*
	 * WEAPONS HUD
	 * */
	{SpriteId::KNIFE_HUD, SpriteSetting{
		"assets/textures/weapons/hud-weapon-sprite-sheet.png",
		sf::Vector2i(48, 24),
		1,
		sf::Vector2i(0, 0),
		sf::Vector2i(0, 0)
	}},
	{SpriteId::PISTOL_HUD, SpriteSetting{
		"assets/textures/weapons/hud-weapon-sprite-sheet.png",
		sf::Vector2i(48, 24),
		1,
		sf::Vector2i(48 + 1, 0),
		sf::Vector2i(1, 0)
	}},
};

#endif //EPITECH_WOLF3D_PROJECT_SRC_SPRITESETTING_HPP_
