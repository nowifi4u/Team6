#pragma once

#include <src/utils/network/server_connector.h>

#include <src/lobby/data.h>

#include <src/game.h>

class Lobby
{
public:

	Lobby(boost::asio::io_service& io, const std::string& addr, const std::string& port)
		: connector(io, addr, port)
	{

	}

	void init()
	{
		{
			LOG_2("Game::init: Sending Login request...");
			connector.send_Games();

			const auto response = connector.read_packet();

			LobbyData::readJSON_Games_vector(lobbies, json::parse(response.second));
		}
	}

	void draw() const
	{
		std::cout << "Lobbies:" << std::endl;
		for (size_t idx = 0; idx < lobbies.size(); idx++)
		{
			const LobbyData& lobby = lobbies.at(idx);

			std::cout << "#:" << idx << std::setw(4);
			std::cout << "Name:" << lobby.name << std::setw(16);
			std::cout << "Player:" << lobby.num_players << std::setw(4);
			std::cout << "Turns:" << ((lobby.num_turns == -1) ? "Infinity" : std::to_string(lobby.num_turns)) << std::setw(6);
			std::cout << "State:";
			switch (lobby.state)
			{
			case LobbyData::INIT: std::cout << "INIT";
			case LobbyData::RUN: std::cout << "RUN";
			case LobbyData::FINISHED: std::cout << "FINISHED";
			}
			std::cout << std::endl;
		}
	}

	void start()
	{
		while (true)
		{
			try {

				init();
				draw();

				std::cout << "'new' for creating a new lobby";
				std::cout << "Your choice: ";

				std::string choice;
				std::cin >> choice;

				server_connector::Login login;

				{
					std::cout << "Enter name: ";

					std::getline(std::cin, login.name);
					std::cin.clear();
					std::cin.ignore();
				}

				{
					std::cout << "Enter password: ";
					std::string password;

					std::getline(std::cin, password);
					std::cin.clear();
					std::cin.ignore(-1, '\n');

					login.password = password;
				}

				if (choice == "new")
				{
					{
						std::cout << "Enter game name: ";
						std::string game;

						std::getline(std::cin, game);
						std::cin.clear();
						std::cin.ignore(-1, '\n');

						login.game = game;
					}

					{
						std::cout << "Enter number of turns (default: INFINITY): ";
						std::string num_turns;

						std::getline(std::cin, num_turns);
						std::cin.clear();
						std::cin.ignore(-1, '\n');

						login.num_turns = ((num_turns.length() == 0) ? -1 : std::stoul(num_turns));
					}

					{
						std::cout << "Enter number of players (default: 1): ";
						std::string num_players;

						std::getline(std::cin, num_players);
						std::cin.clear();
						std::cin.ignore(-1, '\n');

						login.num_players = ((num_players.length() == 0) ? 1 : std::stoi(num_players));
					}
				}
				else
				{
					login.game = lobbies.at(std::stoi(choice)).name;
				}

				// TODO: Game start
			}
			catch (int err)
			{

			}
			catch(...)
			{
				std::cout << "Some error occured." << std::endl;
			}

			std::cout << "Press any key to try again...";

			std::cout << "-----------------------------------------------------------------" << std::endl;
			
		}
	}



protected:
	server_connector connector;

	std::vector<LobbyData> lobbies;

};
