#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include <mutex>
#include <boost/thread.hpp>

#include <map>

#include "game_data.h"


struct game_drawer_config
{
	sf::Color clear_color = sf::Color::Black;

	float vertex_radius = 5;
	sf::Color vertex_color_empty = sf::Color::White;
	sf::Color vertex_color_storage = sf::Color::Yellow;
	sf::Color vertex_color_market = sf::Color::Red;
	sf::Color vertex_color_town = sf::Color::Green;

	sf::Color edge_color = sf::Color::White;
	sf::Color edge_length_color = sf::Color::Magenta;
	std::string edge_length_font = "../res/arial.ttf";
	float edge_length_size = 15;
	float edge_length_offset_x = -7;
	float edge_length_offset_y = -7;
};

namespace game_drawer_layer {



	class _interface
	{
	public:

		virtual void init(const GameData& gamedata, const game_drawer_config& config) = 0;
		virtual void reset() = 0;

		virtual void draw(sf::RenderWindow& window, const GameData& gamedata) = 0;
	};



	class vertecies : public _interface
	{
		std::map<Types::vertex_idx_t, sf::CircleShape> cached_vertecies;

	public:

		void init(const GameData& gamedata, const game_drawer_config& config)
		{
			LOG_2("game_drawer: init_vertecies");

			gamedata.map_graph.for_each_vertex_props([&](const GraphIdx::VertexProperties& v) {
				sf::CircleShape& dot = cached_vertecies[v.idx];
				dot.setRadius(config.vertex_radius);

				if (v.post_idx == UINT32_MAX)
				{
					dot.setFillColor(config.vertex_color_empty);
				}
				else
				{
					switch (gamedata.posts.at(v.post_idx)->type())
					{
					case Posts::TOWN: dot.setFillColor(config.vertex_color_town); break;
					case Posts::STORAGE: dot.setFillColor(config.vertex_color_storage); break;
					case Posts::MARKET: dot.setFillColor(config.vertex_color_market); break;
					}
				}

				dot.setPosition(v.pos_x - config.vertex_radius, v.pos_y - config.vertex_radius);
				});
		}

		void reset()
		{
			cached_vertecies.clear();
		}

		void draw(sf::RenderWindow& window, const GameData& gamedata)
		{
			for (const auto& vertex : cached_vertecies)
			{
				window.draw(vertex.second);
			}
		}
	};



	class edges : public _interface
	{
		std::map<Types::edge_idx_t, sf::Vertex[2]> cached_edges;

	public:

		void init(const GameData& gamedata, const game_drawer_config& config)
		{
			LOG_2("game_drawer: init_edges");

			gamedata.map_graph.for_each_edge_descriptor([&](GraphIdx::edge_descriptor e) {
				const GraphIdx::VertexProperties& es = gamedata.map_graph.graph[boost::source(e, gamedata.map_graph.graph)];
				const GraphIdx::VertexProperties& et = gamedata.map_graph.graph[boost::target(e, gamedata.map_graph.graph)];
				const GraphIdx::EdgeProperties& eprops = gamedata.map_graph.graph[e];

				sf::Vertex* line = cached_edges[eprops.idx];
				line[0] = sf::Vertex(sf::Vector2f(es.pos_x, es.pos_y), config.edge_color);
				line[1] = sf::Vertex(sf::Vector2f(et.pos_x, et.pos_y), config.edge_color);
				});
		}

		void reset()
		{
			cached_edges.clear();
		}

		void draw(sf::RenderWindow& window, const GameData& gamedata)
		{
			for (const auto& edge : cached_edges)
			{
				window.draw(edge.second, 2, sf::Lines);
			}
		}
	};



	class edges_length : public _interface
	{
		sf::Font cached_font;
		std::map<Types::edge_idx_t, sf::Text> cached_edges_length;

	public:

		void init(const GameData& gamedata, const game_drawer_config& config)
		{
			cached_font.loadFromFile(config.edge_length_font);

			gamedata.map_graph.for_each_edge_descriptor([&](GraphIdx::edge_descriptor e) {
				const GraphIdx::VertexProperties& es = gamedata.map_graph.graph[boost::source(e, gamedata.map_graph.graph)];
				const GraphIdx::VertexProperties& et = gamedata.map_graph.graph[boost::target(e, gamedata.map_graph.graph)];
				const GraphIdx::EdgeProperties& eprops = gamedata.map_graph.graph[e];

				sf::Text& line_length = cached_edges_length[eprops.idx];
				const std::string text = std::to_string(eprops.length);
				line_length.setString(text.c_str());
				line_length.setPosition(sf::Vector2f(
					(es.pos_x + et.pos_x) / 2 + config.edge_length_offset_x,
					(es.pos_y + et.pos_y) / 2 + config.edge_length_offset_y
				));

				line_length.setFont(cached_font);
				line_length.setCharacterSize(config.edge_length_size);
				line_length.setFillColor(config.edge_length_color);
				});
		}

		void reset()
		{
			cached_edges_length.clear();
		}

		void draw(sf::RenderWindow& window, const GameData& gamedata)
		{
			for (const auto& edge : cached_edges_length)
			{
				window.draw(edge.second);
			}
		}
	};

} // namespace game_drawer_layer


class game_drawer
{
protected:

	sf::RenderWindow& window;
	const GameData& gamedata;
	const game_drawer_config& config;

	boost::ptr_vector<game_drawer_layer::_interface> layers;
	
public:

	void update_config()
	{
		for (game_drawer_layer::_interface& layer : layers)
		{
			layer.init(gamedata, config);
		}
	}

	game_drawer(sf::RenderWindow& window, const GameData& gamedata, const game_drawer_config& config = {})
		: window(window), gamedata(gamedata), config(config)
	{
		layers.reserve(3);
		layers.push_back(new game_drawer_layer::edges);
		layers.push_back(new game_drawer_layer::vertecies);
		layers.push_back(new game_drawer_layer::edges_length);

		update_config();
	}

	enum status : uint8_t
	{
		READY = 0,
		UPDATING = 1,
		CALCULATING = 2,
	};

	void _draw()
	{
		for (game_drawer_layer::_interface& layer : layers)
		{
			layer.draw(window, gamedata);
		}
	}

	void draw(status s)
	{
		switch (s)
		{
			case status::UPDATING:  window.clear(sf::Color::Red);  break;
			case status::CALCULATING:  window.clear(sf::Color::Yellow);   break;
			case status::READY:
			{
				window.clear(config.clear_color);

				_draw();

			} break;
		}
	}

	void start(const status& s)
	{
		while (window.isOpen())
		{
			try {

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

				this->draw(s);
				window.display();

			}
			catch (boost::thread_interrupted&)
			{
				LOG_2("game_render: Render thread interrupted");
			}
		}
	}
};

void game_drawer_thread(const GameData& gamedata, const game_drawer_config& config, const game_drawer::status& s)
{
	LOG_2("Creating RenderWindow...");
	sf::RenderWindow window(sf::VideoMode(gamedata.map_graph.graph_props().size_width + 20, gamedata.map_graph.graph_props().size_height + 20), "graph");

	LOG_2("Creating game_drawer...");
	game_drawer drawer(window, gamedata, config);

	drawer.start(s);
}