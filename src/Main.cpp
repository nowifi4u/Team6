#define LOG_LEVEL_3

#include <sdkddkver.h>

#include <iostream>

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include <src/lobby.h>

const std::string DEFAULT_ADDR = "wgforge-srv.wargaming.net";
const std::string DEFAULT_PORT = "443";

int main()
{
	boost::asio::io_service io;

	Lobby lobby(io);

	while (true)
	{
		try
		{
			std::string addr, port;
			
			{
				std::cout << "Enter the IP address (default: " << DEFAULT_ADDR << "): ";

				std::getline(std::cin, addr);
				std::cin.clear();
				std::cin.ignore(-1, '\n');

				if (addr.empty())
				{
					addr = DEFAULT_ADDR;
				}
			}

			{
				std::cout << "Enter the port (default: " << DEFAULT_PORT << "): ";

				std::getline(std::cin, port);
				std::cin.clear();
				std::cin.ignore(-1, '\n');

				if (port.empty())
				{
					port = DEFAULT_PORT;
				}
			}

			lobby.connect(addr, port);

			lobby.start();
		}
		catch (boost::asio::error::basic_errors& err)
		{
			std::cout << "ASIO error! " << err;
		}
		catch (...)
		{
			std::cout << "Uncaught ERROR!" << std::endl;
		}
	}
	lobby.start();

	return 0;
}