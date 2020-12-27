#include <iostream>

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>


#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE
#include <spdlog/spdlog.h>

#include <src/game.h>


int main()
{
    spdlog::set_pattern("[%H:%M:%S:%e] [%^%l%$] [thread %t] [%@::%!] %v");
    spdlog::set_level(spdlog::level::debug);

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
	    SPDLOG_ERROR(err.what());
	}
	catch (const std::runtime_error& err)
	{
        SPDLOG_ERROR(err.what());
	}

	//connector.disconnect();

	return 0;
}
