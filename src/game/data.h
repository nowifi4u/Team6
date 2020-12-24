#pragma once

#include <boost/ptr_container/ptr_map.hpp>
#include <boost/ptr_container/ptr_vector.hpp>

#include <vector>
#include <memory>
#include <map>

#include "../utils/ClassDefines.h"
#include "../Types.h"

#include "../graph.h"
#include "../KKSCoordsCalculator.h"

#include "../utils/Logging.h"



//------------------------------ EVENTS ------------------------------//

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

		CLASS_VIRTUAL_DESTRUCTOR(Event_TrainCrash);
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

		CLASS_VIRTUAL_DESTRUCTOR(Event_Parasites);
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

		CLASS_VIRTUAL_DESTRUCTOR(Event_Bandits);
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

		CLASS_VIRTUAL_DESTRUCTOR(Event_Refugees);
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

		CLASS_VIRTUAL_DESTRUCTOR(Event_ResourceOverflow);
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

		CLASS_VIRTUAL_DESTRUCTOR(Event_ResourceLack);
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

		CLASS_VIRTUAL_DESTRUCTOR(Event_GameOver);
	};

	[[nodiscard]]
	inline Event* make_Event(const json& j)
	{
		EventType event_type = (EventType)j["type"].get<int>();

		LOG_1("game_data::make_Event: " << j);

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

	enum GoodsType : uint8_t
	{
		None = 0,
		Product = 1,
		Armor = 2
	};

	struct Train
	{
		Types::train_idx_t idx;
		uint8_t level;
		Types::tick_t cooldown;
		//DISABLED uint32_t fuel;
		uint32_t goods;
		GoodsType goods_type;
		Types::edge_idx_t line_idx;
		Types::player_uid_t player_idx;
		Types::edge_length_t position;
		int8_t speed;

		boost::ptr_vector<Events::Event> events;

		static void readJSON_L1(Train& val, const json& j)
		{
			j["idx"].get_to(val.idx);
			j["level"].get_to(val.level);
			j["cooldown"].get_to(val.cooldown);
			//DISABLED j["fuel"].get_to(val.fuel);
			j["goods"].get_to(val.goods);
			val.goods_type = j["goods_type"].is_null() ? GoodsType::None : j["goods_type"].get<GoodsType>();
			j["line_idx"].get_to(val.line_idx);
			j["player_idx"].get_to(val.player_idx);
			j["position"].get_to(val.position);
			j["speed"].get_to(val.speed);

			Events::make_Event_vector(val.events, j);
		}

		static void updateJSON_L1(Train& val, const json& j)
		{
			//j["idx"].get_to(val.idx);
			j["level"].get_to(val.level);
			j["cooldown"].get_to(val.cooldown);
			//DISABLED j["fuel"].get_to(val.fuel);
			j["goods"].get_to(val.goods);
			val.goods_type = j["goods_type"].is_null() ? GoodsType::None : j["goods_type"].get<GoodsType>();
			j["line_idx"].get_to(val.line_idx);
			//j["player_idx"].get_to(val.player_idx);
			j["position"].get_to(val.position);
			j["speed"].get_to(val.speed);

			Events::make_Event_vector(val.events, j);
		}
	};



} // namespace Trains



//------------------------------ POSTS ------------------------------//

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



//------------------------------ PLAYER ------------------------------//

struct Player
{
	Types::player_uid_t idx;
	std::string name;
	int32_t rating;

	std::map<Types::train_idx_t, Trains::Train> trains;

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



//------------------------------ GAMEDATA ------------------------------//

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
