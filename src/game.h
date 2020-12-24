#pragma once


#include "utils/network/server_connector.h"
#include "render/drawer/GameDrawer.h"
#include "render/DrawerController.h"

#include "globals/drawer.h"


class Game
{
protected:

	server_connector& connector;

public:

	GameData gamedata;
	GameDrawer gamedrawer;

	Game(server_connector& connector)
		: connector(connector), gamedrawer(gamedata)
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
		this->drawer_set_state(GameDrawer::Status::UPDATING);

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

		gamedrawer.init();
	}

	void update()
	{
		this->drawer_set_state(GameDrawer::Status::UPDATING);
		global_Drawer.get_window()->setTitle("Update game data...");
		
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
		gamedata.clear();
	}

	void drawer_set_state(GameDrawer::Status s)
	{
		if (drawer_thread == nullptr)
		{
			LOG_2("Game::drawer_set_state: Render thread not started yet");
			return;
		}

		LOG_2("Game::drawer_set_state: Setting state [" << (uint32_t)s << "]...");
		gamedrawer.status_ = s;
		global_Drawer.interrupt();
	}

	void drawer_start()
	{
		global_Drawer.get_window()->setView(sf::View(sf::Vector2f(gamedata.map_graph_width / 2, gamedata.map_graph_height / 2), sf::Vector2f(gamedata.map_graph_width, gamedata.map_graph_height)));

		LOG_2("Game::drawer_start: Starting GameDrawer thread...");
		global_Drawer.interrupt(&gamedrawer);
	}

	void await_run()
	{
		this->drawer_set_state(GameDrawer::Status::AWAIT_PLAYERS);
		global_Drawer.get_window()->setTitle("Awaiting players...");

		this->connector.send_Turn();
		this->connector.read_packet();
	}

	void calculate_move()
	{
		throw "TODO";

		//this->drawer_set_state(GameDrawer::CALCULATING);
	}

	void await_move()
	{
		this->drawer_set_state(GameDrawer::Status::READY);
		global_Drawer.get_window()->setTitle("Awaiting next tick...");

		this->connector.send_Turn();
		this->connector.read_packet();

		//Sleep(1000);
	}

	void start(const std::string& addr, const std::string& port, const server_connector::Login& lobby)
	{
		this->drawer_start();

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