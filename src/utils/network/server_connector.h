#pragma once

#include <optional>

#include <spdlog/spdlog.h>
#include <nlohmann/json.hpp>
using namespace nlohmann;
#include <boost/endian/conversion.hpp>

#include <src/utils/network/tcp_connector.h>
#include <src/Types.h>

#include <src/utils/bincharstream.h>
#include <src/utils/binstream.h>


class server_connector : protected tcp_connector
{
public:

	tcp::socket& socket()
	{
		return tcp_connector::socket();
	}

	const tcp::resolver::results_type& get_endpoint()
	{
		return tcp_connector::get_endpoint();
	}

	server_connector(boost::asio::io_service& m_io)
		: tcp_connector(m_io) {}

	void connect(const std::string& addr, const std::string& port)
	{
        SPDLOG_DEBUG("connecting to {}:{}", addr, port);
		return tcp_connector::connect(addr, port);
	}

    void disconnect()
    {
        SPDLOG_DEBUG("disconnecting...");
        return tcp_connector::disconnect();
    }

	server_connector(boost::asio::io_service& m_io, const std::string& addr, const std::string& port)
		: tcp_connector(m_io, addr, port) 
    {
	    SPDLOG_INFO("constructing");
    }

	void wait_read()
	{
		return tcp_connector::wait_read();
	}

	enum Action : uint32_t
	{
		LOGIN = 1,
		LOGOUT = 2,
		MOVE = 3,
		UPGRADE = 4,
		TURN = 5,
		PLAYER = 6,
		GAMES = 7,
		MAP = 10
	};

protected:

	static std::string _encodeAction(Action action, const std::string& data = "")
	{
		SPDLOG_TRACE("encoding Action: {}, data: \n{}", action, data);

		uint32_t _action = boost::endian::native_to_little((uint32_t)action);
		uint32_t _length = boost::endian::native_to_little(data.length());
		std::stringstream out;
		writeStreamBinary(out, _action);
		writeStreamBinary(out, _length);
		out << data;

		return out.str();
	}

    enum Result : uint32_t
    {
        OKEY = 0,
        BAD_COMMAND = 1,
        RESOURCE_NOT_FOUND = 2,
        ACCESS_DENIED = 3,
        INAPPROPRIATE_GAME_STATE = 4,
        TIMEOUT = 5,
        INTERNAL_SERVER_ERROR = 500
    };

	std::pair<Result, size_t> _read_header()
	{
		std::string buffer = tcp_connector::read_until_size(8);

		BinCharIStream parser(buffer.c_str());

        Result header_action = (Result)boost::endian::little_to_native(parser.read<uint32_t>());
		uint32_t header_size = boost::endian::little_to_native(parser.read<uint32_t>());

		return std::make_pair(header_action, header_size);
	}

public:

    std::pair<Result, std::string> read_packet()
    {
        SPDLOG_TRACE("reading packet...");

        const auto header = _read_header();

        const std::string data = tcp_connector::read_until_size(header.second);

        SPDLOG_TRACE("Action: {}, size: {}, data: \n{}", header.first, header.second, data);

        if (header.first != Result::OKEY) throw header.first;

        return std::make_pair(header.first, data);
    }



    //------------------------------ Login ------------------------------//

    struct Login
    {
        std::string name;

        std::optional<std::string> password;
        std::optional<std::string> game;
        std::optional<Types::tick_t> num_turns;
        std::optional<uint8_t> num_players;

        static std::string encodeJSON(const Login& val)
        {
            SPDLOG_TRACE("encoding login.");

            json j{
                {"name", val.name}
            };
            if (val.password.has_value()) j["password"] = val.password.value();
            if (val.game.has_value()) j["game"] = val.game.value();
            if (val.num_turns.has_value()) j["num_turns"] = val.num_turns.value();
            if (val.num_players.has_value()) j["num_players"] = val.num_players.value();

            SPDLOG_TRACE("encoding login end.");
            return _encodeAction(Action::LOGIN, j.dump());
        }
    };

    void send_Login(const Login& val)
    {
        SPDLOG_TRACE("sending login.");
        return tcp_connector::send(Login::encodeJSON(val));
    }

    void async_send_Login(const Login& val)
    {
        SPDLOG_TRACE("async sending login.");
        return tcp_connector::async_send(Login::encodeJSON(val));
    }

    //------------------------------ Player ------------------------------//

