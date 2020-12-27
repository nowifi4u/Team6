#pragma once

#include "Types.h"

#include <nlohmann/json.hpp>
using namespace nlohmann;
#include <boost/ptr_container/ptr_vector.hpp>

#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>


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

	struct Event
	{
		virtual EventType type() const = 0;

        virtual std::basic_ostream<char>& doprint(std::basic_ostream<char> &os) const = 0;

        template<typename OStream>
        friend OStream &operator<<(OStream &os, const Event &e)
        {
            return e.doprint(os);
        }

	};

	struct Event_TrainCrash : public Event
	{
		Types::train_idx_t train;
		Types::tick_t tick;

		EventType type() const { return EventType::TRAIN_COLLISION; }

		[[nodiscard]]
		static Event_TrainCrash* readJSON_L1(const json& j)
		{
			Event_TrainCrash* event = new Event_TrainCrash;

			j["trains"].get_to(event->train);
			j["tick"].get_to(event->tick);

			return event;
		}

        virtual std::basic_ostream<char>& doprint(std::basic_ostream<char> &os) const
        {
            os  << "[Event TrainCrash "
                << "train_idx=" << train << ", "
                << "tick="      << tick;//  << ", "

            os << ']';

            return os;
        }

	};

	struct Event_Parasites : public Event
	{
		uint8_t parasite_power;
		Types::tick_t tick;

		EventType type() const { return EventType::PARASITES_ASSAULT; }

		[[nodiscard]]
		static Event_Parasites* readJSON_L1(const json& j)
		{
			Event_Parasites* event = new Event_Parasites;

			j["parasites_power"].get_to(event->parasite_power);
			j["tick"].get_to(event->tick);

			return event;
		}

        virtual std::basic_ostream<char>& doprint(std::basic_ostream<char> &os) const
        {
            os  << "[Event Parasites "
                << "parasite_power="    << (uint32_t) parasite_power << ", "
                << "tick="              << tick;//           << ", "

            os << ']';

            return os;
        }

	};

	struct Event_Bandits : public Event
	{
		uint8_t hijacker_power;
		Types::tick_t tick;

		EventType type() const { return EventType::HIJACKERS_ASSAULT; }

		[[nodiscard]]
		static Event_Bandits* readJSON_L1(const json& j)
		{
			Event_Bandits* event = new Event_Bandits;

			j["hijackers_power"].get_to(event->hijacker_power);
			j["tick"].get_to(event->tick);

			return event;
		}

        virtual std::basic_ostream<char>& doprint(std::basic_ostream<char> &os) const
        {
            os  << "[Event Bandits "
                << "hijacker_power="    << (uint32_t) hijacker_power << ", "
                << "tick="              << tick;//           << ", "

            os << ']';

            return os;
        }

	};

	struct Event_Refugees : public Event
	{
		uint8_t refugees_number;
		Types::tick_t tick;

		EventType type() const { return EventType::REFUGEES_ARRIVAL; }

		[[nodiscard]]
		static Event_Refugees* readJSON_L1(const json& j)
		{
			Event_Refugees* event = new Event_Refugees;

			j["refugees_number"].get_to(event->refugees_number);
			j["tick"].get_to(event->tick);

			return event;
		}

        virtual std::basic_ostream<char>& doprint(std::basic_ostream<char> &os) const
        {
            os  << "[Event Refugees "
                << "refugees_number="    << (uint32_t) refugees_number << ", "
                << "tick="               << tick;//           << ", "

            os << ']';

            return os;
        }

	};

	struct Event_ResourceOverflow : public Event
	{
		EventType type() const { return EventType::RESOURCE_OVERFLOW; }

		[[nodiscard]]
		static Event_ResourceOverflow* readJSON_L1(const json& j)
		{
			Event_ResourceOverflow* event = new Event_ResourceOverflow;

			return event;
		}

        virtual std::basic_ostream<char>& doprint(std::basic_ostream<char> &os) const
        {
            os  << "[Event ResourceOverflow ";
            os << ']';

            return os;
        }

	};

	struct Event_ResourceLack : public Event
	{
		EventType type() const { return EventType::RESOURCE_LACK; }

		[[nodiscard]]
		static Event_ResourceLack* readJSON_L1(const json& j)
		{
			Event_ResourceLack* event = new Event_ResourceLack;

			return event;
		}

        virtual std::basic_ostream<char>& doprint(std::basic_ostream<char> &os) const
        {
            os  << "[Event ResourceLack ";
            os << ']';

            return os;
        }

	};

	struct Event_GameOver : public Event
	{
		EventType type() const { return EventType::GAME_OVER; }

		[[nodiscard]]
		static Event_GameOver* readJSON_L1(const json& j)
		{
			Event_GameOver* event = new Event_GameOver;

			return event;
		}

        virtual std::basic_ostream<char>& doprint(std::basic_ostream<char> &os) const
        {
            os  << "[Event GameOver ";
            os << ']';

            return os;
        }

	};

	[[nodiscard]]
	inline Event* make_Event(const json& j)
	{
		EventType event_type = (EventType)j["type"].get<int>();

		SPDLOG_DEBUG("game_data::make_Event: {} ", j);

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
			SPDLOG_DEBUG("Last pushed event: {}", vec[vec.size()-1]);
		}
	}

} // namespace Events
