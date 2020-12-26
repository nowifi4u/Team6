#pragma once

#include "../data.h"
#include "graph_edge.h"
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

	TrainSolver(const GameData& gamedata, GraphEdgeDijkstra& graphsolver, Types::train_idx_t train_idx, GraphVertexMap<double>& deltas_market, GraphVertexMap<double>& deltas_storage, State state = State::STANDBY)
		: gamedata(gamedata),
		graphsolver(graphsolver),
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
		graphsolver.calculate(get_edge(), train_data.position);

		GraphIdx::vertex_descriptor target = gamedata.graph().null_vertex();
		double target_value = -INFINITY;

		gamedata.map_graph.for_each_vertex_descriptor([&](GraphIdx::vertex_descriptor v) {
			if (gamedata.graph()[v].post_idx != UINT32_MAX)
				if (gamedata.posts.at(gamedata.graph()[v].post_idx)->type() == Posts::MARKET)
				{
					const Posts::Market* market = (const Posts::Market*) gamedata.posts.at(gamedata.graph()[v].post_idx);

					double value = std::min<double>({
						(double)Trains::TrainTiers[train_data.level].goods_capacity - train_data.goods,
						(double)market->product_capacity,
						(double)market->product + market->replenishment * graphsolver[v].first - deltas_market[v]
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
		graphsolver.calculate(get_edge(), train_data.position);

		GraphIdx::vertex_descriptor target = gamedata.graph().null_vertex();
		double target_value = -INFINITY;

		gamedata.map_graph.for_each_vertex_descriptor([&](GraphIdx::vertex_descriptor v) {
			if (gamedata.graph()[v].post_idx != UINT32_MAX)
				if (gamedata.posts.at(gamedata.graph()[v].post_idx)->type() == Posts::STORAGE)
				{
					const Posts::Storage* storage = (const Posts::Storage*)gamedata.posts.at(gamedata.graph()[v].post_idx);

					double value = std::min<double>({ 
						(double)Trains::TrainTiers[train_data.level].goods_capacity - train_data.goods,
						(double)storage->armor_capacity,
						(double)storage->armor + storage->replenishment * graphsolver[v].first - deltas_storage[v]
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
		graphsolver.calculate(get_edge(), train_data.position);

		GraphIdx::vertex_descriptor target = gamedata.graph().null_vertex();
		Types::edge_length_t target_dist = UINT32_MAX;

		gamedata.map_graph.for_each_vertex_descriptor([&](GraphIdx::vertex_descriptor v) {
			if (gamedata.graph()[v].post_idx != UINT32_MAX)
				if (gamedata.posts.at(gamedata.graph()[v].post_idx)->type() == Posts::MARKET)
					if (graphsolver[v].first < target_dist)
					{
						target = v;
						target_dist = graphsolver[v].first;
					}
			});

		return target;
	}

	GraphIdx::vertex_descriptor choose_target_EMERGENCY_ARMOR() const
	{
		graphsolver.calculate(get_edge(), train_data.position);

		GraphIdx::vertex_descriptor target = gamedata.graph().null_vertex();
		Types::edge_length_t target_dist = UINT32_MAX;

		gamedata.map_graph.for_each_vertex_descriptor([&](GraphIdx::vertex_descriptor v) {
			if (gamedata.graph()[v].post_idx != UINT32_MAX)
				if (gamedata.posts.at(gamedata.graph()[v].post_idx)->type() == Posts::STORAGE)
					if (graphsolver[v].first < target_dist)
					{
						target = v;
						target_dist = graphsolver[v].first;
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

		if (target == gamedata.graph().null_vertex()) return std::nullopt;

		const auto solver = graphsolver.get_obj(target);
		GraphDijkstra::path_t path = graphsolver.get_path(target);

		if (path.size() == 0) return std::nullopt;

		if (solver.second == false)
		{
			GraphIdx::vertex_descriptor v = boost::source(get_edge(), gamedata.graph());

			if (train_data.position == 0)
			{
				if (path.size() == 0) return std::nullopt;
				
				if (Graph::isSource(gamedata.graph(), v, path.front()))
				{
					return server_connector::Move{ Graph::get_edge(gamedata.graph(), v, path.front())->idx, 1, train_idx };
				}
				else
				{
					return server_connector::Move{ Graph::get_edge(gamedata.graph(), v, path.front())->idx, -1, train_idx };
				}
			}
			else
			{
				return server_connector::Move{ get_edge_props().idx, -1, train_idx };
			}
		}
		else
		{
			GraphIdx::vertex_descriptor v = boost::target(get_edge(), gamedata.graph());


			if (train_data.position == gamedata.graph()[get_edge()].length)
			{
				if (path.size() == 0) return std::nullopt;

				if (Graph::isSource(gamedata.graph(), v, path.front()))
				{
					return server_connector::Move{ Graph::get_edge(gamedata.graph(), v, path.front())->idx, 1, train_idx };
				}
				else
				{
					return server_connector::Move{ Graph::get_edge(gamedata.graph(), v, path.front())->idx, -1, train_idx };
				}
			}
			else
			{
				return server_connector::Move{ get_edge_props().idx, 1, train_idx };
			}
		}
	}



private:
	const Types::train_idx_t train_idx;
	
	const Trains::Train& train_data;

	GraphVertexMap<double>& deltas_market;
	GraphVertexMap<double>& deltas_storage;

public:
	const GameData& gamedata;
	const Trains::Train& gamedata_train;

	GraphEdgeDijkstra& graphsolver;

	TrainSolver::State state;
};