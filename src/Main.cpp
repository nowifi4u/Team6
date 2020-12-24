#define LOG_LEVEL_1

#include <sdkddkver.h>

#include <iostream>
#include <fstream>

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include "game_drawer.h"

int main()
{

	try
	{
		GameData gamedata;

		{
			std::cout << "Enter file name: ";
			std::string filename;
			std::getline(std::cin, filename);

			std::ifstream fin(filename);

			GameData::readJSON_L0(gamedata, json::parse(fin));

			std::cout << "Unit length: ";
			double unit_length;
			std::cin >> unit_length;

			GameData::calculateCoordinates(gamedata, 800, 800, unit_length);
		}

		
		
		game_drawer_config drawer_config;
		drawer_config.window_videomode = sf::VideoMode(800, 800);
		sf::RenderWindow* drawer_window;
		game_drawer::status drawer_status = game_drawer::READY;

		game_drawer_thread(gamedata, drawer_config, drawer_status, drawer_window);
	}
	catch (const nlohmann::detail::type_error& err)
	{
		std::cout << "ERROR! " << err.what() << std::endl;
	}
	catch (const std::runtime_error& err)
	{
		std::cout << "ERROR! " << err.what() << std::endl;
	}

	return 0;
}