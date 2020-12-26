//
// Created by non on 12/26/20.
//

#ifndef TEAM6_CONTROLLER_H
#define TEAM6_CONTROLLER_H

#include "utils/network/server_connector.h"

class GameController
{
private:
    GameData& gamedata;
    server_connector connector;

public:
    GameController(GameData& gamedata, boost::asio::io_service& io)
        : gamedata(gamedata), connector(io)
    {
        SPDLOG_DEBUG("constructing from io.");
    }


    void init(const server_connector::Login& lobby)
    {
        SPDLOG_DEBUG("initiating game controller.");

        {
            SPDLOG_TRACE("sending Login request...");
            connector.send_Login(lobby);

            const auto response = connector.read_packet();
            SPDLOG_TRACE("received response code: {}", response.first);

            GameData::readJSON_Login(gamedata, json::parse(response.second));
        }

        {
            SPDLOG_TRACE("sending L0 request...");
            connector.send_Map({ 0 });

            const auto response = connector.read_packet();
            SPDLOG_TRACE("received response code: {}", response.first);

            GameData::readJSON_L0(gamedata, json::parse(response.second));
        }

        {
            SPDLOG_TRACE("sending L10 request...");
            connector.send_Map({ 10 });

            const auto response = connector.read_packet();
            SPDLOG_TRACE("received response code: {}", response.first);

            GameData::readJSON_L10(gamedata, json::parse(response.second));
        }

        {
            SPDLOG_TRACE("sending L1 request...");
            connector.send_Map({ 1 });

            const auto response = connector.read_packet();
            SPDLOG_TRACE("received response code: {}", response.first);

            GameData::readJSON_L1(gamedata, json::parse(response.second));
        }

    }

    void connect(const std::string& addr, const std::string& port)
    {
        SPDLOG_INFO("Connecting to {}:{}", addr, port);
        connector.connect(addr, port);
    }

    void update()
    {
        SPDLOG_DEBUG("updating game data...");
        connector.send_Map({ 1 });

        const auto response = connector.read_packet();
        SPDLOG_TRACE("received response code: {}", response.first);

        GameData::readJSON_L1(gamedata, json::parse(response.second));
    }
    void reset()
    {
        SPDLOG_DEBUG("resetting controller.");

        connector.disconnect();
        gamedata.clear();
        SPDLOG_DEBUG("resetting controller end.");
    }

    void await_run()
    {
        SPDLOG_DEBUG("sending turn package...");
        this->connector.send_Turn();
        SPDLOG_DEBUG("waiting for turn package answer...");
        this->connector.read_packet();
    }

    void await_move()
    {
        SPDLOG_DEBUG("sending turn package...");
        this->connector.send_Turn();
        SPDLOG_DEBUG("waiting for turn package answer...");
        this->connector.read_packet();
    }


};

#endif //TEAM6_CONTROLLER_H
