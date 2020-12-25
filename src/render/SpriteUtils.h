#pragma once

#include <SFML/Graphics.hpp>

namespace SpriteUtils {

	inline sf::Vector2f getSize(const sf::Sprite& sprite)
	{
		return sf::Vector2f(sprite.getTexture()->getSize().x, sprite.getTexture()->getSize().y);
	}

	inline void setSize(sf::Sprite& sprite, const sf::Vector2f& size)
	{
		const auto act_size = getSize(sprite);

		sprite.setScale(size.x / act_size.x, size.y / act_size.y);
	}

	inline void centerOrigin(sf::Sprite& sprite)
	{
		const auto act_size = getSize(sprite);

		sprite.setOrigin(
			act_size.x / 2.f,
			act_size.y / 2.f
		);
	}

} // SpriteUtils
