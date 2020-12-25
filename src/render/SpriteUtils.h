#pragma once

#include <SFML/Graphics.hpp>

inline void setSize(sf::Sprite& sprite, const sf::Vector2f& size)
{
	sprite.setScale(size.x / sprite.getTexture()->getSize().x, size.y / sprite.getTexture()->getSize().y);
}
