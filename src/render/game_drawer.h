#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include <mutex>
#include <boost/thread.hpp>

#include <map>
#include <functional>

#include <src/game/data.h>
#include <src/render/TextureManager.h>
#include <src/render/SpriteUtils.h>

#include <src/utils/value_map.h>
#include <src/utils/Math.h>
#include <src/utils/MinMax.h>


struct game_drawer_config
{
	sf::Color clear_color = sf::Color::Black;
	sf::VideoMode window_videomode;
	std::string window_name;

	std::string edge_length_font;

	ValueMap<float> padding_width = ValueMap<float>(0, 1, 0, 1);
	ValueMap<float> padding_height = ValueMap<float>(0, 1, 0, 1);

	float frame_time = 1.0f / 10.0f;

	TextureManager* textures = nullptr;
};



namespace game_drawer_layer {


	class layer_base
	{
	public:

		layer_base() {}

		virtual void init(const GameData& gamedata, const game_drawer_config& config) = 0;
		virtual void reset() = 0;
		virtual void draw(sf::RenderWindow& window, const GameData& gamedata, const game_drawer_config& config) = 0;
	};


	class vertecies : public layer_base
	{
	public:

		std::map<Graph::vertex_descriptor, sf::Sprite> nodes_g;

		vertecies() : layer_base() {}

		void init(const GameData& gamedata, const game_drawer_config& config)
		{
			LOG_3("game_drawer_layer::vertecies::init");

			Graph::for_each_vertex_descriptor(gamedata.graph(), [&](Graph::vertex_descriptor v) {

				sf::Sprite& s = nodes_g[v];
				bool b = false;

				
				if (gamedata.map_graph.graph[v].post_idx != UINT32_MAX) {
					switch (getPostType(v, gamedata)) {
					case Posts::PostType::MARKET:
						b = config.textures->RequireResource("market");
						s = sf::Sprite(*config.textures->GetResource("market"));
						break;
					case  Posts::PostType::TOWN:
						b = config.textures->RequireResource("castle");
						s = sf::Sprite(*config.textures->GetResource("castle"));
						break;
					case  Posts::PostType::STORAGE:
						b = config.textures->RequireResource("storage");
						s = sf::Sprite(*config.textures->GetResource("storage"));
						break;
					default:
						break;
					}

					SpriteUtils::setSize(s, sf::Vector2f{ 40, 40 });
				}
				else {
					b = config.textures->RequireResource("cs");
					s = sf::Sprite(*config.textures->GetResource("cs"));

					SpriteUtils::setSize(s, sf::Vector2f{ 25, 25 });
				}

				SpriteUtils::centerOrigin(s);

				const CoordsHolder::point_type& vcoords = gamedata.map_graph_coords->get_map()[v];
				s.setPosition(sf::Vector2f{
						config.padding_width.map(vcoords[0]),
						config.padding_height.map(vcoords[1])
					});
				}
			);
		}

		Posts::PostType getPostType(Graph::vertex_descriptor v, const GameData& gamedata) {

			const Graph::VertexProperties& vprops = gamedata.map_graph.graph[v];

			switch (gamedata.posts.at(vprops.post_idx)->type())
			{
			case Posts::TOWN: return Posts::TOWN;
			case Posts::STORAGE: return Posts::STORAGE;
			case Posts::MARKET: return Posts::MARKET;
			}
		}

		void reset()
		{
			LOG_3("game_drawer_layer::vertecies::reset");

			nodes_g.clear();
		}

		void draw(sf::RenderWindow& window, const GameData& gamedata, const game_drawer_config& config)
		{
			for (const auto& vertex : nodes_g)
			{
				window.draw(vertex.second);
			}
		}
	};



	class edges : public layer_base
	{
		std::map<Graph::edge_descriptor, sf::Sprite> edges_g;

	public:
		edges() : layer_base() {}


