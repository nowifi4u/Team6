#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include <mutex>
#include <boost/thread.hpp>

#include <map>

#include "../game/data.h"
#include "../render/TextureManager.h"

constexpr float vertex_scale_koeff = 0.1;

struct game_drawer_config
{
	sf::Color clear_color = sf::Color::Black;
	sf::VideoMode window_videomode;
	std::string window_name;

	std::string edge_length_font = "res/arial.ttf";

	sf::Uint8 delta = 20;

	bool to_draw_edge_length = true;
};

namespace game_drawer_layer {



	class layer_base
	{
	public:

		layer_base(TextureManager& tm) : textureManager_(tm) {}

		TextureManager& textureManager_;
		virtual void init(const GameData& gamedata, const game_drawer_config& config) = 0;
		virtual void reset() = 0;
		virtual void draw(sf::RenderWindow& window, const GameData& gamedata) = 0;
	};



	class vertecies : public layer_base
	{
	public:

		std::map<GraphIdx::vertex_descriptor, sf::Sprite> nodes_g;

		vertecies(TextureManager& tm) : layer_base(tm) {}

		void init(const GameData& gamedata, const game_drawer_config& config)
		{
			LOG_3("game_drawer_layer::vertecies::init");

			gamedata.map_graph.for_each_vertex_descriptor([&](GraphIdx::vertex_descriptor v) {

				sf::Sprite& s = nodes_g[v];
				bool b = false;
				if (gamedata.map_graph.graph[v].post_idx != GraphIdx::uint32_max) {
					switch (getPostType(v, gamedata)) {
					case Posts::PostType::MARKET:
						b = textureManager_.RequireResource("market");
						s = sf::Sprite(*textureManager_.GetResource("market"));
						break;
					case  Posts::PostType::TOWN:
						b = textureManager_.RequireResource("castle");
						s = sf::Sprite(*textureManager_.GetResource("castle"));
						break;
					case  Posts::PostType::STORAGE:
						b = textureManager_.RequireResource("storage");
						s = sf::Sprite(*textureManager_.GetResource("storage"));
						break;
					default:
						break;
					}
				}
				else {
					b = textureManager_.RequireResource("cs");
					s = sf::Sprite(*textureManager_.GetResource("cs"));
				}


				s.setScale(vertex_scale_koeff, vertex_scale_koeff);
				s.setOrigin(
					s.getTexture()->getSize().x * s.getScale().x / 2.f,
					s.getTexture()->getSize().y * s.getScale().y / 2.f
				);

				const CoordsHolder::point_type& vcoords = gamedata.map_graph_coords->get_map()[v];
				s.setPosition(sf::Vector2f{
					   (float)vcoords[0] + config.delta,
					   (float)vcoords[1] + config.delta
					});
				}
			);
		}

		Posts::PostType getPostType(GraphIdx::vertex_descriptor v, const GameData& gamedata) {

			const GraphIdx::VertexProperties& vprops = gamedata.map_graph.graph[v];

			switch (gamedata.posts.at(vprops.post_idx)->type())
			{
			case Posts::TOWN: return Posts::TOWN;
			case Posts::STORAGE: return Posts::STORAGE;
			default: return Posts::MARKET;
			}
		}

		void reset()
		{
			LOG_3("game_drawer_layer::vertecies::reset");

			nodes_g.clear();
		}

		void draw(sf::RenderWindow& window, const GameData& gamedata)
		{
			for (const auto& vertex : nodes_g)
			{
				window.draw(vertex.second);
			}
		}
	};



	class edges : public layer_base
	{
		std::map<GraphIdx::edge_descriptor, sf::Sprite> edges_g;
		vertecies& vert;

	public:
		edges(TextureManager& tm, vertecies& vert_) : layer_base(tm), vert(vert_) {}


