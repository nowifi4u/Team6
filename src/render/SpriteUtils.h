#pragma once

#include <SFML/Graphics.hpp>

namespace VectorUtils {

	inline sf::Vector2f makeVector2f(const sf::Vector2u& vec)
	{
		return sf::Vector2f(vec.x, vec.y);
	}

	inline sf::Vector2u makeVector2u(const sf::Vector2f& vec)
	{
		return sf::Vector2u(vec.x, vec.y);
	}

} // namespace VectorUtils

namespace TextureUtils {

	inline sf::Vector2u getSize(const sf::Texture& texture)
	{
		return sf::Vector2u(texture.getSize().x, texture.getSize().y);
	}

} // TextureUtils

namespace SpriteUtils {

	inline sf::Vector2u getSize(const sf::Sprite& sprite)
	{
		return TextureUtils::getSize(*sprite.getTexture());
	}

	inline void setSize(sf::Sprite& sprite, const sf::Vector2f& size)
	{
		const auto act_size = getSize(sprite);

		sprite.setScale(size.x / act_size.x, size.y / act_size.y);
	}

	inline void setSize(sf::Sprite& sprite, const sf::Vector2u& size)
	{
		const auto act_size = getSize(sprite);

		sprite.setScale(size.x / act_size.x, size.y / act_size.y);
	}

	inline void centerOrigin(sf::Transformable& obj, const sf::Vector2u& size)
	{
		obj.setOrigin(
			size.x / 2.f,
			size.y / 2.f
		);
	}

	inline void centerOrigin(sf::Transformable& obj, const sf::Vector2f& size)
	{
		obj.setOrigin(
			size.x / 2.f,
			size.y / 2.f
		);
	}

	inline void centerOrigin(sf::Sprite& sprite)
	{
		const auto act_size = getSize(sprite);

		centerOrigin(sprite, act_size);
	}

} // SpriteUtils
