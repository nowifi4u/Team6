#pragma once

#include "Types.h"

#include <nlohmann/json.hpp>
#include <map>

#include <spdlog/spdlog.h>
#include "spdlog/fmt/ostr.h"

struct Player
{
	Types::player_uid_t idx;
	std::string name;
	int32_t rating;

	std::map<Types::train_idx_t, Trains::Train> trains;

	static void readJSON_L1(Player& val, const json& j)
	{
		j["idx"].get_to(val.idx);
		j["name"].get_to(val.name);
		j["rating"].get_to(val.rating);
	}

	static void updateJSON_L1(Player& val, const json& j)
	{
		//j["idx"].get_to(val.idx);
		//j["name"].get_to(val.name);
		j["rating"].get_to(val.rating);
	}

    template<typename OStream>
    friend OStream &operator<<(OStream &os, const Player &p)
    {
        os  << "[Player "
                << "idx="       << p.idx    << ", "
                << "name="      << p.name   << ", "
                << "rating="    << p.rating << ", ";

        os << '\n';
        os << "trains=";
        {
            os << "[\n";
            for (const auto& [idx, t]  : p.trains)
            {
                os << '\t' << t << '\n';
            }
            os << "]";
        }
        os << "]";

        return os;
    }

    CLASS_VIRTUAL_DESTRUCTOR(Player);
};