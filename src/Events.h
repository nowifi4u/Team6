#pragma once
#include <cstdint>

namespace GameData {

	/*typedef block*/
	using tick_t = uint64_t; 
	using player_uid_t = std::string;
	using player_idx_t = uint32_t;
	using train_idx_t = uint32_t;
	using post_idx_t = uint32_t;
	using edge_idx_t = uint32_t;
	using edge_length_idx_t = uint32_t;
	using product_t = uint64_t;
	using population_t = uint32_t;
	using armor_t = uint32_t;

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
		virtual const EventType type() const = 0;
	};

	struct Event_Parasites : public Event
	{
		uint8_t parasite_power;
		tick_t tick;

		const EventType type() const { return EventType::PARASITES_ASSAULT; }
	};

	struct Event_Bandits : public Event
	{
		uint8_t hijacker_power;
		tick_t tick;

		const EventType type() const { return EventType::HIJACKERS_ASSAULT; }
	};

	struct Event_Refugees : public Event
	{
		uint8_t refugees_number;
		tick_t tick;

		const EventType type() const { return EventType::REFUGEES_ARRIVAL; }
	};

	struct Event_TrainCrash : public Event
	{
		train_idx_t train;
		tick_t tick;

		const EventType type() const { return EventType::TRAIN_COLLISION; }
	};

	struct Event_ResourceOverflow : public Event
	{
		const EventType type() const { return EventType::RESOURCE_OVERFLOW; }
	};

	struct Event_ResourceLack : public Event
	{
		const EventType type() const { return EventType::RESOURCE_LACK; }
	};

	struct Event_GameOver : public Event
	{
		const EventType type() const { return EventType::GAME_OVER; }
	};

}