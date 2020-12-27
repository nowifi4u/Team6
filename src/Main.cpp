#define LOG_LEVEL_3

#include <sdkddkver.h>

#include <iostream>

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include <src/game.h>

int main()
{
	boost::asio::io_service io;
	server_connector connector(io);

	try
	{

		Game game(io);

		game.connect("wgforge-srv.wargaming.net", "443");
		game.init({ "test3", "test3", "Game of Thrones", -1, 1 });
		game.drawer_start();

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