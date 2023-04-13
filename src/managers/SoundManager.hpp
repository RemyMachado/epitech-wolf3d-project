#ifndef EPITECH_WOLF3D_PROJECT_SRC_MANAGERS_SOUNDMANAGER_HPP_
#define EPITECH_WOLF3D_PROJECT_SRC_MANAGERS_SOUNDMANAGER_HPP_

#include <unordered_map>
#include <SFML/Audio.hpp>
#include <iostream>

enum class SoundId {
  KNIFE_ATTACK,
  PISTOL_ATTACK,
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

  void play_sound(SoundId sound_id) {
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
	sound->play();
	sound_buffers.insert({sound_id, std::move(sound_buffer)});
	sounds.insert({sound_id, std::move(sound)});
  }

 private:
  SoundManager() = default;
};

#endif //EPITECH_WOLF3D_PROJECT_SRC_MANAGERS_SOUNDMANAGER_HPP_
