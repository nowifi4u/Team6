#pragma once

#include <string>
#include <vector>
#include <functional>
#include <boost/asio.hpp>
#include <spdlog/spdlog.h>


#include <src/utils/ClassDefines.h>


using boost::asio::ip::tcp;

class tcp_connector
{

	boost::asio::io_service& m_io;
	boost::asio::io_service::work m_work;
	tcp::socket m_socket;
	tcp::resolver::results_type m_endpoint;

public:

	tcp::socket& socket()
	{
		return m_socket;
	}

	const tcp::resolver::results_type& get_endpoint()
	{
		return m_endpoint;
	}

	tcp_connector(boost::asio::io_service& m_io)
		: m_io(m_io), m_work(m_io), m_socket(m_io)
    {
	    SPDLOG_DEBUG("constructing");

    }

	void connect(const std::string& addr, const std::string& port)
	{
	    SPDLOG_DEBUG("connecting to {}:{}", addr, port);
		m_endpoint = tcp::resolver(m_io).resolve({ addr, port });
		boost::asio::connect(m_socket, m_endpoint);
	}

	void disconnect()
	{
	    SPDLOG_DEBUG("disconnecting...");
		m_socket.close();
	}

	tcp_connector(boost::asio::io_service& m_io, const std::string& addr, const std::string& port)
		: tcp_connector(m_io)
	{
		connect(addr, port);
	}

	void send(const std::string& data)
	{
		SPDLOG_TRACE("sending packet with data: \n{}", data);

		m_socket.send(boost::asio::buffer(data));
	}

	static void async_default_handler(const boost::system::error_code& ec, std::size_t size)
	{
		if (ec.failed())
		{
            SPDLOG_ERROR("*smth* failed, message: {}", std::string(ec.message()));
			throw ec;
		}
	}

	void async_send(const std::string& data)
	{
		SPDLOG_TRACE("sending async packet...");

		m_socket.async_send(boost::asio::buffer(data), async_default_handler);
	}

	template <class AsyncHandler>
	void async_send(const std::string& data, AsyncHandler handler)
	{
		SPDLOG_TRACE("sending async packet with callback...");

		m_socket.async_send(boost::asio::buffer(data), handler);
	}

	void wait_read()
	{
		SPDLOG_TRACE("waiting for answer...");

		m_socket.wait(boost::asio::socket_base::wait_type::wait_read);
        SPDLOG_TRACE("answer received.");
	}

	std::string read_until_eof()
	{
		std::string data(m_socket.available(), '0');

		m_socket.read_some(boost::asio::buffer(data));

		return data;
	}

	std::string read_until_size(size_t size)
	{
		std::string data(size, '0');

		boost::asio::read(m_socket, boost::asio::buffer(data), boost::asio::transfer_exactly(size));

		return data;
	}

	template <class AsyncHandler>
	void async_read_until_eof(std::string& buffer, AsyncHandler handler)
	{
		m_socket.async_read_some(boost::asio::buffer(buffer), handler);
	}

	template <class AsyncHandler>
	void async_read_until_size(std::string& buffer, size_t size, AsyncHandler handler)
	{
		boost::asio::async_read(m_socket, boost::asio::buffer(buffer), handler, boost::asio::transfer_exactly(size));
	}

};
