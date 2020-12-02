#pragma once

#include <boost/ptr_container/ptr_map.hpp>

#include <map>
#include <vector>
#include <memory>
#include <string>

#include "ClassDefines.h"
#include "Events.h"


namespace GameData {

	enum GameState
	{
		INIT = 1,
		RUN = 2,
		FINISHED = 3
	};

	struct GameConfig
	{
		std::string name;
		uint32_t num_players;
		tick_t num_turns;
		GameState state;
	};

	struct GameList
	{
		std::vector<GameConfig> games;
	};

	

	//------------------------------ EVENTS ------------------------------//

	

	//------------------------------ TRAIN ------------------------------//

	struct Train_Tier
	{
		const uint32_t goods_capacity;
		const uint32_t fuel_capacity;
		const uint64_t next_level_price;
	};

	const Train_Tier TrainTiers[3]
	{
		{40,400,40},
		{80,800,80},
		{160,1600,UINT32_MAX}
	};

	struct Train
	{
		const train_idx_t idx;
		uint8_t tier;
		std::vector<Event> events;
		uint64_t cooldown;
		uint32_t fuel;
		uint32_t goods;
		edge_idx_t line_idx;
		player_uid_t player_idx;
		edge_length_idx_t position;
		int8_t speed;
	};

	//------------------------------ POST ------------------------------//

	struct Position
	{
		int32_t x;
		int32_t y;
	};

	enum PostType
	{
		TOWN = 1,
		MARKET = 2,
		STORAGE = 3
	};

	struct Post
	{
		const edge_idx_t idx;
		const Position pos;
		std::vector<Event> events;

		const std::string name;
		uint32_t point_idx;
		EventType events_;

		virtual const PostType type() const = 0;

		VIRTUAL_DESTRUCTOR(Post);
	};

	struct Storage : public Post
	{
		uint32_t armor;
		uint32_t armor_capacity;
		uint32_t replenishment;

		const PostType type() const { PostType::STORAGE; }

		VIRTUAL_DESTRUCTOR(Storage);
	};

	struct Market : public Post
	{
		uint32_t product;
		uint32_t product_capacity;
		uint32_t replenishment;

		const PostType type() const { PostType::MARKET; }

		VIRTUAL_DESTRUCTOR(Market);
	};

	struct Town_Tier
	{
		const uint32_t population_capacity;
		const uint32_t product_capacity;
		const uint32_t armor_capacity;
		const uint64_t cooldown_after_crash;
		const uint64_t next_level_price;
	};

	struct Town : public Post
	{
		uint32_t armor;
		uint32_t armor_capacity;
		uint8_t level;
		//uint32_t next_level_price;
		player_uid_t player_idx;
		uint32_t population;
		uint32_t population_capacity;
		uint32_t product;
		uint32_t product_capacity;
		uint64_t train_cooldown;

		const PostType type() const { PostType::TOWN; }

		VIRTUAL_DESTRUCTOR(Town);
	};

	struct Player
	{
		const player_idx_t idx;
		const std::string name;
		const player_uid_t player_idx;
		int32_t rating;

		std::map<train_idx_t, Train> trains;
	};

	struct PlayerCreds
	{
		std::string name;
		std::string password;
	};

	class GameDataClass
	{
	public:	
		GameDataClass() {}
		GameConfig gamestatus;
		std::map<player_uid_t, Player> players;
		boost::ptr_map<post_idx_t, Post*> posts;
		//std::map<post_idx_t, Post> posts;
	};
}