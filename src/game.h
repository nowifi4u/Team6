#pragma once


#include <src/utils/network/server_connector.h>
#include <src/render/game_drawer.h>
#include <src/game/solver.h>

#include <src/utils/MinMax.h>


class Game
{
protected:

	boost::thread* drawer_thread = nullptr;
	status drawer_status = status::READY;

	server_connector connector;

public:

	GameData gamedata;

	sf::RenderWindow* drawer_window = nullptr;
	game_drawer_config drawer_config;

	Game(boost::asio::io_service& io)
		: connector(io)
	{

	}

	~Game()
	{
		this->reset();
	}

	void connect(const std::string& addr, const std::string& port)
	{
		connector.connect(addr, port);
	}

	void init(const server_connector::Login& lobby)
	{
		this->drawer_set_state(status::UPDATING);

		{
			LOG_2("Game::init: Sending Login request...");
			connector.send_Login(lobby);

			const auto response = connector.read_packet();

			GameData::readJSON_Login(gamedata, json::parse(response.second));
		}

		{
			LOG_2("Game::init: Sending L0 request...");
			connector.send_Map({ 0 });

			const auto response = connector.read_packet();

			GameData::readJSON_L0(gamedata, json::parse(response.second));
		}

		{
			LOG_2("Game::init: Sending L10 request...");
			connector.send_Map({ 10 });

			const auto response = connector.read_packet();

			GameData::readJSON_L10(gamedata, json::parse(response.second));
		}

		{
			LOG_2("Game::init: Sending L1 request...");
			connector.send_Map({ 1 });

			const auto response = connector.read_packet();

			GameData::readJSON_L1(gamedata, json::parse(response.second));
		}

		{
			MinMaxReducer<float> minmax_x;
			MinMaxReducer<float> minmax_y;

			Graph::for_each_vertex_descriptor(gamedata.graph(), [&](Graph::vertex_descriptor v) {
				const CoordsHolder::point_type& vcoords = gamedata.map_graph_coords->get_map()[v];

				minmax_x.consume(vcoords[0]);
				minmax_y.consume(vcoords[1]);
				});

			drawer_config.padding_width.set_input(minmax_x.min(), minmax_x.max());
			drawer_config.padding_height.set_input(minmax_y.min(), minmax_y.max());
		}
	}

	void update()
	{
		this->drawer_set_state(status::UPDATING);
		this->drawer_window->setTitle("Update game data...");

		{
			LOG("Updating game data...");
			connector.send_Map({ 1 });

			const auto response = connector.read_packet();

			GameData::readJSON_L1(gamedata, json::parse(response.second));
		}
	}

	void reset()
	{
		LOG_2("Game::reset");

		connector.disconnect();
		this->drawer_stop();
		gamedata.clear();
	}

	void drawer_set_state(status s)
	{
		if (drawer_thread == nullptr)
		{
			LOG_2("Game::drawer_set_state: Render thread not started yet");
			return;
		}

		LOG_2("Game::drawer_set_state: Setting state [" << (uint32_t)s << "]...");
		drawer_status = s;
		drawer_thread->interrupt();
	}

	void drawer_start()
	{
		if (drawer_thread != nullptr)
		{
			LOG_2("Game::drawer_start: Render thread already running!");
			return;
		}

		drawer_config.window_videomode = sf::VideoMode({ 800, 800 });

		drawer_config.padding_width.set_output(100, 700);
		drawer_config.padding_height.set_output(100, 700);

		drawer_config.edge_length_font = WORKING_DIRECTORY + "res/arial.ttf";

		drawer_config.textures = new TextureManager("res/Game/textures.cfg");

		LOG_2("Game::drawer_start: Starting game_drawer thread...");
		drawer_thread = new boost::thread(&game_drawer_thread, boost::ref(gamedata), boost::ref(drawer_config), boost::ref(drawer_status), boost::ref(drawer_window));
	}

	void drawer_stop()
	{
		if (drawer_thread == nullptr)
		{
			LOG_2("Game::drawer_stop: Render thread not started yet");
			return;
		}

		LOG_2("Game::drawer_stop: Stopping game_drawer thread...");
		delete drawer_thread;
		drawer_thread = nullptr;
		drawer_window = nullptr;

		delete drawer_config.textures;
		drawer_config.textures = nullptr;
	}

	void drawer_window_wait() const
	{
		LOG_2("Game::drawer_window_wait: Waiting for drawer_window...");
		while (drawer_window == nullptr);
		LOG_2("Game::drawer_window_wait: drawer_window ready!");
	}

	void drawer_join()
	{
		if (drawer_thread == nullptr)
		{
			LOG_2("Game::drawer_join: Render thread not started yet");
			return;
		}

		LOG_2("Game::drawer_join: Joining game_drawer thread...");
		drawer_thread->join();
	}

	void await_run()
	{
		this->drawer_set_state(AWAIT_PLAYERS);
		this->drawer_window->setTitle("Awaiting players...");

		this->connector.send_Turn();
		this->connector.read_packet();
	}

	void calculate_move()
	{
		throw "TODO";

		//this->drawer_set_state(game_drawer::CALCULATING);
	}

	void await_move()
	{
		this->drawer_set_state(status::READY);
		this->drawer_window->setTitle("Awaiting next tick...");

		this->connector.send_Turn();
		this->connector.read_packet();

		//Sleep(1000);
	}

	void start(const std::string& addr, const std::string& port, const server_connector::Login& lobby)
	{
		this->drawer_start();
		this->drawer_window_wait();

		this->await_run();
		this->update();

		while (true /*gamedata.game_state == GameData::GameState::RUN*/)
		{
			//this->calculate_move();

			this->await_move();

			this->update();
		}

		LOG("Game::start: Game ended.");
	}
};