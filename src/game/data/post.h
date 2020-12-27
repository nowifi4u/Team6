#pragma once

#include "Types.h"

#include <nlohmann/json.hpp>
#include <boost/ptr_container/ptr_vector.hpp>

#include <spdlog/spdlog.h>
#include "spdlog/fmt/ostr.h"

namespace Posts {



	enum PostType : uint8_t
	{
		TOWN = 1,
		MARKET = 2,
		STORAGE = 3
	};

	struct Post
	{
		Types::vertex_idx_t idx;
		std::string name;
		Types::vertex_idx_t point_idx;

		boost::ptr_vector<Events::Event> events;

		virtual PostType type() const = 0;

        virtual std::basic_ostream<char>& doprint(std::basic_ostream<char> &os) const
        {
            os  << "[Post "
                << "idx="       << idx       << ", "
                << "name="      << name      << ", "
                << "point_idx=" << point_idx; // << ", ";

            /*
            os << "events=";
            {
                os << "[\n";
                for (const auto& e  : events)
                {
                    os << '\t' << e << '\n';
                }
                os << "]";
            }
             */
            os << "]";

            return os;
        }

        template<typename OStream>
        friend OStream &operator<<(OStream &os, const Post &p)
        {
            return p.doprint(os);
        }

		CLASS_VIRTUAL_DESTRUCTOR(Post);
	};

	struct Storage : public Post
	{
		uint32_t armor;
		uint32_t armor_capacity;
		uint32_t replenishment;

		PostType type() const { return PostType::STORAGE; }

		[[nodiscard]]
		static Storage* readJSON_L1(const json& j)
		{
			Storage* ptr = new Storage;

			j["armor"].get_to(ptr->armor);
			j["armor_capacity"].get_to(ptr->armor_capacity);
			j["replenishment"].get_to(ptr->replenishment);

			return ptr;
		}

		static void updateJSON_L1(Storage* ptr, const json& j)
		{
			j["armor"].get_to(ptr->armor);
			//j["armor_capacity"].get_to(ptr->armor_capacity);
			//j["replenishment"].get_to(ptr->replenishment);
		}

		CLASS_VIRTUAL_DESTRUCTOR(Storage);
	};

	struct Market : public Post
	{
		uint32_t product;
		uint32_t product_capacity;
		uint32_t replenishment;

		PostType type() const { return PostType::MARKET; }

		[[nodiscard]]
		static Market* readJSON_L1(const json& j)
		{
			Market* ptr = new Market;

			j["product"].get_to(ptr->product);
			j["product_capacity"].get_to(ptr->product_capacity);
			j["replenishment"].get_to(ptr->replenishment);

			return ptr;
		}

		static void updateJSON_L1(Market* ptr, const json& j)
		{
			j["product"].get_to(ptr->product);
			//j["product_capacity"].get_to(ptr->product_capacity);
			//j["replenishment"].get_to(ptr->replenishment);
		}

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
		static Town* readJSON_L1(const json& j)
		{
			Town* ptr = new Town;

			j["armor"].get_to(ptr->armor);
			j["level"].get_to(ptr->level);
			ptr->player_idx = j["player_idx"].is_null() ? "" : j["player_idx"].get<Types::player_uid_t>();
			j["population"].get_to(ptr->population);
			j["product"].get_to(ptr->product);
			j["train_cooldown"].get_to(ptr->train_cooldown);

			return ptr;
		}

		static void updateJSON_L1(Town* ptr, const json& j)
		{
			j["armor"].get_to(ptr->armor);
			j["level"].get_to(ptr->level);
			//j["player_idx"].get_to(ptr->player_idx);
			j["population"].get_to(ptr->population);
			j["product"].get_to(ptr->product);
			j["train_cooldown"].get_to(ptr->train_cooldown);
		}

		CLASS_VIRTUAL_DESTRUCTOR(Town);
	};

	[[nodiscard]]
	inline Post* make_Post(const json& j)
	{
		PostType post_type = (PostType)j["type"].get<uint8_t>();

		Post* ptr;

		switch (post_type)
		{
		case PostType::TOWN:	ptr = reinterpret_cast<Post*>(Town::readJSON_L1(j)); break;
		case PostType::MARKET:	ptr = reinterpret_cast<Post*>(Market::readJSON_L1(j)); break;
		case PostType::STORAGE:	ptr = reinterpret_cast<Post*>(Storage::readJSON_L1(j)); break;
		default:				return nullptr;
		}

		j["idx"].get_to(ptr->idx);
		j["name"].get_to(ptr->name);
		j["point_idx"].get_to(ptr->point_idx);

		Events::make_Event_vector(ptr->events, j);

		return ptr;
	}

	inline void updateJSON_L1(Post* ptr, const json& j)
	{
		PostType post_type = ptr->type();

		switch (post_type)
		{
		case PostType::TOWN:	Town::updateJSON_L1(dynamic_cast<Town*>(ptr), j); break;
		case PostType::MARKET:	Market::updateJSON_L1(dynamic_cast<Market*>(ptr), j); break;
		case PostType::STORAGE:	Storage::updateJSON_L1(dynamic_cast<Storage*>(ptr), j); break;
		}

		Events::make_Event_vector(ptr->events, j);
	}

} // namespace Posts