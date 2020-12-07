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


    inline std::string encodeString(Action action, const std::string& data = "")
    {
        LOG_2("encodeString: Encoding {Action:" << action << ",data:" << data << "}");

        uint32_t _action = boost::endian::native_to_little((uint32_t) action);
        uint32_t _length = boost::endian::native_to_little(data.length());
        std::stringstream out;
        writeStreamBinary(out, _action);
        writeStreamBinary(out, _length);
        out << data;
        return out.str();
    }

    namespace encode {

       

        struct Login
        {
            std::string name;

            std::optional<std::string> password;
            std::optional<std::string> game;
            std::optional<Types::tick_t> num_turns;
            std::optional<uint8_t> num_players;

            static std::string encodeJSON(const Login& val)
            {
                json j{
                    {"name", val.name}
                };
                if (val.password.has_value()) j["password"] = val.password.value();
                if (val.game.has_value()) j["game"] = val.game.value();
                if (val.num_turns.has_value()) j["num_turns"] = val.num_turns.value();
                if (val.num_players.has_value()) j["num_players"] = val.num_players.value();

                return Packets::encodeString(Action::LOGIN, j.dump());
            }
        };

        struct Player
        {
            static std::string encodeJSON()
            {
                return Packets::encodeString(Action::PLAYER);
            }
        };

        struct Logout
        {
            static std::string encodeJSON()
            {
                return Packets::encodeString(Action::LOGOUT);
            }
        };

        struct Map
        {
            uint8_t layer;

            static std::string encodeJSON(const Map& val)
            {
                json j{
                    {"layer", val.layer}
                };

                return Packets::encodeString(Action::MAP, j.dump());
            }
        };

        struct Move
        {
            Types::edge_idx_t line_idx;
            int8_t speed;
            Types::train_idx_t train_idx;

            static std::string encodeJSON(const Move& val)
            {
                json j{
                    {"line_idx", val.line_idx},
                    {"speed", val.speed},
                    {"train_idx", val.train_idx}
                };

                return Packets::encodeString(Action::MOVE, j.dump());
            }
        };

        struct Upgrade
        {
            std::vector<Types::post_idx_t> posts;
            std::vector<Types::train_idx_t> trains;

            static std::string encodeJSON(const Upgrade& val)
            {
                json j{
                    {"posts", val.posts},
                    {"trains", val.trains}
                };

                return Packets::encodeString(Action::UPGRADE, j.dump());
            }
        };

        struct Turn
        {
            static std::string encodeJSON()
            {
                return Packets::encodeString(Action::TURN);
            }
        };

        struct Games
        {
            static std::string encodeJSON()
            {
                return Packets::encodeString(Action::GAMES);
            }
        };

    } // namespace encode

} // namespace Packets