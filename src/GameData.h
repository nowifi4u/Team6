#pragma once

#include <boost/ptr_container/ptr_map.hpp>
#include <boost/ptr_container/ptr_vector.hpp>

#include <vector>
#include <memory>
#include <map>

#include "ClassDefines.h"
#include "Types.h"

#include "json-parser.h"
#include "graph.h"

#include "Logging.h"



//------------------------------ EVENTS ------------------------------//

namespace Events {



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
		virtual EventType type() const = 0;

		[[nodiscard]]
		static Event* readJSON_L1(const ptree& pt);

		static void readJSON_L1_vector(boost::ptr_vector<Event>& vec, const ptree& pt);
	};

	struct Event_TrainCrash : public Event
	{
		Types::train_idx_t train;
		Types::tick_t tick;

		EventType type() const { return EventType::TRAIN_COLLISION; }

		[[nodiscard]]
		static Event_TrainCrash* readJSON_L1(const ptree& pt);

		CLASS_VIRTUAL_DESTRUCTOR(Event_TrainCrash);
	};

	struct Event_Parasites : public Event
	{
		uint8_t parasite_power;
		Types::tick_t tick;

		EventType type() const { return EventType::PARASITES_ASSAULT; }

		[[nodiscard]]
		static Event_Parasites* readJSON_L1(const ptree& pt);

		CLASS_VIRTUAL_DESTRUCTOR(Event_Parasites);
	};

	struct Event_Bandits : public Event
	{
		uint8_t hijacker_power;
		Types::tick_t tick;

		EventType type() const { return EventType::HIJACKERS_ASSAULT; }

		[[nodiscard]]
		static Event_Bandits* readJSON_L1(const ptree& pt);

		CLASS_VIRTUAL_DESTRUCTOR(Event_Bandits);
	};

	struct Event_Refugees : public Event
	{
		uint8_t refugees_number;
		Types::tick_t tick;

		EventType type() const { return EventType::REFUGEES_ARRIVAL; }

		[[nodiscard]]
		static Event_Refugees* readJSON_L1(const ptree& pt);

		CLASS_VIRTUAL_DESTRUCTOR(Event_Refugees);
	};

	struct Event_ResourceOverflow : public Event
	{
		EventType type() const { return EventType::RESOURCE_OVERFLOW; }

		[[nodiscard]]
		static Event_ResourceOverflow* readJSON_L1(const ptree& pt);

		CLASS_VIRTUAL_DESTRUCTOR(Event_ResourceOverflow);
	};

	struct Event_ResourceLack : public Event
	{
		EventType type() const { return EventType::RESOURCE_LACK; }

		[[nodiscard]]
		static Event_ResourceLack* readJSON_L1(const ptree& pt);

		CLASS_VIRTUAL_DESTRUCTOR(Event_ResourceLack);
	};

	struct Event_GameOver : public Event
	{
		EventType type() const { return EventType::GAME_OVER; }

		[[nodiscard]]
		static Event_GameOver* readJSON_L1(const ptree& pt);

		CLASS_VIRTUAL_DESTRUCTOR(Event_GameOver);
	};



} // namespace Events



//------------------------------ TRAINS ------------------------------//

namespace Trains {



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
		Types::train_idx_t idx;
		uint8_t tier;
		Types::tick_t cooldown;
		uint32_t fuel;
		uint32_t goods;
		Types::edge_idx_t line_idx;
		Types::player_uid_t player_idx;
		Types::edge_length_t position;
		int8_t speed;

		boost::ptr_vector<Events::Event> events;

		static void readJSON_L1(Train& val, const ptree& pt);

		static void updateJSON(Train& val, const ptree& pt);
	};



} // namespace Trains



//------------------------------ POSTS ------------------------------//

namespace Posts {



	enum PostType
	{
		TOWN = 1,
		MARKET = 2,
		STORAGE = 3
	};

	struct Post
	{
		Types::edge_idx_t idx;
		std::string name;
		uint32_t point_idx;

		boost::ptr_vector<Events::Event> events;

		virtual PostType type() const = 0;

		[[nodiscard]]
		static Post* readJSON_L1(const ptree& pt);
		static void updateJSON(Post* ptr, const ptree& pt);

		CLASS_VIRTUAL_DESTRUCTOR(Post);
	};

	struct Storage : public Post
	{
		uint32_t armor;
		uint32_t armor_capacity;
		uint32_t replenishment;

		PostType type() const { return PostType::STORAGE; }

		[[nodiscard]]
		static Storage* readJSON_L1(const ptree& pt);
		static void updateJSON(Storage* ptr, const ptree& pt);

		CLASS_VIRTUAL_DESTRUCTOR(Storage);
	};

	struct Market : public Post
	{
		uint32_t product;
		uint32_t product_capacity;
		uint32_t replenishment;

		PostType type() const { return PostType::MARKET; }

		[[nodiscard]]
		static Market* readJSON_L1(const ptree& pt);
		static void updateJSON(Market* ptr, const ptree& pt);

		CLASS_VIRTUAL_DESTRUCTOR(Market);
	};

	struct Town_Tier
	{
		const uint32_t population_capacity;
		const uint32_t product_capacity;
		const uint32_t armor_capacity;
		const Types::tick_t cooldown_after_crash;
		const uint64_t next_level_price;
	};

	struct Town : public Post
	{
		uint32_t armor;
		uint8_t level;
		Types::player_uid_t player_idx;
		uint32_t population;
		uint32_t product;
		Types::tick_t train_cooldown;

		PostType type() const { return PostType::TOWN; }

		[[nodiscard]]
		static Town* readJSON_L1(const ptree& pt);
		static void updateJSON(Town* ptr, const ptree& pt);

		CLASS_VIRTUAL_DESTRUCTOR(Town);
	};



} // namespace Posts



//------------------------------ PLAYER ------------------------------//

struct Player
{
	Types::player_uid_t idx;
	std::string name;
	int32_t rating;

	std::map<Types::train_idx_t, Trains::Train> trains;

	static void readJSON_L1(Player& val, const ptree& pt);
	static void updateJSON(Player& val, const ptree& pt);

	CLASS_VIRTUAL_DESTRUCTOR(Player);
};



//------------------------------ GAMEDATA ------------------------------//

struct GameData
{
	enum GameState
	{
		INIT = 1,
		RUN = 2,
		FINISHED = 3
	};

	GameState game_state;

	Types::player_uid_t player_idx;
	uint32_t home_idx;
	Types::post_idx_t post_idx;
	bool in_game;

	std::map<Types::player_uid_t, Player> players;

	GraphIdx graph;
	boost::ptr_map<Types::post_idx_t, Posts::Post> posts;

	static void readJSON_Login(GameData& val, const ptree& pt);
	static void readJSON_L0(GameData& val, const ptree& pt);
	static void readJSON_L10(GameData& val, const ptree& pt);
	static void readJSON_L1(GameData& val, const ptree& pt);

	static void updateJSON(GameData& val, const ptree& pt);

	CLASS_VIRTUAL_DESTRUCTOR(GameData);
};