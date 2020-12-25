#pragma once

#include <boost/ptr_container/ptr_map.hpp>

#include <vector>
#include <memory>

#include "graph/graph.h"
#include "graph/KKSCoordsCalculator.h"

#include "data/event.h"
#include "data/train.h"
#include "data/player.h"
#include "data/post.h"




struct GameData
{
	enum GameState : uint8_t
	{
		INIT = 1,
		RUN = 2,
		FINISHED = 3
	};

	GameState game_state;

	Types::player_uid_t player_idx;
	Types::vertex_idx_t home_idx;
	Types::post_idx_t post_idx;
	bool in_game;

	std::map<Types::player_uid_t, Player> players;

	GraphIdx map_graph;
	CoordsHolder* map_graph_coords = nullptr;
	Types::position_t map_graph_width;
	Types::position_t map_graph_height;

	boost::ptr_map<Types::post_idx_t, Posts::Post*> posts;

	void clear()
	{
		players.clear();
		map_graph.clear();
		posts.clear();

		if (map_graph_coords != nullptr)
		{
			delete map_graph_coords;
			map_graph_coords = nullptr;
		}
	}

	static void readJSON_Login(GameData& val, const json& j)
	{
		if (j.find("error") != j.end()) throw std::runtime_error(j["error"].get<std::string>());

		j["idx"].get_to(val.player_idx);
		j["home"]["idx"].get_to(val.home_idx);
		j["home"]["post_idx"].get_to(val.post_idx);
		j["in_game"].get_to(val.in_game);
	}

	static void readJSON_L0(GameData& val, const json& j)
	{
		GraphIdx::readJSON_L0(val.map_graph, j);
	}

	//-------------------- CLIENT-SIDE COORDINATES --------------------//

	static void calculateCoordinates(GameData& val, double topology_width, double topology_height, double unit_edge_length)
	{
		val.map_graph_coords = (CoordsHolder*) new KKSCoordsCalculator(val.map_graph.graph, topology_width, topology_height, unit_edge_length);

		// Read Graph border size
		val.map_graph_width = topology_width;
		val.map_graph_height = topology_height;

		val.map_graph_coords->for_each(val.map_graph.graph, [&](CoordsHolder::point_type& point) {
			point[0] = (point[0] + topology_width) / 2;
			point[1] = (point[1] + topology_height) / 2;
			});
	}

	//-------------------- SERVER-SIDE COORDINATES --------------------//

	static void readJSON_L10(GameData& val, const json& j)
	{
		val.map_graph_coords = new CoordsHolder(val.map_graph.graph);

		// Read Vertex coordinates
		for (const json& ji : j["coordinates"])
		{
			Types::vertex_idx_t idx = ji["idx"].get<Types::vertex_idx_t>();

			CoordsHolder::point_type& point = (*val.map_graph_coords)[val.map_graph.vmap.at(idx)];

			ji["x"].get_to(point[0]);
			ji["y"].get_to(point[1]);
		}

		// Read Graph border size
		val.map_graph_width = j["size"][0].get<Types::position_t>();
		val.map_graph_height = j["size"][1].get<Types::position_t>();
	}

	static void readJSON_L1(GameData& val, const json& j)
	{
		//Parse Players
		for (const auto& [player_idx, ji] : j["ratings"].items())
		{
			//Player memory initialization + value initialization
			Player::readJSON_L1(val.players[player_idx], ji);
		}

		//Parse Trains
		for (const json& ji : j["trains"])
		{
			Types::train_idx_t train_idx = ji["idx"].get<Types::train_idx_t>();
			Types::player_uid_t player_idx = ji["player_idx"].get<Types::player_uid_t>();

			//Train memory initialization + value initialization
			Trains::Train::readJSON_L1(val.players.at(player_idx).trains[train_idx], ji);
		}

		//Parse Posts
		for (const json& ji : j["posts"])
		{
			Types::post_idx_t post_idx = ji["idx"].get<Types::post_idx_t>();

			val.posts[post_idx] = Posts::make_Post(ji);
		}
	}

	static void updateJSON_L1(GameData& val, const json& j)
	{
		//Parse Players
		for (const auto& [player_idx, ji] : j["ratings"].items())
		{
			//Player memory initialization + value initialization
			Player::updateJSON_L1(val.players[player_idx], ji);
		}

		//Parse Trains
		for (const json& ji : j["trains"])
		{
			Types::train_idx_t train_idx = ji["idx"].get<Types::train_idx_t>();
			Types::player_uid_t player_idx = ji["player_idx"].get<Types::player_uid_t>();

			Trains::Train::updateJSON_L1(val.players.at(player_idx).trains[train_idx], ji);
		}

		//Parse Posts
		for (const json& ji : j["posts"])
		{
			Types::post_idx_t post_idx = ji["idx"].get<Types::post_idx_t>();

			Posts::updateJSON_L1(val.posts.at(post_idx), ji);
		}
	}

	CLASS_VIRTUAL_DESTRUCTOR(GameData);
};

struct GameLobby
{
	std::string name;
	uint8_t num_players;
	Types::tick_t num_turns;
	GameData::GameState state;

	static void readJSON_Games(GameLobby& val, const json& j)
	{
		j["name"].get_to(val.name);
		j["num_plaers"].get_to(val.num_players);
		j["num_turns"].get_to(val.num_turns);
		j["state"].get_to(val.state);
	}
};
