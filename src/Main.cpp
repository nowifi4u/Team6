#define LOG_LEVEL_3

#include <iostream>

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include "game.h"

int main()
{
	boost::asio::io_service io;
	server_connector connector(io);

	connector.connect("wgforge-srv.wargaming.net", "443");

	Game game(connector);
	game.init({ "test3", "test3", "Game of Thrones", -1, 1 });

	global_Drawer.start(&game.gamedrawer, { 800, 800 }, "Kek");
	game.drawer_set_state(GameDrawer::Status::READY);

	global_Drawer.window_join();

	try
	{
		
	}
	catch (const std::runtime_error& err)
	{
		std::cout << "ERROR! " << err.what() << std::endl;
	}

	//connector.disconnect();

	return 0;
}