#define LOG_LEVEL_2

#include "GameData.h"

#include "packet-parser.h"
#include "bincharstream.h"
#include "binstream.h"

#include <iostream>
#include <iomanip>
#include <algorithm>

#include "game_connector.h"

int main()
{
	boost::asio::io_service io;
	game_connector connector(io);
	GameData gamedata;

	try
	{

		connector.connect("wgforge-srv.wargaming.net", "443");

		connector.socket.set_option(tcp::no_delay(true));
		connector.socket.set_option(boost::asio::socket_base::receive_buffer_size(65536));
		connector.socket.set_option(boost::asio::socket_base::send_buffer_size(65536));

		{
			LOG_2("Sending Games request...");
			connector.send(Packets::encode::Games::encodeJSON());

			connector.wait_read();

			const auto response = connector.read_packet();
		}

		{
			LOG_2("Sending Login request...");
			connector.send(Packets::encode::Login::encodeJSON({ "test2", "test2", "Game of Thrones", -1, 10 }));

			connector.wait_read();


			const auto response = connector.read_packet();

			GameData::readJSON_Login(gamedata, json::parse(response.second));
		}

		{
			LOG_2("Sending L0 request...");
			connector.send(Packets::encode::Map::encodeJSON({ 0 }));

			connector.wait_read();

			const auto response = connector.read_packet();

			GameData::readJSON_L0(gamedata, json::parse(response.second));
		}

		{
			LOG_2("Sending L10 request...");
			connector.send(Packets::encode::Map::encodeJSON({ 10 }));

			connector.wait_read();

			const auto response = connector.read_packet();

			GameData::readJSON_L10(gamedata, json::parse(response.second));
		}

		{
			LOG_2("Sending L1 request...");
			connector.send(Packets::encode::Map::encodeJSON({ 1 }));

			connector.wait_read();

			const auto response = connector.read_packet();

			GameData::readJSON_L1(gamedata, json::parse(response.second));
		}

		{
			LOG_2("Sending Games request...");
			connector.send(Packets::encode::Games::encodeJSON());

			connector.wait_read();

			const auto response = connector.read_packet();

			Sleep(1000);
		}

	}
	catch (const nlohmann::detail::type_error& err)
	{
		std::cout << "ERROR! " << err.what() << std::endl;
	}
	catch (const std::runtime_error& err)
	{
		std::cout << "ERROR! " << err.what() << std::endl;
	}

	connector.close();

	return 0;

	/*GameData gamedata;

	std::cout << "Importing GameData..." << std::endl;

	ptree_from_file("small_graph.json", [&](const ptree& pt) {
		GameData::readJSON_L0(gamedata, pt);
		});
	
	std::cout << "Calculating coordinates..." << std::endl;

	std::cout << "Drawing Graph with coordinates..." << std::endl;

	gamedata.graph.for_each_vertex([&](auto v) {
		const auto& vertex = gamedata.graph.graph[v];
		std::cout << v
			<< " idx:" << vertex.idx
			<< " length:" << (int32_t)vertex.post_idx
			<< " pos:[" << vertex.pos_x << ' ' << vertex.pos_y << ']'
			<< std::endl;
		});*/

	return 0;
}