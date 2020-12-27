#pragma once

#include <src/utils/network/server_connector.h>

#include <src/lobby/data.h>

#include <src/game.h>

#include <cstdio>

class Lobby
{
public:

	void connect(const std::string& addr, const std::string& port)
	{
		connector.connect(addr, port);
	}

	Lobby(boost::asio::io_service& io)
		: connector(io)
	{

	}

	~Lobby()
	{
		connector.disconnect();
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

	void draw_split() const
	{
		std::cout << std::string(91, '-') << std::endl;
	}

	void draw_clear() const
	{
		std::cout << std::string(20, '\n');
	}

	void cin_clear() const
	{
		std::cin.clear();
		std::cin.ignore(-1, '\n');
	}

	void draw() const
	{
		std::cout << "Lobbies:" << std::endl;
		draw_split();
		std::cout << '|';
		std::cout << std::setw(5) << "#" << '|';
		std::cout << std::setw(40) << "Name" << '|';
		std::cout << std::setw(10) << "Players" << '|';
		std::cout << std::setw(15) << "Turns" << '|';
		std::cout << std::setw(15) << "Status" << '|';
		std::cout << std::endl;
		draw_split();
		for (size_t idx = 0; idx < lobbies.size(); idx++)
		{
			const LobbyData& lobby = lobbies.at(idx);

			std::cout << '|';
			std::cout << std::setw(5) << idx << '|';
			std::cout << std::setw(40) << lobby.name << '|';
			std::cout << std::setw(10) << (int)lobby.num_players << '|';
			std::cout << std::setw(15) << ((lobby.num_turns == -1) ? "Infinity" : std::to_string(lobby.num_turns)) << '|';
			std::cout << std::setw(15);
			switch (lobby.state)
			{
			case LobbyData::INIT: std::cout << "INIT"; break;
			case LobbyData::RUN: std::cout << "RUN"; break;
			case LobbyData::FINISHED: std::cout << "FINISHED"; break;
			}
			std::cout << '|' << std::endl;
		}
		draw_split();
	}

	server_connector::Login choose_lobby() const
	{
		draw();

		std::cout << "Press Enter to create a new Game" << std::endl;

		std::string choice;
		{
			std::cout << "Your choice: ";

			std::getline(std::cin, choice);
			cin_clear();
		}

		server_connector::Login login;

		if (choice.empty())
		{
			{
				std::cout << "Enter name: ";

				std::getline(std::cin, login.name);
				cin_clear();
			}

			{
				std::cout << "Enter password: ";
				std::string password;

				std::getline(std::cin, password);
				cin_clear();

				login.password = password;
			}

			{
				std::cout << "Enter game name: ";
				std::string game;

				std::getline(std::cin, game);
				cin_clear();

				login.game = game;
			}

			{
				std::cout << "Enter number of turns (default: INFINITY): ";
				std::string num_turns;

				std::getline(std::cin, num_turns);
				cin_clear();

				login.num_turns = ((num_turns.length() == 0) ? -1 : std::stoi(num_turns));
			}

			{
				std::cout << "Enter number of players (default: 1): ";
				std::string num_players;

				std::getline(std::cin, num_players);
				cin_clear();

				login.num_players = ((num_players.length() == 0) ? 1 : std::stoi(num_players));
			}
		}
		else
		{
			login.game = lobbies.at(std::stoi(choice)).name;

			{
				std::cout << "Enter name: ";

				std::getline(std::cin, login.name);
				cin_clear();
			}

			{
				std::cout << "Enter password: ";
				std::string password;

				std::getline(std::cin, password);
				cin_clear();

				login.password = password;
			}
		}

		return login;
	}

	void start()
	{
		while (true)
		{
			try {

				init();
				
				server_connector::Login login = choose_lobby();

				Game game(connector);

				draw_clear();

				std::cout << "Starting the game.." << std::endl;
				game.start(login);
			}
			catch (const std::invalid_argument& err)
			{
				std::cout << "Invalid choice! Please try again..." << std::endl;
			}
			catch (const nlohmann::detail::type_error& err)
			{
				std::cout << "JSON parsing ERROR! " << err.what() << std::endl;
			}
			catch (const std::runtime_error& err)
			{
				std::cout << "Runtime ERROR! " << err.what() << std::endl;
			}
			catch (int err)
			{
				std::cout << "ERROR! Code " << err << std::endl;
			}
			catch (server_connector::Result conerr)
			{
				std::cout << "Invalid packet! Error code " << conerr << std::endl;
			}

			std::cout << "Press any key to try again...";
			std::getc(stdin);
			draw_clear();
			
		}
	}



protected:
	server_connector connector;

	std::vector<LobbyData> lobbies;

};
