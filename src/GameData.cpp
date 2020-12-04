#include "GameData.h"


//------------------------------ EVENTS ------------------------------//

namespace Events {



	Event* Event::readJSON_L1(const ptree& pt)
	{
		EventType event_type = (EventType)pt.get<uint8_t>("type");

		switch (event_type)
		{
		case EventType::TRAIN_COLLISION: return dynamic_cast<Event*>(Event_TrainCrash::readJSON_L1(pt));
		case EventType::PARASITES_ASSAULT: return dynamic_cast<Event*>(Event_Parasites::readJSON_L1(pt));
		case EventType::HIJACKERS_ASSAULT: return dynamic_cast<Event*>(Event_Bandits::readJSON_L1(pt));
		case EventType::REFUGEES_ARRIVAL: return dynamic_cast<Event*>(Event_Refugees::readJSON_L1(pt));
		case EventType::RESOURCE_OVERFLOW: return dynamic_cast<Event*>(Event_ResourceOverflow::readJSON_L1(pt));
		case EventType::RESOURCE_LACK: return dynamic_cast<Event*>(Event_ResourceLack::readJSON_L1(pt));
		case EventType::GAME_OVER: return dynamic_cast<Event*>(Event_GameOver::readJSON_L1(pt));
		default: return nullptr;
		}
	}

	void Event::readJSON_L1_vector(boost::ptr_vector<Event>& vec, const ptree& pt)
	{
		vec.clear();
		vec.reserve(ptree_array_size(pt, "events"));
		ptree_array_foreach(pt, "events", [&](const ptree& item) {
			vec.push_back(Event::readJSON_L1(pt));
			});
	}


	Event_TrainCrash* Event_TrainCrash::readJSON_L1(const ptree& pt)
	{
		Event_TrainCrash* event = new Event_TrainCrash;

		event->train = pt.get<BOOST_TYPEOF(Event_TrainCrash::train)>("train");
		event->tick = pt.get<BOOST_TYPEOF(Event_TrainCrash::tick)>("tick");

		return event;
	}

	Event_Parasites* Event_Parasites::readJSON_L1(const ptree& pt)
	{
		Event_Parasites* event = new Event_Parasites;

		event->parasite_power = pt.get<BOOST_TYPEOF(Event_Parasites::parasite_power)>("parasites_power");
		event->tick = pt.get<BOOST_TYPEOF(Event_Parasites::tick)>("tick");

		return event;
	}

	Event_Bandits* Event_Bandits::readJSON_L1(const ptree& pt)
	{
		Event_Bandits* event = new Event_Bandits;

		event->hijacker_power = pt.get<BOOST_TYPEOF(Event_Bandits::hijacker_power)>("hijackers_power");
		event->tick = pt.get<BOOST_TYPEOF(Event_Bandits::tick)>("tick");

		return event;
	}

	Event_Refugees* Event_Refugees::readJSON_L1(const ptree& pt)
	{
		Event_Refugees* event = new Event_Refugees;

		event->refugees_number = pt.get<BOOST_TYPEOF(Event_Refugees::refugees_number)>("refugees_number");
		event->tick = pt.get<BOOST_TYPEOF(Event_Refugees::tick)>("tick");

		return event;
	}

	Event_ResourceOverflow* Event_ResourceOverflow::readJSON_L1(const ptree& pt)
	{
		Event_ResourceOverflow* event = new Event_ResourceOverflow;

		return event;
	}

	Event_ResourceLack* Event_ResourceLack::readJSON_L1(const ptree& pt)
	{
		Event_ResourceLack* event = new Event_ResourceLack;

		return event;
	}

	Event_GameOver* Event_GameOver::readJSON_L1(const ptree& pt)
	{
		Event_GameOver* event = new Event_GameOver;

		return event;
	}



} // namespace Events



//------------------------------ TRAINS ------------------------------//

namespace Trains {



