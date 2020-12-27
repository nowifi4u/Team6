#pragma once

#include <spdlog/spdlog.h>

#include "utils/network/server_connector.h"
#include "render/game_drawer.h"

#include "utils/MinMax.h"


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
	    SPDLOG_DEBUG("constructing from io.");
	}

	~Game()
	{
        SPDLOG_DEBUG("destructing.");
		this->reset();
	}

	void connect(const std::string& addr, const std::string& port)
	{
	    SPDLOG_INFO("Connecting to {}:{}", addr, port);
		connector.connect(addr, port);
	}

	void init(const server_connector::Login& lobby)
	{
		this->drawer_set_state(status::UPDATING);
	    SPDLOG_DEBUG("initiating game started.");

		{
			SPDLOG_TRACE("sending Login request...");
			connector.send_Login(lobby);

			const auto response = connector.read_packet();
			SPDLOG_TRACE("received response code: {}", response.first);

			GameData::readJSON_Login(gamedata, json::parse(response.second));
		}

		{
			SPDLOG_TRACE("sending L0 request...");
			connector.send_Map({ 0 });

			const auto response = connector.read_packet();
            SPDLOG_TRACE("received response code: {}", response.first);

			GameData::readJSON_L0(gamedata, json::parse(response.second));
		}

		{
			SPDLOG_TRACE("sending L10 request...");
			connector.send_Map({ 10 });

			const auto response = connector.read_packet();
            SPDLOG_TRACE("received response code: {}", response.first);

			GameData::readJSON_L10(gamedata, json::parse(response.second));
		}

		{
			SPDLOG_TRACE("sending L1 request...");
			connector.send_Map({ 1 });

			const auto response = connector.read_packet();
            SPDLOG_TRACE("received response code: {}", response.first);

			GameData::readJSON_L1(gamedata, json::parse(response.second));
		}

		{
			MinMaxReducer<float> minmax_x;
			MinMaxReducer<float> minmax_y;

			gamedata.map_graph.for_each_vertex_descriptor([&](GraphIdx::vertex_descriptor v) {
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
			SPDLOG_DEBUG("updating game data...");
			connector.send_Map({ 1 });

			const auto response = connector.read_packet();
            SPDLOG_TRACE("received response code: {}", response.first);

			GameData::readJSON_L1(gamedata, json::parse(response.second));
		}
	}

	void reset()
	{
		SPDLOG_DEBUG("resetting game.");

		connector.disconnect();
		this->drawer_stop();
		gamedata.clear();
        SPDLOG_DEBUG("resetting game end.");
	}

	void drawer_set_state(status s)
	{
		if (drawer_thread == nullptr)
		{
			SPDLOG_WARN("render thread not started yet.");
			return;
		}

        SPDLOG_DEBUG("changing drawer state from {} to {}", drawer_status, s);
		drawer_status = s;
		drawer_thread->interrupt();
	}

	void drawer_start()
	{
		if (drawer_thread != nullptr)
		{
            SPDLOG_WARN("render thread already started.");
			return;
		}

		drawer_config.window_videomode = sf::VideoMode({ 800, 800 });

		drawer_config.padding_width.set_output(100, 700);
		drawer_config.padding_height.set_output(100, 700);

		drawer_config.edge_length_font = WORKING_DIRECTORY + "res/arial.ttf";

		drawer_config.textures = new TextureManager("res/Game/textures.cfg");

		SPDLOG_INFO("starting game_drawer thread...");
		drawer_thread = new boost::thread(&game_drawer_thread, boost::ref(gamedata), boost::ref(drawer_config), boost::ref(drawer_status), boost::ref(drawer_window));
	}

	void drawer_stop()
	{
		if (drawer_thread == nullptr)
		{
			SPDLOG_WARN("render thread not started yet");
			return;
		}

		SPDLOG_DEBUG("stopping game_drawer thread...");
		delete drawer_thread;
		drawer_thread = nullptr;
		drawer_window = nullptr;

		delete drawer_config.textures;
		drawer_config.textures = nullptr;
	}

	// normalii net?
	void drawer_window_wait() const
	{
	    // TODO Rework to wait/notify
		SPDLOG_DEBUG("waiting for drawer_window...");
		while (drawer_window == nullptr);
		SPDLOG_DEBUG("drawer_window was created.");
	}

	void drawer_join()
	{
		if (drawer_thread == nullptr)
		{
			SPDLOG_WARN("render thread not started yet.");
			return;
		}

		SPDLOG_DEBUG("joining game_drawer thread...");
		drawer_thread->join();
	}

	// what it actually for?
	void await_run()
	{
        SPDLOG_DEBUG("waiting for run...");
		this->drawer_set_state(AWAIT_PLAYERS);
		this->drawer_window->setTitle("Awaiting players...");

        SPDLOG_DEBUG("sending turn package...");
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
        SPDLOG_DEBUG("waiting for move...");
		this->drawer_set_state(status::READY);
		this->drawer_window->setTitle("Awaiting next tick...");

        SPDLOG_DEBUG("sending turn package...");
		this->connector.send_Turn();
		this->connector.read_packet();

		//Sleep(1000);
	}

	void start(const std::string& addr, const std::string& port, const server_connector::Login& lobby)
	{
	    SPDLOG_INFO("starting game on {}:{}", addr, port);

		this->drawer_start();
		this->drawer_window_wait();

		this->await_run();
		this->update();

		SPDLOG_INFO("about to start game cycle.");
		while (true /*gamedata.game_state == GameData::GameState::RUN*/)
		{
            SPDLOG_TRACE("game iteration start.");
			//this->calculate_move();

			this->await_move();

			this->update();
            SPDLOG_TRACE("game iteration end.");
		}

		SPDLOG_INFO("game ended.");
	}
};
