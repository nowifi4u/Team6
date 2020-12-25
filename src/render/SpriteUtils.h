#pragma once

#include <SFML/Graphics.hpp>

namespace SpriteUtils {

	inline void setSize(sf::Sprite& sprite, const sf::Vector2f& size)
	{
		sprite.setScale(size.x / sprite.getTexture()->getSize().x, size.y / sprite.getTexture()->getSize().y);
	}

	inline void centerOrigin(sf::Sprite& sprite)
	{
		sprite.setOrigin(
			sprite.getTexture()->getSize().x / 2.f,
			sprite.getTexture()->getSize().y / 2.f
		);
	}

} // SpriteUtils