		void init(const GameData& gamedata, const game_drawer_config& config)
		{
			LOG_3("game_drawer_layer::edges::init");

			gamedata.map_graph.for_each_edge_descriptor([&](GraphIdx::edge_descriptor e) {

				textureManager_.RequireResource("railway");

				auto u = boost::source(e, gamedata.map_graph.graph);
				auto v = boost::target(e, gamedata.map_graph.graph);

				const auto& coords = gamedata.map_graph_coords->get_map();
				if (coords[u][0] >= coords[v][0])
					std::swap(u, v);

				sf::Texture* temp = textureManager_.GetResource("railway");
				auto temp_x = temp->getSize().x;
				auto temp_y = temp->getSize().y;

				auto vertex_size = vert.nodes_g[v].getTexture()->getSize().x * vertex_scale_koeff;
				auto edge_optimal_x_size = vertex_size / 2.0;
				auto edge_scale_koeff = edge_optimal_x_size / temp_x;

				double vertecies_distance = sqrt((coords[u][0] - coords[v][0]) * (coords[u][0] - coords[v][0]) +
					(coords[u][1] - coords[v][1]) * (coords[u][1] - coords[v][1])
				);
				sf::Texture* main_texture = textureManager_.GetResource("railway");

				main_texture->setRepeated(true);
				edges_g[e] = sf::Sprite(*main_texture,
					sf::IntRect(0, 0, temp_x, round(vertecies_distance / edge_scale_koeff)));
				edges_g[e].setOrigin(sf::Vector2f{ (float)edge_optimal_x_size / 2.f, 0.f });
				edges_g[e].setPosition(
					vert.nodes_g[v].getPosition().x,
					vert.nodes_g[v].getPosition().y + vertex_size / 2.f
				);
				edges_g[e].scale(edge_scale_koeff, edge_scale_koeff);

				double tan_alpha;
				if (coords[u][1] > coords[v][1]) {
					tan_alpha = (coords[v][0] - coords[u][0]) / (coords[u][1] - coords[v][1]);
					edges_g[e].setRotation((float)(atan(tan_alpha) * 180.0 / 3.14159265));
				}
				else if (coords[u][1] < coords[v][1]) {
					tan_alpha = (-coords[u][1] + coords[v][1]) / (coords[v][0] - coords[u][0]);
					edges_g[e].setRotation((float)(90 + atan(tan_alpha) * 180.0 / 3.14159265));
				}
				else {
					edges_g[e].setRotation(90);
				}
				});
		}

		void reset()
		{
			LOG_3("game_drawer_layer::edges::reset");
			edges_g.clear();
		}

		void draw(sf::RenderWindow& window, const GameData& gamedata)
		{
			for (const auto& edge : edges_g)
			{
				window.draw(edge.second);
			}
		}
	};


	//class trains : public layer_base
	//{
	//	std::map<Types::edge_idx_t, sf::Vertex[2]> cached_edges;

	//public:

	//	trains(TextureManager& tm): layer_base(tm) {}

	//	void init(const GameData& gamedata, const game_drawer_config& config)
	//	{
	//		LOG_3("game_drawer_layer::edges::init");

	//		gamedata.map_graph.for_each_edge_descriptor([&](GraphIdx::edge_descriptor e) {
	//			const CoordsHolder::point_type& es = gamedata.map_graph_coords->get_map()[boost::source(e, gamedata.map_graph.graph)];
	//			const CoordsHolder::point_type& et = gamedata.map_graph_coords->get_map()[boost::target(e, gamedata.map_graph.graph)];
	//			const GraphIdx::EdgeProperties& eprops = gamedata.map_graph.graph[e];

	//			sf::Vertex* line = cached_edges[eprops.idx];
	//			line[0] = sf::Vertex(sf::Vector2f(es[0], es[1]), config.edge_color);
	//			line[1] = sf::Vertex(sf::Vector2f(et[0], et[1]), config.edge_color);
	//			});
	//	}

	//	void reset()
	//	{
	//		LOG_3("game_drawer_layer::edges::reset");

	//		cached_edges.clear();
	//	}

	//	void draw(sf::RenderWindow& window, const GameData& gamedata)
	//	{
	//		for (const auto& edge : cached_edges)
	//		{
	//			window.draw(edge.second, 2, sf::Lines);
	//		}
	//	}
	//};



	//class edges_length : public layer_base
	//{
	//	sf::Font cached_font;
	//	std::map<Types::edge_idx_t, sf::Text> cached_edges_length;

	//public:

	//	edges_length(TextureManager& tm) : layer_base(tm) {}

	//	void init(const GameData& gamedata, const game_drawer_config& config)
	//	{
	//		LOG_3("game_drawer_layer::edges_length::init");

	//		cached_font.loadFromFile(config.edge_length_font);

	//		gamedata.map_graph.for_each_edge_descriptor([&](GraphIdx::edge_descriptor e) {
	//			const CoordsHolder::point_type& es = gamedata.map_graph_coords->get_map()[boost::source(e, gamedata.map_graph.graph)];
	//			const CoordsHolder::point_type& et = gamedata.map_graph_coords->get_map()[boost::target(e, gamedata.map_graph.graph)];
	//			const GraphIdx::EdgeProperties& eprops = gamedata.map_graph.graph[e];

	//			sf::Text& line_length = cached_edges_length[eprops.idx];
	//			line_length.setString(std::to_string(eprops.length));
	//			line_length.setPosition(sf::Vector2f(
	//				(es[0] + et[0]) / 2 + config.edge_length_offset_x,
	//				(es[1] + et[1]) / 2 + config.edge_length_offset_y
	//			));

	//			line_length.setFont(cached_font);
	//			line_length.setCharacterSize(config.edge_length_size);
	//			line_length.setFillColor(config.edge_length_color);
	//			});
	//	}

