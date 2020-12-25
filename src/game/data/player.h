#pragma once

#include "Types.h"

#include <nlohmann/json.hpp>
#include <map>


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

	CLASS_VIRTUAL_DESTRUCTOR(Player);
};