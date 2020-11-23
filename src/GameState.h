#pragma once

#include <string>
#include <map>
#include <vector>

#define VIRTUAL_DESTRUCTOR(CLASS) \
virtual ~##CLASS##()

struct GameGtate
{

	//------------------------------ TYPEDEFS ------------------------------//

	enum EventType
	{
		__EVENT__ = 0,
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
		virtual constexpr EventType type() = 0;
	};

	struct Event_Parasites : public Event
	{
		uint8_t parasite_power;
		uint64_t tick;

		constexpr EventType type() { return EventType::PARASITES_ASSAULT; }
	};

	struct Event_Bandits : public Event
	{
		uint8_t hijacker_power;
		uint64_t tick;

		constexpr EventType type() { return EventType::HIJACKERS_ASSAULT; }
	};

	struct Event_Refugees : public Event
	{
		uint8_t refugees_number;
		uint64_t tick;

		constexpr EventType type() { return EventType::REFUGEES_ARRIVAL; }
	};

	struct Event_TrainCrash : public Event
	{
		uint32_t train;
		uint64_t tick;

		constexpr EventType type() { return EventType::TRAIN_COLLISION; }
	};

	struct Event_ResourceOverflow : public Event
	{
		constexpr EventType type() { return EventType::RESOURCE_OVERFLOW; }
	};

	struct Event_ResourceLack : public Event
	{
		constexpr EventType type() { return EventType::RESOURCE_LACK; }
	};

	struct Event_GameOver : public Event
	{
		constexpr EventType type() { return EventType::GAME_OVER; }
	};

	struct Position
	{
		int32_t x;
		int32_t y;
	};

	struct Vertex
	{
		const uint32_t idx;
		Position pos; // const ??

		VIRTUAL_DESTRUCTOR(Vertex);
	};

	
	enum PostType
	{
		__POST__ = 0,
		TOWN = 1,
		MARKET = 2,
		STORAGE = 3
	};

	struct Post : public Vertex
	{
		std::vector<Event> events;

		const std::string name;
		uint32_t point_idx;
		EventType events;

		static inline constexpr PostType type = PostType::__POST__;

		VIRTUAL_DESTRUCTOR(Post);
	};

	struct Storage : public Post
	{
		uint32_t armor;
		uint32_t armor_capacity;
		uint32_t replenishment;

		static inline constexpr PostType type = PostType::STORAGE;

		VIRTUAL_DESTRUCTOR(Storage);
	};

	struct Market : public Post
	{
		uint32_t product;
		uint32_t product_capacity;
		uint32_t replenishment;

		static inline constexpr PostType type = PostType::MARKET;

		VIRTUAL_DESTRUCTOR(Market);
	};

	struct Train
	{

	};

	struct Town : public Post
	{
		uint32_t armor;
		uint32_t armor_capacity;
		uint8_t level;
		//uint32_t next_level_price;
		std::string player_idx;
		uint32_t population;
		uint32_t population_capacity;
		uint32_t product;
		uint32_t product_capacity;
		uint32_t train_cooldown;

		static inline constexpr PostType type = PostType::TOWN;

		VIRTUAL_DESTRUCTOR(Town);
	};

	struct Player
	{
		uint32_t idx;
		std::string name;
		int32_t rating;
		std::string player_idx;
		std::string password;

		//std::map<uint32_t, 
	};
};