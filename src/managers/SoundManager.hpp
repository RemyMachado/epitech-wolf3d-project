#ifndef EPITECH_WOLF3D_PROJECT_SRC_MANAGERS_SOUNDMANAGER_HPP_
#define EPITECH_WOLF3D_PROJECT_SRC_MANAGERS_SOUNDMANAGER_HPP_

#include <unordered_map>
#include <SFML/Audio.hpp>
#include <iostream>

enum class SoundId {
  KNIFE_ATTACK,
  PISTOL_ATTACK,
  ENEMY_DOG_BARK,
  PLAYER_DIES,
  PLAYER_SMALL_PAIN,
  PLAYER_BIG_PAIN,
  MUSIC_BACKGROUND,
  SWITCH_WEAPON,
  AMMO,
};

struct SoundSetting {
  std::string sound_path;
};

static std::unordered_map<SoundId, SoundSetting> SOUND_SETTINGS = {
	/*
	 * Weapons
	 * */
	{SoundId::KNIFE_ATTACK, SoundSetting{
		"assets/sounds/weapons/knife_attack.wav",
	}},
	{SoundId::PISTOL_ATTACK, SoundSetting{
		"assets/sounds/weapons/pistol_attack.wav",
	}},
	{SoundId::ENEMY_DOG_BARK, SoundSetting{
		"assets/sounds/enemies/dog_bark.wav",
	}},
	{SoundId::PLAYER_DIES, SoundSetting{
		"assets/sounds/player/player_dies.wav",
	}},
	{SoundId::PLAYER_SMALL_PAIN, SoundSetting{
		"assets/sounds/player/player_small_pain.wav",
	}},
	{SoundId::PLAYER_BIG_PAIN, SoundSetting{
		"assets/sounds/player/player_big_pain.wav",
	}},
	{SoundId::MUSIC_BACKGROUND, SoundSetting{
		"assets/sounds/background/2_wondering_about_my_loved_ones.ogg",
	}},
	{SoundId::SWITCH_WEAPON, SoundSetting{
		"assets/sounds/weapons/switch_weapon.wav",
	}},
	{SoundId::AMMO, SoundSetting{
		"assets/sounds/weapons/ammo.wav",
	}},
};

class SoundManager {
 private:
  std::unordered_map<SoundId, std::unique_ptr<sf::SoundBuffer>> sound_buffers;
  std::unordered_map<SoundId, std::unique_ptr<sf::Sound>> sounds;

 public:
  SoundManager(const SoundManager &) = delete;
  SoundManager &operator=(const SoundManager &) = delete;

  static SoundManager &get_instance() {
	static SoundManager instance;
	return instance;
  }

  void play_sound(SoundId sound_id, bool loop = false) {
	auto found = sounds.find(sound_id);

	if (found != sounds.end()) {
	  found->second->play();
	  return;
	}

	auto sound_buffer = std::make_unique<sf::SoundBuffer>();
	auto filePath = SOUND_SETTINGS.at(sound_id).sound_path;

	if (!sound_buffer->loadFromFile(filePath)) {
	  throw std::runtime_error("Failed to load sound from file: \"" + filePath + "\"");
	}

	auto sound = std::make_unique<sf::Sound>(*sound_buffer);
	if (loop) {
	  sound->setLoop(true);
	}
	sound->play();
	sound_buffers.insert({sound_id, std::move(sound_buffer)});
	sounds.insert({sound_id, std::move(sound)});
  }

  void play_music_background() {
	play_sound(SoundId::MUSIC_BACKGROUND, true);
  }

 private:
  SoundManager() = default;
};

#endif //EPITECH_WOLF3D_PROJECT_SRC_MANAGERS_SOUNDMANAGER_HPP_
