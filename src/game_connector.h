#pragma once

#include <vector>
#include <thread>
#include <boost/asio.hpp>
#include <boost/thread.hpp>

#include "packet-parser.h"
#include "ClassDefines.h"

using boost::asio::ip::tcp;

struct game_connector
{
	boost::asio::io_service& io;
	boost::asio::io_service::work work;
	tcp::socket socket;
	tcp::resolver::results_type endpoint;

	game_connector(boost::asio::io_service& io)
		: io(io), work(io), socket(io) {}

	void connect(const std::string addr, const std::string port)
	{
		endpoint = tcp::resolver(io).resolve({ addr, port });
		boost::asio::connect(socket, endpoint);
	}

	size_t send(const std::string data)
	{
		return socket.send(boost::asio::buffer(data));
	}

	static void _async_send_handler(const boost::system::error_code& ec, std::size_t size)
	{
		if (ec.failed())
		{
			std::cerr << ec.message() << std::endl;
			throw ec.value();
		}
	}

	void async_send(const std::string& data)
	{
		socket.async_send(boost::asio::buffer(data), _async_send_handler);
	}

	void wait_read()
	{
		socket.wait(boost::asio::socket_base::wait_type::wait_read);
	}

	std::string _read_until_eof()
	{
		std::string data(socket.available(), '0');

		socket.read_some(boost::asio::buffer(data));

		return data;
	}

	std::string _read_until_size(size_t size)
	{
		std::string data(size, '0');

		boost::asio::read(socket, boost::asio::buffer(data), boost::asio::transfer_exactly(size));

		return data;
	}

	std::pair<Packets::Action, size_t> _read_header()
	{
		std::string buffer = _read_until_size(8);

		BinCharIStream parser(buffer.c_str());

		Packets::Action header_action = (Packets::Action) boost::endian::little_to_native(parser.read<uint32_t>());
		uint32_t header_size = boost::endian::little_to_native(parser.read<uint32_t>());

		return std::make_pair(header_action, header_size);
		
	}

	std::pair<Packets::Action, std::string> read_packet()
	{
		const auto header = _read_header();

		return std::make_pair(header.first, _read_until_size(header.second));
	}
};