	void Train::readJSON_L1(Train& val, const ptree& pt)
	{
		val.idx = pt.get<BOOST_TYPEOF(Train::idx)>("idx");
		val.tier = pt.get<BOOST_TYPEOF(Train::tier)>("tier");
		val.cooldown = pt.get<BOOST_TYPEOF(Train::cooldown)>("cooldown");
		val.fuel = pt.get<BOOST_TYPEOF(Train::fuel)>("fuel");
		val.goods = pt.get<BOOST_TYPEOF(Train::goods)>("goods");
		val.line_idx = pt.get<BOOST_TYPEOF(Train::line_idx)>("line_idx");
		val.player_idx = pt.get<BOOST_TYPEOF(Train::player_idx)>("player_idx");
		val.position = pt.get<BOOST_TYPEOF(Train::position)>("position");
		val.speed = pt.get<BOOST_TYPEOF(Train::speed)>("speed");

		Events::Event::readJSON_L1_vector(val.events, pt);
	}

	void Train::updateJSON(Train& val, const ptree& pt)
	{
		//val.idx = pt.get<BOOST_TYPEOF(Train::idx)>("idx");
		val.tier = pt.get<BOOST_TYPEOF(Train::tier)>("tier");
		val.cooldown = pt.get<BOOST_TYPEOF(Train::cooldown)>("cooldown");
		val.fuel = pt.get<BOOST_TYPEOF(Train::fuel)>("fuel");
		val.goods = pt.get<BOOST_TYPEOF(Train::goods)>("goods");
		val.line_idx = pt.get<BOOST_TYPEOF(Train::line_idx)>("line_idx");
		//val.player_idx = pt.get<BOOST_TYPEOF(Train::player_idx)>("player_idx");
		val.position = pt.get<BOOST_TYPEOF(Train::position)>("position");
		val.speed = pt.get<BOOST_TYPEOF(Train::speed)>("speed");

		Events::Event::readJSON_L1_vector(val.events, pt);
	}



} // namespace Trains



//------------------------------ POSTS ------------------------------//

namespace Posts {



	Post* Post::readJSON_L1(const ptree& pt)
	{
		PostType post_type = (PostType)pt.get<uint8_t>("type");

		Post* ptr;

		switch (post_type)
		{
		case PostType::TOWN: ptr = Town::readJSON_L1(pt);
		case PostType::MARKET: ptr = Market::readJSON_L1(pt);
		case PostType::STORAGE: ptr = Storage::readJSON_L1(pt);
		default: return nullptr;
		}

		ptr->idx = pt.get<BOOST_TYPEOF(Post::idx)>("idx");
		ptr->name = pt.get<BOOST_TYPEOF(Post::name)>("name");
		ptr->point_idx = pt.get_optional<BOOST_TYPEOF(Post::point_idx)>("post_idx").get_value_or(UINT32_MAX);

		Events::Event::readJSON_L1_vector(ptr->events, pt);
	}

	void Post::updateJSON(Post* ptr, const ptree& pt)
	{
		PostType post_type = ptr->type();

		switch (post_type)
		{
		case PostType::TOWN: return Town::updateJSON(dynamic_cast<Town*>(ptr), pt);
		case PostType::MARKET: return Market::updateJSON(dynamic_cast<Market*>(ptr), pt);
		case PostType::STORAGE: return Post::updateJSON(dynamic_cast<Post*>(ptr), pt);
		}

		Events::Event::readJSON_L1_vector(ptr->events, pt);
	}

	Storage* Storage::readJSON_L1(const ptree& pt)
	{
		Storage* ptr = new Storage;

		ptr->armor = pt.get<BOOST_TYPEOF(Storage::armor)>("armor");
		ptr->armor_capacity = pt.get<BOOST_TYPEOF(Storage::armor_capacity)>("armor_capacity");
		ptr->replenishment = pt.get<BOOST_TYPEOF(Storage::replenishment)>("replenishment");

		return ptr;
	}

	void Storage::updateJSON(Storage* ptr, const ptree& pt)
	{
		ptr->armor = pt.get<BOOST_TYPEOF(Storage::armor)>("armor");
		//ptr->armor_capacity = pt.get<BOOST_TYPEOF(Storage::armor_capacity)>("armor_capacity");
		//ptr->replenishment = pt.get<BOOST_TYPEOF(Storage::replenishment)>("replenishment");
	}

	Market* Market::readJSON_L1(const ptree& pt)
	{
		Market* ptr = new Market;

		ptr->product = pt.get<BOOST_TYPEOF(Market::product)>("product");
		ptr->product_capacity = pt.get<BOOST_TYPEOF(Market::product_capacity)>("product_capacity");
		ptr->replenishment = pt.get<BOOST_TYPEOF(Market::replenishment)>("replenishment");

		return ptr;
	}

