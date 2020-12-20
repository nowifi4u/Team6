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

	game_drawer_config drawer_config;

	Game(boost::asio::io_service& io)
		: connector(io)
	{

	}

	~Game()
	{
		this->reset();
	}

	void init_gamedata(const std::string& addr, const std::string& port, const game_connector::Login& lobby)
	{
		connector.connect(addr, port);

		{
			LOG_1("Sending Login request...");
			connector.send_Login(lobby);

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
	}

	void update_gamedata()
	{
		{
			LOG_1("Sending L1 request...");
			connector.send_Map({ 1 });

			const auto response = connector.read_packet();

			GameData::readJSON_L1(gamedata, json::parse(response.second));
		}
	}

	void reset()
	{
		connector.disconnect();
		this->drawer_stop();
		gamedata.clear();
	}

	void drawer_set_state(game_drawer::status s)
	{
		if (drawer_thread == nullptr)
		{
			std::cerr << "Render thread not started yet" << std::endl;
			return;
		}

		drawer_status = s;
		drawer_thread->interrupt();
	}

	void drawer_start()
	{
		if (drawer_thread != nullptr)
		{
			std::cerr << "Render thread already running!" << std::endl;
			return;
		}

		LOG_2("Game: Starting game_drawer thread...");
		drawer_thread = new boost::thread(&game_drawer_thread, boost::ref(gamedata), boost::ref(drawer_config), boost::ref(drawer_status));
	}

	void drawer_stop()
	{
		if (drawer_thread == nullptr)
		{
			std::cerr << "Render thread not started yet" << std::endl;
			return;
		}

		LOG_2("Game: Stopping game_drawer thread...");
		delete drawer_thread;
		drawer_thread = nullptr;
	}

	void drawer_join()
	{
		if (drawer_thread == nullptr)
		{
			std::cerr << "Render thread not started yet" << std::endl;
			return;
		}

		LOG_2("Game: Joining game_drawer thread...");
		drawer_thread->join();
	}
};