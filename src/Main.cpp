#define LOG_LEVEL_1

#include <iostream>

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include "game.h"

int main()
{
	boost::asio::io_service io;
	game_connector connector(io);

	try
	{

		Game game(io);

		game.start("wgforge-srv.wargaming.net", "443", { "test3", "test3", "Game of Thrones", -1, 1 });

		game.drawer_join();
	}
	catch (const nlohmann::detail::type_error& err)
	{
		std::cout << "ERROR! " << err.what() << std::endl;
	}
	catch (const std::runtime_error& err)
	{
		std::cout << "ERROR! " << err.what() << std::endl;
	}

	//connector.disconnect();

	return 0;
}