	void Market::updateJSON(Market* ptr, const ptree& pt)
	{
		ptr->product = pt.get<BOOST_TYPEOF(Market::product)>("product");
		//ptr->product_capacity = pt.get<BOOST_TYPEOF(Market::product_capacity)>("product_capacity");
		//ptr->replenishment = pt.get<BOOST_TYPEOF(Market::replenishment)>("replenishment");
	}

	Town* Town::readJSON_L1(const ptree& pt)
	{
		Town* ptr = new Town;

		ptr->armor = pt.get<BOOST_TYPEOF(Town::armor)>("armor");
		ptr->level = pt.get<BOOST_TYPEOF(Town::level)>("level");
		ptr->player_idx = pt.get<BOOST_TYPEOF(Town::player_idx)>("player_idx");
		ptr->population = pt.get<BOOST_TYPEOF(Town::population)>("population");
		ptr->product = pt.get<BOOST_TYPEOF(Town::product)>("product");
		ptr->train_cooldown = pt.get<BOOST_TYPEOF(Town::train_cooldown)>("train_cooldown");

		return ptr;
	}

	void Town::updateJSON(Town* ptr, const ptree& pt)
	{
		ptr->armor = pt.get<BOOST_TYPEOF(Town::armor)>("armor");
		ptr->level = pt.get<BOOST_TYPEOF(Town::level)>("level");
		//ptr->player_idx = pt.get<BOOST_TYPEOF(Town::player_idx)>("player_idx");
		ptr->population = pt.get<BOOST_TYPEOF(Town::population)>("population");
		ptr->product = pt.get<BOOST_TYPEOF(Town::product)>("product");
		ptr->train_cooldown = pt.get<BOOST_TYPEOF(Town::train_cooldown)>("train_cooldown");
	}



} // namespace Posts



//------------------------------ PLAYER ------------------------------//

void Player::readJSON_L1(Player& val, const ptree& pt)
{
	val.idx = pt.get<BOOST_TYPEOF(Player::idx)>("idx");
	val.name = pt.get<BOOST_TYPEOF(Player::name)>("name");
	val.rating = pt.get<BOOST_TYPEOF(Player::rating)>("rating");
}

void Player::updateJSON(Player& val, const ptree& pt)
{
	//val.idx = pt.get<BOOST_TYPEOF(Player::idx)>("idx");
	//val.name = pt.get<BOOST_TYPEOF(Player::name)>("name");
	val.rating = pt.get<BOOST_TYPEOF(Player::rating)>("rating");
}



//------------------------------ GAMEDATA ------------------------------//

void GameData::readJSON_Login(GameData& val, const ptree& pt)
{
	val.player_idx = pt.get<BOOST_TYPEOF(GameData::player_idx)>("idx");
	val.home_idx = pt.get_child("home").get<BOOST_TYPEOF(GameData::home_idx)>("idx");
	val.post_idx = pt.get_child("home").get<BOOST_TYPEOF(GameData::post_idx)>("post_idx");
	val.in_game = pt.get<BOOST_TYPEOF(GameData::in_game)>("in_game");
}

void GameData::readJSON_L0(GameData& val, const ptree& pt)
{
	GraphIdx::readJSON_L0(val.graph, pt);
}

void GameData::readJSON_L10(GameData& val, const ptree& pt)
{
	GraphIdx::readJSON_L10(val.graph, pt);
}

void GameData::readJSON_L1(GameData& val, const ptree& pt)
{
	for (const auto& player : pt.get_child("ratings"))
	{
		//Allocate Player for player_idx
		val.players[player.first];

		Player::readJSON_L1(val.players[player.first], pt);
	}

	/*ptree_array_foreach(pt, "posts", [&](const ptree& ipt) {
		Types::train_idx_t train_idx = ipt.get_value<Types::train_idx_t>("idx");
		Types::player_uid_t player_idx = ipt.get_value<Types::player_uid_t>("player_idx");
		val.players[player_idx].trains.insert({});
		Train::readJSON_L1(val.players[player_idx].trains.rbegin().operator*(), ipt);
		});*/
}

void GameData::updateJSON(GameData& val, const ptree& pt)
{
	for (const auto& player : pt.get_child("ratings"))
	{
		Player::updateJSON(val.players[player.first], pt);
	}
}