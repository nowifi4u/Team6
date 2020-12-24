#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include "../../utils/Logging.h"

class InterfaceDrawer
{
public:

	virtual void draw(sf::RenderWindow& window) = 0;
};