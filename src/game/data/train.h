#pragma once

#include "Types.h"
#include "utils/Logging.h"

#include <nlohmann/json.hpp>
#include <boost/ptr_container/ptr_vector.hpp>


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
