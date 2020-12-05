#pragma once

#include <string>
#include <sstream>
#include <iomanip>
#include <memory>
#include <optional>

#include <boost/typeof/decltype.hpp>

#include <boost/endian/conversion.hpp>
#include <boost/endian/arithmetic.hpp>
#include <boost/endian/buffers.hpp>

#include <nlohmann/json.hpp>


#include "Types.h"
#include "bincharstream.h"
#include "binstream.h"

#include "Logging.h"


namespace Packets {

    using nlohmann::json;

    enum Action
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



    inline std::pair<Action, json> decodeJSON(const char* data)
    {
        BinCharIStream parser(data);

        std::pair<Action, json> result;
        uint32_t _action = boost::endian::little_to_native(*parser.read<uint32_t>());
        uint32_t _length = boost::endian::little_to_native(*parser.read<uint32_t>());
        result.first = (Action) _action;
        result.second = json::parse(parser.get_offset() + parser.release());
        return result;
    }



    inline std::string encodeString(Action action, const std::string& data = "")
    {
        uint32_t _action = boost::endian::native_to_little((uint32_t) action);
        uint32_t _length = boost::endian::native_to_little(data.length());
        std::stringstream out;
        writeStreamBinary(out, _action);
        writeStreamBinary(out, _length);
        out << data;
        return data;
    }

    namespace encode {

       

        struct Login
        {
            std::string name;

            std::optional<std::string> password;
            std::optional<std::string> game;
            std::optional<Types::tick_t> num_turns;
            std::optional<uint8_t> num_players;

            static std::string encodeJSON(const Login& val);
        };

        struct Player
        {
            static std::string encodeJSON();
        };

        struct Logout
        {
            static std::string encodeJSON();
        };

        struct Map
        {
            uint8_t layer;

            static std::string encodeJSON(const Map& val);
        };

        struct Move
        {
            Types::edge_idx_t line_idx;
            int8_t speed;
            Types::train_idx_t train_idx;

            static std::string encodeJSON(const Move& val);
        };

        struct Upgrade
        {
            std::vector<Types::post_idx_t> posts;
            std::vector<Types::train_idx_t> trains;

            static std::string encodeJSON(const Upgrade& val);
        };

        struct Turn
        {
            static std::string encodeJSON();
        };

        struct Games
        {
            static std::string encodeJSON();
        };

    } // namespace encode

} // namespace Packets