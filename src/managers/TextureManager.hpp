#ifndef EPITECH_WOLF3D_PROJECT_SRC_TEXTUREMANAGER_HPP_
#define EPITECH_WOLF3D_PROJECT_SRC_TEXTUREMANAGER_HPP_

#include <SFML/Graphics/Texture.hpp>
#include <iostream>
#include "../SpriteSetting.hpp"

class TextureManager {
 private:
  std::unordered_map<SpriteId, sf::Texture> textures;

 public:
  TextureManager(const TextureManager &) = delete;
  TextureManager &operator=(const TextureManager &) = delete;

  static TextureManager &get_instance() {
	static TextureManager instance;
	return instance;
  }

  const sf::Texture &get_texture(SpriteId sprite_id) {
	auto found = textures.find(sprite_id);

	if (found != textures.end()) {
	  return found->second;
	}

	sf::Texture texture;
	auto filePath = SPRITE_SETTINGS.at(sprite_id).texture_path;

	if (!texture.loadFromFile(filePath)) {
	  throw std::runtime_error("Failed to load texture from file: \"" + filePath + "\"");
	}

	textures.insert({sprite_id, texture});
	return textures.at(sprite_id);
  }

 private:
  TextureManager() = default;
};

#endif //EPITECH_WOLF3D_PROJECT_SRC_TEXTUREMANAGER_HPP_
