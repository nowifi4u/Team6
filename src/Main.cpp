#define LOG_LEVEL_3

#include "GameData.h"

#include "packet-parser.h"
#include "bincharstream.h"
#include "binstream.h"

#include <iostream>

#include "game_connector.h"

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

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
			connector.send(Packets::encode::Games::encodeJSON());

			const auto response = connector.read_packet();
		}

		{
			LOG_1("Sending Login request...");
			connector.send(Packets::encode::Login::encodeJSON({ "test3", "test3", "Game of Thrones", -1, 3 }));

			const auto response = connector.read_packet();

			GameData::readJSON_Login(gamedata, json::parse(response.second));
		}

		{
			LOG_1("Sending L0 request...");
			connector.send(Packets::encode::Map::encodeJSON({ 0 }));

			const auto response = connector.read_packet();

			GameData::readJSON_L0(gamedata, json::parse(response.second));
		}

		{
			LOG_1("Sending L10 request...");
			connector.send(Packets::encode::Map::encodeJSON({ 10 }));

			const auto response = connector.read_packet();

			GameData::readJSON_L10(gamedata, json::parse(response.second));
		}

		{
			LOG_1("Sending L1 request...");
			connector.send(Packets::encode::Map::encodeJSON({ 1 }));

			const auto response = connector.read_packet();

			GameData::readJSON_L1(gamedata, json::parse(response.second));
		}

		{
			LOG_1("Sending L1 request...");
			connector.send(Packets::encode::Map::encodeJSON({ 1 }));

			const auto response = connector.read_packet();

			GameData::readJSON_L1(gamedata, json::parse(response.second));
		}

		{
			LOG_1("Sending Games request...");
			connector.send(Packets::encode::Games::encodeJSON());

			const auto response = connector.read_packet();
		}

		sf::RenderWindow window(sf::VideoMode(gamedata.map_graph.graph_props().size_width + 20, gamedata.map_graph.graph_props().size_height + 20), "graph");
		sf::Font font_arial;
		font_arial.loadFromFile("../res/arial.ttf");

		while (window.isOpen())
		{
			sf::Event event;
			while (window.pollEvent(event))
			{
				// Request for closing the window
				if (event.type == sf::Event::Closed)
				{
					window.close();
					exit(0);
				}

			}

			window.clear();

			gamedata.map_graph.for_each_edge_descriptor([&](GraphIdx::edge_descriptor e) {
				const GraphIdx::VertexProperties& es = gamedata.map_graph.graph[boost::source(e, gamedata.map_graph.graph)];
				const GraphIdx::VertexProperties& et = gamedata.map_graph.graph[boost::target(e, gamedata.map_graph.graph)];

				sf::Vertex line[2]{
					sf::Vertex(sf::Vector2f(es.pos_x, es.pos_y), sf::Color(255, 255, 255, 100)),
					sf::Vertex(sf::Vector2f(et.pos_x, et.pos_y), sf::Color(255, 255, 255, 100))
				};
				window.draw(line, 2, sf::Lines);

				sf::Text line_length(std::to_string(gamedata.map_graph.graph[e].length), font_arial, 15);
				line_length.setPosition(sf::Vector2f((es.pos_x + et.pos_x) / 2 - 7, (es.pos_y + et.pos_y) / 2 - 7));
				line_length.setFillColor(sf::Color::Magenta);

				window.draw(line_length);

				});

			gamedata.map_graph.for_each_vertex_props([&](const GraphIdx::VertexProperties& v) {
				sf::CircleShape dot(5);
				if (v.post_idx == UINT32_MAX)
				{
					dot.setFillColor(sf::Color(255, 255, 255, 100));
				}
				else
				{
					switch (gamedata.posts[v.post_idx]->type())
					{
					case Posts::TOWN: dot.setFillColor(sf::Color(255, 0, 0, 100)); break;
					case Posts::STORAGE: dot.setFillColor(sf::Color(0, 255, 255, 100)); break;
					case Posts::MARKET: dot.setFillColor(sf::Color(0, 255, 0, 100)); break;
					}
				}

				dot.setPosition(v.pos_x - 5, v.pos_y - 5);
				window.draw(dot);
				});

			window.display();
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