	//	void reset()
	//	{
	//		LOG_3("game_drawer_layer::edges_length::reset");

	//		cached_edges_length.clear();
	//	}

	//	void draw(sf::RenderWindow& window, const GameData& gamedata)
	//	{
	//		for (const auto& edge : cached_edges_length)
	//		{
	//			window.draw(edge.second);
	//		}
	//	}
	//};

	class background : public layer_base
	{
		sf::Sprite bg;

	public:

		background(TextureManager& tm) : layer_base(tm) {}

		void init(const GameData& gamedata, const game_drawer_config& config)
		{
			LOG_3("game_drawer_layer::background::init");

			textureManager_.RequireResource("bg");
			sf::Texture* bg_texture = textureManager_.GetResource("bg");
			bg.setTexture(*bg_texture);
		}

		void reset()
		{
			LOG_3("game_drawer_layer::background::reset");
			//nothing
		}

		void draw(sf::RenderWindow& window, const GameData& gamedata)
		{
			window.draw(bg);
		}
	};

} // namespace game_drawer_layer


enum status : uint8_t
{
	AWAIT_PLAYERS = 0,
	READY = 1,
	UPDATING = 2,
	CALCULATING = 3,
};

class game_drawer
{
protected:

	/*
		WARNING!!!
			DO NOT CHANGE FIELDS ORDER!!!
		!!!
	*/

	const game_drawer_config& config;

	sf::Clock clock_;
	sf::Time elapsed_;

	TextureManager textureManager_;

	game_drawer_layer::vertecies vert; // for edges init

	boost::ptr_vector<game_drawer_layer::layer_base> layers;
	
public:

	game_drawer(const GameData& gamedata, const game_drawer_config& config = {})
		: config(config), clock_(), textureManager_(), vert(textureManager_)
	{
		layers.push_back(new game_drawer_layer::background(textureManager_));
		layers.push_back(&vert);
		layers.push_back(new game_drawer_layer::edges(textureManager_, vert));
		//layers.push_back(new game_drawer_layer::edges_length(textureManager_));
		
	}

	void init(const GameData& gamedata, sf::RenderWindow& window)
	{
		LOG_2("game_drawer::init");

		for (game_drawer_layer::layer_base& layer : layers)
		{
			layer.init(gamedata, config);
		}
	}

	void restart_clock() {
		elapsed_ += clock_.restart();
	}

	void handle_input(sf::RenderWindow& window, sf::Event event, const GameData& gamedata, const status& s) {

		if (event.type == sf::Event::Closed)
		{
			window.close();
			exit(0);
		}
	}

	void reset()
	{
		LOG_2("game_drawer::reset");

		for (game_drawer_layer::layer_base& layer : layers)
		{
			layer.reset();
		}
	}

	

	void draw(sf::RenderWindow& window, const GameData& gamedata)
	{
		for (game_drawer_layer::layer_base& layer : layers)
		{
			layer.draw(window, gamedata);
		}
	}

	void render(sf::RenderWindow& window, const GameData& gamedata, const status& s)
	{
		switch (s)
		{
			case status::AWAIT_PLAYERS:  window.clear(sf::Color::Blue);  break;
			case status::UPDATING:  window.clear(sf::Color::Red);  break;
			case status::CALCULATING:  window.clear(sf::Color::Yellow);   break;
			case status::READY:  window.clear(config.clear_color); break;
		}

		if(s == status::READY)
			draw(window, gamedata);

		window.display();
	}

	void start(sf::RenderWindow& window, const GameData& gamedata, const status& s)
	{
		LOG_2("game_drawer: start");

		init(gamedata, window);

		while (window.isOpen())
		{
			try {
				sf::Event event;
				while (window.pollEvent(event))
				{
					this->handle_input(window, event, gamedata, s);
					this->update(window, gamedata, s);
					this->render(window, gamedata, s);
					this->restart_clock();
				}
			}
			catch (boost::thread_interrupted&)
			{
				LOG_2("game_render: Render thread interrupted");
			}
		}
	}

	void update(sf::RenderWindow& window, const GameData& gamedata,const status& s) {
		//update infos
	}
};

void game_drawer_thread(const GameData& gamedata, const game_drawer_config& config, const status& s, sf::RenderWindow*& callback)
{
	LOG_2("game_drawer_thread: Creating RenderWindow...");
	sf::RenderWindow window(config.window_videomode, config.window_name);
	LOG_2("game_drawer_thread: Saving RenderWindow pointer...");
	callback = &window;
	

	LOG_2("game_drawer_thread: Creating game_drawer...");
	game_drawer drawer(gamedata, config);

	LOG_2("game_drawer_thread: Starting draw loop...");
	drawer.start(window, gamedata, s);
}