		void init(const GameData& gamedata, const game_drawer_config& config)
		{
			LOG_3("game_drawer_layer::edges::init");

			Graph::for_each_edge_descriptor(gamedata.graph(), [&](Graph::edge_descriptor e) {

				sf::Sprite& edge = edges_g[e];

				config.textures->RequireResource("railway");

				auto u = boost::source(e, gamedata.map_graph.graph);
				auto v = boost::target(e, gamedata.map_graph.graph);

				const auto& coords = gamedata.map_graph_coords->get_map();
				if (coords[u][0] >= coords[v][0])
					std::swap(u, v);

				sf::Texture* main_texture = config.textures->GetResource("railway");
				auto main_size = TextureUtils::getSize(*main_texture);

				double vertecies_distance = sqrt(
					Math::sqr(config.padding_width.map(coords[u][0]) - config.padding_width.map(coords[v][0])) +
					Math::sqr(config.padding_height.map(coords[u][1]) - config.padding_height.map(coords[v][1]))
				);

				const float edge_length_coeff = 0.1;

				main_texture->setRepeated(true);
				edge = sf::Sprite(*main_texture,
					sf::IntRect(0, 0, main_size.x, round(vertecies_distance / edge_length_coeff)));
				edge.setOrigin(sf::Vector2f{ main_size.x / 2.f, 0.f });
				edge.setPosition(
					config.padding_width.map(coords[v][0]),
					config.padding_height.map(coords[v][1])
				);
				edge.setScale(0.1, edge_length_coeff);
				

				double tan_alpha;
				if (coords[u][1] > coords[v][1]) {
					tan_alpha = (config.padding_width.map(coords[v][0]) - config.padding_width.map(coords[u][0])) /
						(config.padding_height.map(coords[u][1]) - config.padding_height.map(coords[v][1]));
					edge.setRotation((float)(atan(tan_alpha) * 180.0 / 3.14159265));
				}
				else if (coords[u][1] < coords[v][1]) {
					tan_alpha = (config.padding_height.map(coords[v][1]) - config.padding_height.map(coords[u][1])) /
						(config.padding_width.map(coords[v][0]) - config.padding_width.map(coords[u][0]));
					edge.setRotation((float)(90 + atan(tan_alpha) * 180.0 / 3.14159265));
				}
				else {
					edge.setRotation(90);
				}
				});
		}

		void reset()
		{
			LOG_3("game_drawer_layer::edges::reset");
			edges_g.clear();
		}

		void draw(sf::RenderWindow& window, const GameData& gamedata, const game_drawer_config& config)
		{
			for (const auto& edge : edges_g)
			{
				window.draw(edge.second);
			}
		}
	};


	class trains : public layer_base
	{
		std::map<Types::train_idx_t, sf::Sprite> trains_g;
		std::map<Types::train_idx_t, const Trains::Train*> tMap;

	public:

		trains() : layer_base() {}

		void init(const GameData& gamedata, const game_drawer_config& config)
		{
			LOG_3("game_drawer_layer::trains::init");

			for (auto& player : gamedata.players)
			{
				for (auto& train : player.second.trains) {
					sf::Sprite& s = trains_g[train.first];
					tMap[train.first] = &train.second;

					bool b = config.textures->RequireResource("train");
					s = sf::Sprite(*config.textures->GetResource("train"));
					SpriteUtils::setSize(s, sf::Vector2f{ 25, 25 });
					SpriteUtils::centerOrigin(s);
				}
			}
		}

		void reset()
		{
			LOG_3("game_drawer_layer::edges::reset");

			trains_g.clear();
		}

		void draw(sf::RenderWindow& window, const GameData& gamedata, const game_drawer_config& config)
		{
			for (auto& s : trains_g)
			{
				const Trains::Train* t = tMap[s.first];
				const auto& edge = gamedata.map_graph.emap.at(t->line_idx);
				auto u = boost::source(edge, gamedata.map_graph.graph);
				auto v = boost::target(edge, gamedata.map_graph.graph);

				const auto& coords = gamedata.map_graph_coords->get_map();

				double v_x_distance = config.padding_width.map(coords[u][0]) - config.padding_width.map(coords[v][0]);
				double v_y_distance = config.padding_height.map(coords[u][1]) - config.padding_height.map(coords[v][1]);

				double edge_length = gamedata.map_graph.graph[edge].length;
				float position = (float)t->position;
				float koeff = position / edge_length;

				s.second.setPosition(
					sf::Vector2f(
					config.padding_width.map(coords[u][0]) + (float)v_x_distance * koeff,
					config.padding_height.map(coords[u][1]) + (float)v_y_distance * koeff
					));
				window.draw(s.second);
			}
		}
	};


	class edges_length : public layer_base
	{
		sf::Font cached_font;
		std::map<Types::edge_idx_t, sf::Text> cached_edges_length;

	public:

		edges_length() : layer_base() {}

		void init(const GameData& gamedata, const game_drawer_config& config)
		{
			LOG_3("game_drawer_layer::edges_length::init");

			cached_font.loadFromFile(config.edge_length_font);

			Graph::for_each_edge_descriptor(gamedata.graph(), [&](Graph::edge_descriptor e) {
				const CoordsHolder::point_type& es = gamedata.map_graph_coords->get_map()[boost::source(e, gamedata.map_graph.graph)];
				const CoordsHolder::point_type& et = gamedata.map_graph_coords->get_map()[boost::target(e, gamedata.map_graph.graph)];
				const Graph::EdgeProperties& eprops = gamedata.map_graph.graph[e];

				sf::Text& line_length = cached_edges_length[eprops.idx];
				line_length.setString(std::to_string(eprops.length));
				line_length.setPosition(sf::Vector2f(
					(config.padding_width.map(es[0]) + config.padding_width.map(et[0])) / 2,
					(config.padding_height.map(es[1]) + config.padding_height.map(et[1])) / 2
				));

				SpriteUtils::centerOrigin(line_length, sf::Vector2f(12, line_length.getCharacterSize() / 2.0f));
				line_length.setFont(cached_font);
				line_length.setCharacterSize(24);
				line_length.setFillColor(sf::Color::Red);
				});
		}

