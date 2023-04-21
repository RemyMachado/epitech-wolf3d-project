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
  DOOR,
  DOOR_SHADOW,
  FLOOR,
  CEILING,

  // Hud
  HUD_EMPTY,
  HUD_FACES,
  WINNING_SCREEN,
  SPLASH_SCREEN,

  // Weapons
  KNIFE_IDLE,
  KNIFE_ATTACK,
  KNIFE_HUD,
  PISTOL_IDLE,
  PISTOL_ATTACK,
  PISTOL_HUD,
  THOMPSON_IDLE,
  THOMPSON_ATTACK,
  THOMPSON_HUD,
  MACHINE_GUN_IDLE,
  MACHINE_GUN_ATTACK,
  MACHINE_GUN_HUD,

  // Enemies
  ENEMY_GUARD_IDLE,
  ENEMY_DOG_IDLE,
  ENEMY_MECHA_H_IDLE,
  ENEMY_GUARD_ATTACK,
  ENEMY_DOG_ATTACK,
  ENEMY_MECHA_H_ATTACK,
  ENEMY_GUARD_DEATH,
  ENEMY_DOG_DEATH,
  ENEMY_MECHA_H_DEATH,
  ENEMY_GUARD_WALK,
  ENEMY_DOG_WALK,
  ENEMY_MECHA_H_WALK,
  ENEMY_GUARD_HURT,

  // Texts
  TEXT_GAME_OVER,

  // Pickups
  PICKUP_HEALTH,
  PICKUP_AMMO,
  PICKUP_THOMPSON,
  PICKUP_MACHINE_GUN,
  PICKUP_WINNING_KEY,
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
    {SpriteId::DOOR, SpriteSetting{
        SpriteId::DOOR,
        "assets/textures/walls/wood.png",
        sf::Vector2i(64, 64),
        1,
        sf::Vector2i(0, 0),
        sf::Vector2i(0, 0)
    }},
    {SpriteId::DOOR_SHADOW, SpriteSetting{
        SpriteId::DOOR_SHADOW,
        "assets/textures/walls/wood-dark.png",
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
    {SpriteId::WINNING_SCREEN, SpriteSetting{
        SpriteId::WINNING_SCREEN,
        "assets/textures/hud/winning-screen.png",
        sf::Vector2i(118, 78),
        1,
        sf::Vector2i(0, 0),
        sf::Vector2i(0, 0)
    }},
    {SpriteId::SPLASH_SCREEN, SpriteSetting{
        SpriteId::SPLASH_SCREEN,
        "assets/textures/hud/splash-screen.png",
        sf::Vector2i(319, 200),
        1,
        sf::Vector2i(0, 0),
        sf::Vector2i(0, 0)
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
    {SpriteId::THOMPSON_IDLE, SpriteSetting{
        SpriteId::THOMPSON_IDLE,
        "assets/textures/weapons/weapons-sprite-sheet.png",
        sf::Vector2i(64, 64),
        1,
        sf::Vector2i(0, (64 + 1) * 2),
        sf::Vector2i(0, 0)
    }},
    {SpriteId::MACHINE_GUN_IDLE, SpriteSetting{
        SpriteId::MACHINE_GUN_IDLE,
        "assets/textures/weapons/weapons-sprite-sheet.png",
        sf::Vector2i(64, 64),
        1,
        sf::Vector2i(0, (64 + 1) * 3),
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
    {SpriteId::THOMPSON_ATTACK, SpriteSetting{
        SpriteId::THOMPSON_ATTACK,
        "assets/textures/weapons/weapons-sprite-sheet.png",
        sf::Vector2i(64, 64),
        4,
        sf::Vector2i(64 + 1, (64 + 1) * 2),
        sf::Vector2i(64 + 1, 0)
    }},
    {SpriteId::MACHINE_GUN_ATTACK, SpriteSetting{
        SpriteId::MACHINE_GUN_ATTACK,
        "assets/textures/weapons/weapons-sprite-sheet.png",
        sf::Vector2i(64, 64),
        4,
        sf::Vector2i(64 + 1, (64 + 1) * 3),
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
        sf::Vector2i(0, 0)
    }},
    {SpriteId::THOMPSON_HUD, SpriteSetting{
        SpriteId::THOMPSON_HUD,
        "assets/textures/weapons/hud-weapon-sprite-sheet.png",
        sf::Vector2i(48, 24),
        1,
        sf::Vector2i((48 + 1) * 2, 0),
        sf::Vector2i(0, 0)
    }},
    {SpriteId::MACHINE_GUN_HUD, SpriteSetting{
        SpriteId::MACHINE_GUN_HUD,
        "assets/textures/weapons/hud-weapon-sprite-sheet.png",
        sf::Vector2i(48, 24),
        1,
        sf::Vector2i((48 + 1) * 3, 0),
        sf::Vector2i(0, 0)
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

    /*
     * ENEMIES
     * */
    {SpriteId::ENEMY_GUARD_IDLE, SpriteSetting{
        SpriteId::ENEMY_GUARD_IDLE,
        "assets/textures/enemies/guard-sprite-sheet.png",
        sf::Vector2i(64, 64),
        1,
        sf::Vector2i(0, 0),
        sf::Vector2i(0, 0)
    }},
    {SpriteId::ENEMY_MECHA_H_IDLE, SpriteSetting{
        SpriteId::ENEMY_MECHA_H_IDLE,
        "assets/textures/enemies/mecha-h-sprite-sheet.png",
        sf::Vector2i(64, 64),
        1,
        sf::Vector2i(64, 0),
        sf::Vector2i(0, 0)
    }},
    {SpriteId::ENEMY_DOG_IDLE, SpriteSetting{
        SpriteId::ENEMY_DOG_IDLE,
        "assets/textures/enemies/dog-sprite-sheet.png",
        sf::Vector2i(64, 64),
        1,
        sf::Vector2i(0, 0),
        sf::Vector2i(0, 0)
    }},
    {SpriteId::ENEMY_GUARD_WALK, SpriteSetting{
        SpriteId::ENEMY_GUARD_WALK,
        "assets/textures/enemies/guard-sprite-sheet.png",
        sf::Vector2i(64, 64),
        4,
        sf::Vector2i(0, 64 + 1),
        sf::Vector2i(0, 64 + 1)
    }},
    {SpriteId::ENEMY_DOG_WALK, SpriteSetting{
        SpriteId::ENEMY_DOG_WALK,
        "assets/textures/enemies/dog-sprite-sheet.png",
        sf::Vector2i(64, 64),
        4,
        sf::Vector2i(0, 0),
        sf::Vector2i(0, 64 + 1)
    }},
    {SpriteId::ENEMY_MECHA_H_WALK, SpriteSetting{
        SpriteId::ENEMY_MECHA_H_WALK,
        "assets/textures/enemies/mecha-h-sprite-sheet.png",
        sf::Vector2i(64, 64),
        4,
        sf::Vector2i(0, 0),
        sf::Vector2i(64, 0)
    }},
    {SpriteId::ENEMY_GUARD_ATTACK, SpriteSetting{
        SpriteId::ENEMY_GUARD_ATTACK,
        "assets/textures/enemies/guard-sprite-sheet.png",
        sf::Vector2i(64, 64),
        3,
        sf::Vector2i(0, (64 + 1) * 6),
        sf::Vector2i(64 + 1, 0)
    }},
    {SpriteId::ENEMY_DOG_ATTACK, SpriteSetting{
        SpriteId::ENEMY_DOG_ATTACK,
        "assets/textures/enemies/dog-sprite-sheet.png",
        sf::Vector2i(64, 64),
        3,
        sf::Vector2i(0, (64 + 1) * 5),
        sf::Vector2i(64 + 1, 0)
    }},
    {SpriteId::ENEMY_MECHA_H_ATTACK, SpriteSetting{
        SpriteId::ENEMY_MECHA_H_ATTACK,
        "assets/textures/enemies/mecha-h-sprite-sheet.png",
        sf::Vector2i(64, 64),
        3,
        sf::Vector2i(0, 64),
        sf::Vector2i(64, 0)
    }},
    {SpriteId::ENEMY_GUARD_HURT, SpriteSetting{
        SpriteId::ENEMY_GUARD_HURT,
        "assets/textures/enemies/guard-sprite-sheet.png",
        sf::Vector2i(64, 64),
        1,
        sf::Vector2i((64 + 1) * 7, (64 + 1) * 5),
        sf::Vector2i(0, 0)
    }},
    {SpriteId::ENEMY_GUARD_DEATH, SpriteSetting{
        SpriteId::ENEMY_GUARD_DEATH,
        "assets/textures/enemies/guard-sprite-sheet.png",
        sf::Vector2i(64, 64),
        5,
        sf::Vector2i(0, (64 + 1) * 5),
        sf::Vector2i((64 + 1), 0)
    }},
    {SpriteId::ENEMY_DOG_DEATH, SpriteSetting{
        SpriteId::ENEMY_DOG_DEATH,
        "assets/textures/enemies/dog-sprite-sheet.png",
        sf::Vector2i(64, 64),
        4,
        sf::Vector2i(0, (64 + 1) * 4),
        sf::Vector2i(64 + 1, 0)
    }},
    {SpriteId::ENEMY_MECHA_H_DEATH, SpriteSetting{
        SpriteId::ENEMY_MECHA_H_DEATH,
        "assets/textures/enemies/mecha-h-sprite-sheet.png",
        sf::Vector2i(64, 64),
        11,
        sf::Vector2i(0, 64 * 2),
        sf::Vector2i(64, 0)
    }},

    /*
     * TEXTS
     * */
    {SpriteId::TEXT_GAME_OVER, SpriteSetting{
        SpriteId::TEXT_GAME_OVER,
        "assets/textures/texts/game-over.png",
        sf::Vector2i(202, 38),
        1,
        sf::Vector2i(0, 0),
        sf::Vector2i(0, 0)
    }},

    /*
     * PICKUPS
     * */
    {SpriteId::PICKUP_HEALTH, SpriteSetting{
        SpriteId::PICKUP_HEALTH,
        "assets/textures/props/props.png",
        sf::Vector2i(64, 64),
        1,
        sf::Vector2i((64 + 1) * 2, (64 + 1) * 5),
        sf::Vector2i(0, 0)
    }},
    {SpriteId::PICKUP_AMMO, SpriteSetting{
        SpriteId::PICKUP_AMMO,
        "assets/textures/props/props.png",
        sf::Vector2i(64, 64),
        1,
        sf::Vector2i((64 + 1) * 3, (64 + 1) * 5),
        sf::Vector2i(0, 0)
    }},
    {SpriteId::PICKUP_THOMPSON, SpriteSetting{
        SpriteId::PICKUP_THOMPSON,
        "assets/textures/props/props.png",
        sf::Vector2i(64, 64),
        1,
        sf::Vector2i((64 + 1) * 4, (64 + 1) * 5),
        sf::Vector2i(0, 0)
    }},
    {SpriteId::PICKUP_MACHINE_GUN, SpriteSetting{
        SpriteId::PICKUP_MACHINE_GUN,
        "assets/textures/props/props.png",
        sf::Vector2i(64, 64),
        1,
        sf::Vector2i(0, (64 + 1) * 6),
        sf::Vector2i(0, 0)
    }},
    {SpriteId::PICKUP_WINNING_KEY, SpriteSetting{
        SpriteId::PICKUP_WINNING_KEY,
        "assets/textures/props/props.png",
        sf::Vector2i(64, 64),
        1,
        sf::Vector2i((64 + 1) * 2, (64 + 1) * 4),
        sf::Vector2i(0, 0)
    }},
};

#endif //EPITECH_WOLF3D_PROJECT_SRC_SPRITESETTING_HPP_