    struct Player
    {
        static std::string encodeJSON()
        {
            SPDLOG_TRACE("encoding Player.");
            return _encodeAction(Action::PLAYER);
        }
    };

    void send_Player()
    {
        SPDLOG_TRACE("sending Player.");
        return tcp_connector::send(Player::encodeJSON());
    }

    void async_send_Player()
    {
        SPDLOG_TRACE("async sending Player.");
        return tcp_connector::async_send(Player::encodeJSON());
    }

    //------------------------------ Logout ------------------------------//

    struct Logout
    {
        static std::string encodeJSON()
        {
            SPDLOG_TRACE("encoding Logout.");
            return _encodeAction(Action::LOGOUT);
        }
    };

    void send_Logout()
    {
        SPDLOG_TRACE("sending Logout.");
        return tcp_connector::send(Logout::encodeJSON());
    }

    void async_send_Logout()
    {
        SPDLOG_TRACE("async sending Logout.");
        return tcp_connector::async_send(Logout::encodeJSON());
    }

    //------------------------------ Map ------------------------------//

    struct Map
    {
        uint8_t layer;

        static std::string encodeJSON(const Map& val)
        {
            SPDLOG_TRACE("encoding Map.");
            json j{
                {"layer", val.layer}
            };

            return _encodeAction(Action::MAP, j.dump());
        }
    };

    void send_Map(const Map& val)
    {
        SPDLOG_TRACE("map Map.");
        return tcp_connector::send(Map::encodeJSON(val));
    }

    void async_send_Map(const Map& val)
    {
        SPDLOG_TRACE("async send Map.");
        return tcp_connector::async_send(Map::encodeJSON(val));
    }

    //------------------------------ Move ------------------------------//

    struct Move
    {
        Types::edge_idx_t line_idx;
        int8_t speed;
        Types::train_idx_t train_idx;

        static std::string encodeJSON(const Move& val)
        {
            SPDLOG_TRACE("encode Move.");
            json j{
                {"line_idx", val.line_idx},
                {"speed", val.speed},
                {"train_idx", val.train_idx}
            };

            return _encodeAction(Action::MOVE, j.dump());
        }
    };

    void send_Move(const Move& val)
    {
        SPDLOG_TRACE("send Move.");
        return tcp_connector::send(Move::encodeJSON(val));
    }

    void async_send_Move(const Move& val)
    {
        SPDLOG_TRACE("async send Move.");
        return tcp_connector::async_send(Move::encodeJSON(val));
    }

    //------------------------------ Upgrade ------------------------------//

    struct Upgrade
    {
        std::vector<Types::post_idx_t> posts;
        std::vector<Types::train_idx_t> trains;

        static std::string encodeJSON(const Upgrade& val)
        {
            SPDLOG_TRACE("encode Upgrade.");
            json j{
                {"posts", val.posts},
                {"trains", val.trains}
            };

            return _encodeAction(Action::UPGRADE, j.dump());
        }
    };

    void send_Upgrade(const Upgrade& val)
    {
        SPDLOG_TRACE("send Upgrade.");
        return tcp_connector::send(Upgrade::encodeJSON(val));
    }

    void async_send_Upgrade(const Upgrade& val)
    {
        SPDLOG_TRACE("async send Upgrade.");
        return tcp_connector::async_send(Upgrade::encodeJSON(val));
    }

    //------------------------------ Turn ------------------------------//

    struct Turn
    {
        static std::string encodeJSON()
        {
            SPDLOG_TRACE("encode Turn.");
            return _encodeAction(Action::TURN);
        }
    };

    void send_Turn()
    {
        SPDLOG_TRACE("send Turn.");
        return tcp_connector::send(Turn::encodeJSON());
    }

    void async_send_Turn()
    {
        SPDLOG_TRACE("async send Turn.");
        return tcp_connector::async_send(Turn::encodeJSON());
    }

    //------------------------------ Games ------------------------------//

    struct Games
    {
        static std::string encodeJSON()
        {
            SPDLOG_TRACE("encode Game.");
            return _encodeAction(Action::GAMES);
        }
    };

    void send_Games()
    {
        SPDLOG_TRACE("send Game.");
        return tcp_connector::send(Games::encodeJSON());
    }

    void async_send_Games()
    {
        SPDLOG_TRACE("async send Game.");
        return tcp_connector::async_send(Games::encodeJSON());
    }

};
