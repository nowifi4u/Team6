#pragma once

#include <src/game/data/base_json_encodable.h>
#include <boost/ptr_container/ptr_vector.hpp>


namespace Events {

	enum EventType : uint8_t
	{
		TRAIN_COLLISION = 1,
		HIJACKERS_ASSAULT = 2,
		PARASITES_ASSAULT = 3,
		REFUGEES_ARRIVAL = 4,
		RESOURCE_OVERFLOW = 5,
		RESOURCE_LACK = 6,
		GAME_OVER = 100
	};

	struct Event : public base_json_encodable
	{
		virtual EventType type() const = 0;
	};

	struct Event_TrainCrash : public Event
	{
		Types::train_idx_t train;
		Types::tick_t tick;

		EventType type() const { return EventType::TRAIN_COLLISION; }

		json encodeJSON() const
		{
			json j;

			j["type"] = EventType::TRAIN_COLLISION;
			j["trains"] = train;
			j["tick"] = tick;

			return j;
		}

		[[nodiscard]]
		static Event_TrainCrash* readJSON_L1(const json& j)
		{
			Event_TrainCrash* event = new Event_TrainCrash;

			j["trains"].get_to(event->train);
			j["tick"].get_to(event->tick);

			return event;
		}

		CLASS_VIRTUAL_DESTRUCTOR(Event_TrainCrash);
	};

	struct Event_Parasites : public Event
	{
		uint8_t parasite_power;
		Types::tick_t tick;

		EventType type() const { return EventType::PARASITES_ASSAULT; }

		json encodeJSON() const
		{
			json j;

			j["type"] = EventType::PARASITES_ASSAULT;
			j["parasites_power"] = parasite_power;
			j["tick"] = tick;

			return j;
		}

		[[nodiscard]]
		static Event_Parasites* readJSON_L1(const json& j)
		{
			Event_Parasites* event = new Event_Parasites;

			j["parasites_power"].get_to(event->parasite_power);
			j["tick"].get_to(event->tick);

			return event;
		}

		CLASS_VIRTUAL_DESTRUCTOR(Event_Parasites);
	};

	struct Event_Bandits : public Event
	{
		uint8_t hijacker_power;
		Types::tick_t tick;

		EventType type() const { return EventType::HIJACKERS_ASSAULT; }

		json encodeJSON() const
		{
			json j;

			j["type"] = EventType::HIJACKERS_ASSAULT;
			j["hijackers_power"] = hijacker_power;
			j["tick"] = tick;

			return j;
		}

		[[nodiscard]]
		static Event_Bandits* readJSON_L1(const json& j)
		{
			Event_Bandits* event = new Event_Bandits;

			j["hijackers_power"].get_to(event->hijacker_power);
			j["tick"].get_to(event->tick);

			return event;
		}

		CLASS_VIRTUAL_DESTRUCTOR(Event_Bandits);
	};

	struct Event_Refugees : public Event
	{
		uint8_t refugees_number;
		Types::tick_t tick;

		EventType type() const { return EventType::REFUGEES_ARRIVAL; }

		json encodeJSON() const
		{
			json j;

			j["type"] = EventType::REFUGEES_ARRIVAL;
			j["refugees_number"] = refugees_number;
			j["tick"] = tick;

			return j;
		}



		[[nodiscard]]
		static Event_Refugees* readJSON_L1(const json& j)
		{
			Event_Refugees* event = new Event_Refugees;

			j["refugees_number"].get_to(event->refugees_number);
			j["tick"].get_to(event->tick);

			return event;
		}

		CLASS_VIRTUAL_DESTRUCTOR(Event_Refugees);
	};

	struct Event_ResourceOverflow : public Event
	{
		EventType type() const { return EventType::RESOURCE_OVERFLOW; }

		json encodeJSON() const
		{
			json j;

			j["type"] = EventType::RESOURCE_OVERFLOW;

			return j;
		}

		[[nodiscard]]
		static Event_ResourceOverflow* readJSON_L1(const json& j)
		{
			Event_ResourceOverflow* event = new Event_ResourceOverflow;

			return event;
		}

		CLASS_VIRTUAL_DESTRUCTOR(Event_ResourceOverflow);
	};

	struct Event_ResourceLack : public Event
	{
		EventType type() const { return EventType::RESOURCE_LACK; }

		json encodeJSON() const
		{
			json j;

			j["type"] = EventType::RESOURCE_LACK;

			return j;
		}

		[[nodiscard]]
		static Event_ResourceLack* readJSON_L1(const json& j)
		{
			Event_ResourceLack* event = new Event_ResourceLack;

			return event;
		}

		CLASS_VIRTUAL_DESTRUCTOR(Event_ResourceLack);
	};

	struct Event_GameOver : public Event
	{
		EventType type() const { return EventType::GAME_OVER; }

		json encodeJSON() const
		{
			json j;

			j["type"] = EventType::GAME_OVER;

			return j;
		}

		[[nodiscard]]
		static Event_GameOver* readJSON_L1(const json& j)
		{
			Event_GameOver* event = new Event_GameOver;

			return event;
		}

		CLASS_VIRTUAL_DESTRUCTOR(Event_GameOver);
	};

	[[nodiscard]]
	inline Event* make_Event(const json& j)
	{
		EventType event_type = (EventType)j["type"].get<int>();

		LOG("Event! " << j);

		switch (event_type)
		{
		case EventType::TRAIN_COLLISION:	return dynamic_cast<Event*>(Event_TrainCrash::readJSON_L1(j));
		case EventType::PARASITES_ASSAULT:	return dynamic_cast<Event*>(Event_Parasites::readJSON_L1(j));
		case EventType::HIJACKERS_ASSAULT:	return dynamic_cast<Event*>(Event_Bandits::readJSON_L1(j));
		case EventType::REFUGEES_ARRIVAL:	return dynamic_cast<Event*>(Event_Refugees::readJSON_L1(j));
		case EventType::RESOURCE_OVERFLOW:	return dynamic_cast<Event*>(Event_ResourceOverflow::readJSON_L1(j));
		case EventType::RESOURCE_LACK:		return dynamic_cast<Event*>(Event_ResourceLack::readJSON_L1(j));
		case EventType::GAME_OVER:			return dynamic_cast<Event*>(Event_GameOver::readJSON_L1(j));
		default:							return nullptr;
		}
	}

	inline void make_Event_vector(boost::ptr_vector<Event>& vec, const json& j)
	{
		vec.clear();
		vec.reserve(j["events"].size());
		for (const json& ji : j["events"])
		{
			vec.push_back(make_Event(ji));
		}
	}

} // namespace Events
