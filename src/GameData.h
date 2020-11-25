#pragma once

#include <boost/ptr_container/ptr_map.hpp>

#include <map>

#include "ClassDefines.h"
#include "GameList.h"

struct GameData
{

	//------------------------------          ------------------------------//
	//------------------------------          ------------------------------//
	//------------------------------ TYPEDEFS ------------------------------//
	//------------------------------          ------------------------------//
	//------------------------------          ------------------------------//

	using GameState = GameList::GameState;
	using GameConfig = GameList::GameConfig;
	using tick_t = GameList::tick_t;

	using player_uid_t = std::string;
	using player_idx_t = uint32_t;
	using train_idx_t = uint32_t;
	using post_idx_t = uint32_t;
	using edge_idx_t = uint32_t;
	using edge_length_idx_t = uint32_t;

	using product_t = uint64_t;
	using population_t = uint32_t;
	using armor_t = uint32_t;

	//------------------------------ EVENT ------------------------------//

	enum EventType
	{
		TRAIN_COLLISION = 1,
		HIJACKERS_ASSAULT = 2,
		PARASITES_ASSAULT = 3,
		REFUGEES_ARRIVAL = 4,
		RESOURCE_OVERFLOW = 5,
		RESOURCE_LACK = 6,
		GAME_OVER = 100
	}; 

	struct Event 
	{
		virtual constexpr EventType type() const = 0;
	};

	struct Event_Parasites : public Event
	{
		uint8_t parasite_power;
		tick_t tick;

		constexpr EventType type() const { return EventType::PARASITES_ASSAULT; }
	};

	struct Event_Bandits : public Event
	{
		uint8_t hijacker_power;
		tick_t tick;

		constexpr EventType type() const { return EventType::HIJACKERS_ASSAULT; }
	};

	struct Event_Refugees : public Event
	{
		uint8_t refugees_number;
		tick_t tick;

		constexpr EventType type() const { return EventType::REFUGEES_ARRIVAL; }
	};

	struct Event_TrainCrash : public Event
	{
		train_idx_t train;
		tick_t tick;

		constexpr EventType type() const { return EventType::TRAIN_COLLISION; }
	};

	struct Event_ResourceOverflow : public Event
	{
		constexpr EventType type() const { return EventType::RESOURCE_OVERFLOW; }
	};

	struct Event_ResourceLack : public Event
	{
		constexpr EventType type() const { return EventType::RESOURCE_LACK; }
	};

	struct Event_GameOver : public Event
	{
		constexpr EventType type() const { return EventType::GAME_OVER; }
	};

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
		EventType events;

		virtual constexpr PostType type() const = 0;

		VIRTUAL_DESTRUCTOR(Post);
	};

	struct Storage : public Post
	{
		uint32_t armor;
		uint32_t armor_capacity;
		uint32_t replenishment;

		constexpr PostType type() const { PostType::STORAGE; }

		VIRTUAL_DESTRUCTOR(Storage);
	};

	struct Market : public Post
	{
		uint32_t product;
		uint32_t product_capacity;
		uint32_t replenishment;

		constexpr PostType type() const { PostType::MARKET; }

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

		constexpr PostType type() const { PostType::TOWN; }

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

	//------------------------------          ------------------------------//
	//------------------------------          ------------------------------//
	//------------------------------   IMPL   ------------------------------//
	//------------------------------          ------------------------------//
	//------------------------------          ------------------------------//

	GameConfig gamestatus;
	std::map<player_uid_t, Player> players;
	boost::ptr_map<post_idx_t, Post*> posts;
	//std::map<post_idx_t, Post> posts;

	GameData() {}



};