#define LOG_LEVEL_3

#include <sdkddkver.h>

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

		game.init("wgforge-srv.wargaming.net", "443", { "test3", "test3", "Game of Thrones", -1, 3 });

		game.drawer_start();
		game.drawer_window_wait();
		game.drawer_window->setTitle("Ready");


		Sleep(3000);
		game.drawer_set_state(game_drawer::UPDATING);
		game.drawer_window->setTitle("Updating...");

		Sleep(1000);
		game.drawer_set_state(game_drawer::CALCULATING);
		game.drawer_window->setTitle("Calculating...");

		Sleep(1000);
		game.drawer_set_state(game_drawer::READY);
		game.drawer_window->setTitle("Ready");


		Sleep(1000);
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