#pragma once

#include <src/game/data/base_json_encodable.h>
#include <map>


struct Player : base_json_encodable
{
	Types::player_uid_t idx;
	std::string name;
	int32_t rating;

	std::map<Types::train_idx_t, Trains::Train> trains;

	json encodeJSON() const
	{
		json j;

		j["idx"] = idx;
		j["name"] = name;
		j["rating"] = rating;

		return j;
	}

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
