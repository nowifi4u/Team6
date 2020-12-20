#define LOG_LEVEL_3

#include <iostream>

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include "GameData.h"

#include "game_connector.h"
#include "game_drawer.h"

int main()
{
	boost::asio::io_service io;
	game_connector connector(io);
	GameData gamedata;

	try
	{

		connector.connect("wgforge-srv.wargaming.net", "443");

		//connector.socket.set_option(tcp::no_delay(true));
		//connector.socket.set_option(boost::asio::socket_base::receive_buffer_size(65536));
		//connector.socket.set_option(boost::asio::socket_base::send_buffer_size(65536));

		{
			LOG_1("Sending Games request...");
			connector.send_Games();

			const auto response = connector.read_packet();
		}

		{
			LOG_1("Sending Login request...");
			connector.send_Login({ "test3", "test3", "Game of Thrones", -1, 3 });

			const auto response = connector.read_packet();

			GameData::readJSON_Login(gamedata, json::parse(response.second));
		}

		{
			LOG_1("Sending L0 request...");
			connector.send_Map({ 0 });

			const auto response = connector.read_packet();

			GameData::readJSON_L0(gamedata, json::parse(response.second));
		}

		{
			LOG_1("Sending L10 request...");
			connector.send_Map({ 10 });

			const auto response = connector.read_packet();

			GameData::readJSON_L10(gamedata, json::parse(response.second));
		}

		{
			LOG_1("Sending L1 request...");
			connector.send_Map({ 1 });

			const auto response = connector.read_packet();

			GameData::readJSON_L1(gamedata, json::parse(response.second));
		}

		{
			LOG_1("Sending L1 request...");
			connector.send_Map({ 1 });

			const auto response = connector.read_packet();

			GameData::readJSON_L1(gamedata, json::parse(response.second));
		}

		{
			LOG_1("Sending Games request...");
			connector.send_Games();

			const auto response = connector.read_packet();
		}

		connector.disconnect();


		game_drawer_config drawer_config;
		game_drawer::status drawer_status = game_drawer::READY;

		LOG_2("Creating game_drawer thread...");
		boost::thread* drawer_thread = new boost::thread(&game_drawer_thread, boost::ref(gamedata), boost::ref(drawer_config), boost::ref(drawer_status));


		Sleep(3000);
		drawer_status = game_drawer::UPDATING;
		drawer_thread->interrupt();

		Sleep(1000);
		drawer_status = game_drawer::CALCULATING;
		drawer_thread->interrupt();

		Sleep(1000);
		drawer_status = game_drawer::READY;
		drawer_thread->interrupt();


		Sleep(1000);
		//drawer_thread->join();
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