#pragma once
#include "../render/ResourceManager.h"
#include "SFML/Graphics.hpp"

class TextureManager :
	public ResourceManager<TextureManager, sf::Texture>
{
public:
	TextureManager(const std::string& path) : ResourceManager(path) {}

	sf::Texture* Load(const std::string& l_path) {
		sf::Texture* texture = new sf::Texture();
		if (!texture->loadFromFile(
			/*Utils::GetWorkingDirectory() +*/ l_path))
		{
			delete texture;
			texture = nullptr;
			std::cerr << "! Failed to load texture: "
				<< l_path << std::endl;
		}
		return texture;
	}
};
