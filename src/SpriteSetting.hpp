#ifndef EPITECH_WOLF3D_PROJECT_SRC_SPRITESETTING_HPP_
#define EPITECH_WOLF3D_PROJECT_SRC_SPRITESETTING_HPP_

#include <SFML/System/Vector2.hpp>
#include <unordered_map>
#include <SFML/Graphics/Text.hpp>

enum class SpriteId {
  // Fonts
  NUMBER_0 = 0,
  NUMBER_1 = 1,
  NUMBER_2 = 2,
  NUMBER_3 = 3,
  NUMBER_4 = 4,
  NUMBER_5 = 5,
  NUMBER_6 = 6,
  NUMBER_7 = 7,
  NUMBER_8 = 8,
  NUMBER_9 = 9,

  // Environment
  WALL,
  WALL_SHADOW,
  FLOOR,
  CEILING,

  // Hud
  HUD_EMPTY,
  HUD_FACES,

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
  TEXT,

};

struct SpriteSetting {
  SpriteId id;
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
		SpriteId::WALL,
		"assets/textures/walls/redbrick.png",
		sf::Vector2i(64, 64),
		1,
		sf::Vector2i(0, 0),
		sf::Vector2i(0, 0)
	}},
	{SpriteId::WALL_SHADOW, SpriteSetting{
		SpriteId::WALL_SHADOW,
		"assets/textures/walls/redbrick-dark.png",
		sf::Vector2i(64, 64),
		1,
		sf::Vector2i(0, 0),
		sf::Vector2i(0, 0)
	}},
	{SpriteId::FLOOR, SpriteSetting{
		SpriteId::FLOOR,
		"assets/textures/floor/colorstone.png",
		sf::Vector2i(64, 64),
		1,
		sf::Vector2i(0, 0),
		sf::Vector2i(0, 0)
	}},
	{SpriteId::CEILING, SpriteSetting{
		SpriteId::CEILING,
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
		SpriteId::HUD_EMPTY,
		"assets/textures/hud/empty-hud.png",
		sf::Vector2i(302, 33),
		1,
		sf::Vector2i(0, 0),
		sf::Vector2i(0, 0)
	}},
	{SpriteId::HUD_FACES, SpriteSetting{
		SpriteId::HUD_FACES,
		"assets/textures/hud/hud-faces-sprite-sheet.png",
		sf::Vector2i(24, 31),
		8,
		sf::Vector2i(0, 1),
		sf::Vector2i(0, 31 + 2)
	}},

	/*
	 * WEAPONS IDLE
	 * */
	{SpriteId::KNIFE_IDLE, SpriteSetting{
		SpriteId::KNIFE_IDLE,
		"assets/textures/weapons/weapons-sprite-sheet.png",
		sf::Vector2i(64, 64),
		1,
		sf::Vector2i(0, 0),
		sf::Vector2i(0, 0)
	}},
	{SpriteId::PISTOL_IDLE, SpriteSetting{
		SpriteId::PISTOL_IDLE,
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
		SpriteId::KNIFE_ATTACK,
		"assets/textures/weapons/weapons-sprite-sheet.png",
		sf::Vector2i(64, 64),
		4,
		sf::Vector2i(64 + 1, 0),
		sf::Vector2i(64 + 1, 0)
	}},
	{SpriteId::PISTOL_ATTACK, SpriteSetting{
		SpriteId::PISTOL_ATTACK,
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
		SpriteId::KNIFE_HUD,
		"assets/textures/weapons/hud-weapon-sprite-sheet.png",
		sf::Vector2i(48, 24),
		1,
		sf::Vector2i(0, 0),
		sf::Vector2i(0, 0)
	}},
	{SpriteId::PISTOL_HUD, SpriteSetting{
		SpriteId::PISTOL_HUD,
		"assets/textures/weapons/hud-weapon-sprite-sheet.png",
		sf::Vector2i(48, 24),
		1,
		sf::Vector2i(48 + 1, 0),
		sf::Vector2i(1, 0)
	}},

	/*
	 * Fonts
	 * */
	{SpriteId::NUMBER_0, SpriteSetting{
		SpriteId::NUMBER_0,
		"assets/textures/fonts/numbers-sprite-sheet.png",
		sf::Vector2i(8, 16),
		1,
		sf::Vector2i(0, 0),
		sf::Vector2i(0, 0)
	}},
	{SpriteId::NUMBER_1, SpriteSetting{
		SpriteId::NUMBER_1,
		"assets/textures/fonts/numbers-sprite-sheet.png",
		sf::Vector2i(8, 16),
		1,
		sf::Vector2i(8 + 1, 0),
		sf::Vector2i(0, 0)
	}},
	{SpriteId::NUMBER_2, SpriteSetting{
		SpriteId::NUMBER_2,
		"assets/textures/fonts/numbers-sprite-sheet.png",
		sf::Vector2i(8, 16),
		1,
		sf::Vector2i(16 + 2, 0),
		sf::Vector2i(0, 0)
	}},
	{SpriteId::NUMBER_3, SpriteSetting{
		SpriteId::NUMBER_3,
		"assets/textures/fonts/numbers-sprite-sheet.png",
		sf::Vector2i(8, 16),
		1,
		sf::Vector2i(24 + 3, 0),
		sf::Vector2i(0, 0)
	}},
	{SpriteId::NUMBER_4, SpriteSetting{
		SpriteId::NUMBER_4,
		"assets/textures/fonts/numbers-sprite-sheet.png",
		sf::Vector2i(8, 16),
		1,
		sf::Vector2i(32 + 4, 0),
		sf::Vector2i(0, 0)
	}},
	{SpriteId::NUMBER_5, SpriteSetting{
		SpriteId::NUMBER_5,
		"assets/textures/fonts/numbers-sprite-sheet.png",
		sf::Vector2i(8, 16),
		1,
		sf::Vector2i(40 + 5, 0),
		sf::Vector2i(0, 0)
	}},
	{SpriteId::NUMBER_6, SpriteSetting{
		SpriteId::NUMBER_6,
		"assets/textures/fonts/numbers-sprite-sheet.png",
		sf::Vector2i(8, 16),
		1,
		sf::Vector2i(48 + 6, 0),
		sf::Vector2i(0, 0)
	}},
	{SpriteId::NUMBER_7, SpriteSetting{
		SpriteId::NUMBER_7,
		"assets/textures/fonts/numbers-sprite-sheet.png",
		sf::Vector2i(8, 16),
		1,
		sf::Vector2i(56 + 7, 0),
		sf::Vector2i(0, 0)
	}},
	{SpriteId::NUMBER_8, SpriteSetting{
		SpriteId::NUMBER_8,
		"assets/textures/fonts/numbers-sprite-sheet.png",
		sf::Vector2i(8, 16),
		1,
		sf::Vector2i(64 + 8, 0),
		sf::Vector2i(0, 0)
	}},
	{SpriteId::NUMBER_9, SpriteSetting{
		SpriteId::NUMBER_9,
		"assets/textures/fonts/numbers-sprite-sheet.png",
		sf::Vector2i(8, 16),
		1,
		sf::Vector2i(72 + 9, 0),
		sf::Vector2i(0, 0)
	}},
};

#endif //EPITECH_WOLF3D_PROJECT_SRC_SPRITESETTING_HPP_
