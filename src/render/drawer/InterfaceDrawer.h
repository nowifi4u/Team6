#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include "../../utils/Logging.h"

class InterfaceDrawer
{
public:

	enum class Type
	{
		GAME
	};

	virtual Type getType() const = 0;

	virtual void draw(sf::RenderWindow& window) = 0;
};