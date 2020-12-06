#include "GameData.h"

#include "packet-parser.h"
#include "bincharstream.h"
#include "binstream.h"

#include <iostream>
#include <iomanip>
#include <algorithm>

#include "game_connector.h"

namespace
{
	bool keepGoing = true;
	void shutdown(int) { keepGoing = false; }
}

int main()
{
	boost::asio::io_service io;
	
	game_connector connector(io);
	connector.connect("wgforge-srv.wargaming.net", "443");
	
	connector.socket.set_option(tcp::no_delay(true));
	connector.socket.set_option(boost::asio::socket_base::receive_buffer_size(65536));
	connector.socket.set_option(boost::asio::socket_base::send_buffer_size(65536));

	std::wcout << "Sending Login request..." << std::endl;
	connector.send(Packets::encode::Games::encodeJSON());
	std::cout << "OK" << std::endl;

	std::cout << "Waiting for answer..." << std::endl;
	connector.wait_read();
	std::cout << "OK" << std::endl;


	auto response = connector.read_packet();
	std::cout << "------------------------- BEGIN -------------------------" << std::endl;
	std::cout << "Action: " << response.first << std::endl;
	std::cout << response.second << std::endl;
	std::cout << "-------------------------  END  -------------------------" << std::endl;

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