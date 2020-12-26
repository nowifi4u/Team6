#pragma once

#include "../data.h"
#include "path.h"
#include "../../utils/network/server_connector.h"
#include "../../graph/GraphVertexMap.h"

#include <optional>
#include <algorithm>

class TrainSolver
{
public:

	enum class State
	{
		EMERGENCY_FOOD,
		EMERGENCY_ARMOR,
		NORMAL_FOOD,
		NORMAL_ARMOR,
		RETURN,
		STANDBY
	};

	TrainSolver(const GameData& gamedata, Types::train_idx_t train_idx, GraphVertexMap<double>& deltas_market, GraphVertexMap<double>& deltas_storage, State state = State::STANDBY)
		: gamedata(gamedata),
		pathsolver(gamedata),
		train_idx(train_idx), gamedata_train(gamedata.self_data().trains.at(train_idx)),
		train_data(gamedata.self_data().trains.at(train_idx)),
		deltas_market(deltas_market),
		deltas_storage(deltas_storage),
		state(state)
	{
	}

	GraphIdx::edge_descriptor get_edge() const
	{
		return gamedata.map_graph.emap.at(train_data.line_idx);
	}

	const GraphIdx::EdgeProperties& get_edge_props() const
	{
		return gamedata.graph()[get_edge()];
	}

	GraphIdx::vertex_descriptor choose_target_NORMAL_FOOD() const
	{
		pathsolver.init(get_edge(), train_data.position);

		GraphIdx::vertex_descriptor target = gamedata.graph().null_vertex();
		double target_value = -INFINITY;

		gamedata.map_graph.for_each_vertex_descriptor([&](GraphIdx::vertex_descriptor v) {
			Types::edge_length_t vdist = pathsolver.distance_to(v);
			if (vdist == 0) return;

			if (gamedata.graph()[v].post_idx != UINT32_MAX)
				if (gamedata.posts.at(gamedata.graph()[v].post_idx)->type() == Posts::MARKET)
				{
					const Posts::Market* market = (const Posts::Market*) gamedata.posts.at(gamedata.graph()[v].post_idx);

					double value = std::min<double>({
						(double)Trains::TrainTiers[train_data.level].goods_capacity - train_data.goods,
						std::min<double>(
							(double)market->product_capacity,
							(double)market->product + market->replenishment * vdist - deltas_market[v]
							) / vdist
						});

					if (value > target_value)
					{
						target = v;
						target_value = value;
					}
				}
			});

		deltas_market[target] += target_value;
		return target;
	}

	GraphIdx::vertex_descriptor choose_target_NORMAL_ARMOR() const
	{
		pathsolver.init(get_edge(), train_data.position);

		GraphIdx::vertex_descriptor target = gamedata.graph().null_vertex();
		double target_value = -INFINITY;

		gamedata.map_graph.for_each_vertex_descriptor([&](GraphIdx::vertex_descriptor v) {
			Types::edge_length_t vdist = pathsolver.distance_to(v);
			if (vdist == 0) return;

			if (gamedata.graph()[v].post_idx != UINT32_MAX)
				if (gamedata.posts.at(gamedata.graph()[v].post_idx)->type() == Posts::STORAGE)
				{
					const Posts::Storage* storage = (const Posts::Storage*)gamedata.posts.at(gamedata.graph()[v].post_idx);

					double value = std::min<double>({
						(double)Trains::TrainTiers[train_data.level].goods_capacity - train_data.goods,
						std::min<double>(
							(double)storage->armor_capacity,
							(double)storage->armor + storage->replenishment * vdist - deltas_storage[v]
							) / vdist
						});

					if (value > target_value)
					{
						target = v;
						target_value = value;
					}
				}
			
			});

		deltas_storage[target] += target_value;
		return target;
	}

	GraphIdx::vertex_descriptor choose_target_EMERGENCY_FOOD() const
	{
		pathsolver.init(get_edge(), train_data.position);

		GraphIdx::vertex_descriptor target = gamedata.graph().null_vertex();
		Types::edge_length_t target_dist = UINT32_MAX;

		gamedata.map_graph.for_each_vertex_descriptor([&](GraphIdx::vertex_descriptor v) {
			Types::edge_length_t vdist = pathsolver.distance_to(v);

			if (gamedata.graph()[v].post_idx != UINT32_MAX)
				if (gamedata.posts.at(gamedata.graph()[v].post_idx)->type() == Posts::MARKET)
					if (vdist < target_dist)
					{
						target = v;
						target_dist = vdist;
					}
			});

		return target;
	}

	GraphIdx::vertex_descriptor choose_target_EMERGENCY_ARMOR() const
	{
		pathsolver.init(get_edge(), train_data.position);

		GraphIdx::vertex_descriptor target = gamedata.graph().null_vertex();
		Types::edge_length_t target_dist = UINT32_MAX;

		gamedata.map_graph.for_each_vertex_descriptor([&](GraphIdx::vertex_descriptor v) {
			Types::edge_length_t vdist = pathsolver.distance_to(v);

			if (gamedata.graph()[v].post_idx != UINT32_MAX)
				if (gamedata.posts.at(gamedata.graph()[v].post_idx)->type() == Posts::STORAGE)
					if (vdist < target_dist)
					{
						target = v;
						target_dist = vdist;
					}
			});

		return target;
	}

	GraphIdx::vertex_descriptor choose_target() const
	{
		switch (state)
		{
		case State::NORMAL_FOOD: return choose_target_NORMAL_FOOD();
		case State::NORMAL_ARMOR: return choose_target_NORMAL_ARMOR();
		case State::EMERGENCY_FOOD: return choose_target_EMERGENCY_FOOD();
		case State::EMERGENCY_ARMOR: return choose_target_EMERGENCY_ARMOR();
		case State::RETURN: return gamedata.home_idx;
		case State::STANDBY: return gamedata.graph().null_vertex();
		}
	}


	std::optional<server_connector::Move> calculate_Turn() const
	{
		GraphIdx::vertex_descriptor target = choose_target();

		return pathsolver.calculate_Move(train_data.idx, target);
	}



private:
	const Types::train_idx_t train_idx;
	
	const Trains::Train& train_data;

	GraphVertexMap<double>& deltas_market;
	GraphVertexMap<double>& deltas_storage;

public:
	const GameData& gamedata;
	const Trains::Train& gamedata_train;

	PathSolver pathsolver;

	TrainSolver::State state;
};