		void reset()
		{
			LOG_3("game_drawer_layer::edges_length::reset");

			cached_edges_length.clear();
		}

		void draw(sf::RenderWindow& window, const GameData& gamedata, const game_drawer_config& config)
		{
			for (const auto& edge : cached_edges_length)
			{
				window.draw(edge.second);
			}
		}
	};

	class background : public layer_base
	{
		sf::Sprite bg;

	public:

		background() : layer_base() {}

		void init(const GameData& gamedata, const game_drawer_config& config)
		{
			LOG_3("game_drawer_layer::background::init");

			config.textures->RequireResource("bg");
			sf::Texture* bg_texture = config.textures->GetResource("bg");
			bg.setTexture(*bg_texture);
			SpriteUtils::setSize(bg, sf::Vector2f( config.window_videomode.width, config.window_videomode.height ));
		}

		void reset()
		{
			LOG_3("game_drawer_layer::background::reset");
			//nothing
		}

		void draw(sf::RenderWindow& window, const GameData& gamedata, const game_drawer_config& config)
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
	const game_drawer_config& config;

	sf::Clock clock_;
	sf::Time elapsed_;

	boost::ptr_vector<game_drawer_layer::layer_base> layers;
	
public:

	game_drawer(const GameData& gamedata, const game_drawer_config& config)
		: config(config), clock_()
	{
		layers.push_back(new game_drawer_layer::background());
		layers.push_back(new game_drawer_layer::edges());
		layers.push_back(new game_drawer_layer::vertecies());
		layers.push_back(new game_drawer_layer::edges_length());
		layers.push_back(new game_drawer_layer::trains());
	}

	void init(const GameData& gamedata)
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

	void handle_input(sf::RenderWindow& window, const GameData& gamedata, const status& s) {

	
	}

	void reset()
	{
		LOG_2("game_drawer::reset");

		for (game_drawer_layer::layer_base& layer : layers)
		{
			layer.reset();
		}
	}

	

	void draw(sf::RenderWindow& window, const GameData& gamedata, const game_drawer_config& config)
	{
		for (game_drawer_layer::layer_base& layer : layers)
		{
			layer.draw(window, gamedata, config);
		}
	}

	void render(sf::RenderWindow& window, const GameData& gamedata, const game_drawer_config& config, const status& s)
	{
		switch (s)
		{
			case status::AWAIT_PLAYERS:  window.clear(sf::Color::Blue);  break;
			case status::UPDATING:  window.clear(sf::Color::Red);  break;
			case status::CALCULATING:  window.clear(sf::Color::Yellow);   break;
			case status::READY:  window.clear(config.clear_color); break;
		}

		if(s == status::READY)
			draw(window, gamedata, config);

		window.display();
	}

	void start(sf::RenderWindow& window, const GameData& gamedata, game_drawer_config& config, const status& s)
	{
		LOG_2("game_drawer: start");

		init(gamedata);

		while (window.isOpen())
		{
			try {	
				this->handle_input(window, gamedata, s);
				this->update(window, gamedata, config, s);
				this->render(window, gamedata, config, s);
				this->restart_clock();
			}
			catch (boost::thread_interrupted&)
			{
				LOG_2("game_render: Render thread interrupted");
			}
		}
	}

	void update(sf::RenderWindow& window, const GameData& gamedata, game_drawer_config& config, const status& s) {

		
		if (elapsed_.asSeconds() >= config.frame_time)
		{
			sf::Event event;
			while (window.pollEvent(event))
			{
				switch(event.type)
				{
				case sf::Event::Closed:
				{
					window.close();
					exit(0);
				}
				case sf::Event::Resized:
					const sf::Vector2u size = window.getSize();
					config.padding_width.set_output(size.x / 10, size.x * 9 / 10);
					config.padding_height.set_output(size.y / 10, size.y * 9 / 10);

					config.window_videomode.width = size.x;
					config.window_videomode.height = size.y;
					window.setView(sf::View(sf::Vector2f(size.x / 2, size.y / 2), sf::Vector2f(size.x, size.y)));

					init(gamedata);
				}
				
			}

			elapsed_ -= sf::seconds(config.frame_time);
		}
	}
};

void game_drawer_thread(const GameData& gamedata, game_drawer_config& config, const status& s, sf::RenderWindow*& callback)
{
	LOG_2("game_drawer_thread: Creating RenderWindow...");
	sf::RenderWindow window(config.window_videomode, config.window_name);
	LOG_2("game_drawer_thread: Saving RenderWindow pointer...");
	callback = &window;
	

	LOG_2("game_drawer_thread: Creating game_drawer...");
	game_drawer drawer(gamedata, config);

	LOG_2("game_drawer_thread: Starting draw loop...");
	drawer.start(window, gamedata, config, s);
}