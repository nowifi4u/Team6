#pragma once


#include "game_connector.h"
#include "game_drawer.h"


class Game
{
protected:

	GameData gamedata;

	boost::thread* drawer_thread = nullptr;
	game_drawer::status drawer_status = game_drawer::READY;

	game_connector connector;

public:

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

	void init(const std::string& addr, const std::string& port, const game_connector::Login& lobby)
	{
		connector.connect(addr, port);

		{
			LOG_1("Game::init: Sending Login request...");
			connector.send_Login(lobby);

			const auto response = connector.read_packet();

			GameData::readJSON_Login(gamedata, json::parse(response.second));
		}

		{
			LOG_1("Game::init: Sending L0 request...");
			connector.send_Map({ 0 });

			const auto response = connector.read_packet();

			GameData::readJSON_L0(gamedata, json::parse(response.second));
		}

		{
			LOG_1("Game::init: Sending L10 request...");
			connector.send_Map({ 10 });

			const auto response = connector.read_packet();

			GameData::readJSON_L10(gamedata, json::parse(response.second));
		}

		{
			LOG_1("Game::init: Sending L1 request...");
			connector.send_Map({ 1 });

			const auto response = connector.read_packet();

			GameData::readJSON_L1(gamedata, json::parse(response.second));
		}
	}

	void update()
	{
		{
			LOG_1("Game::update: Sending L1 request...");
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

	void drawer_set_state(game_drawer::status s)
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

		drawer_config.window_videomode = sf::VideoMode(gamedata.map_graph.graph_props().size_width + 20, gamedata.map_graph.graph_props().size_height + 20);

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
};