#pragma once

#include "../data.h"
#include "graph_edge.h"
#include "../../utils/network/server_connector.h"

#include <optional>

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

	TrainSolver(const GameData& gamedata, GraphEdgeDijkstra& graphsolver, Types::train_idx_t train_idx)
		: gamedata(gamedata),
		graphsolver(graphsolver),
		train_idx(train_idx), gamedata_train(gamedata.players.at(gamedata.player_idx).trains.at(train_idx)),
		train_data(gamedata.players.at(gamedata.player_idx).trains.at(train_idx))
	{
	}

	GraphIdx::edge_descriptor get_edge() const
	{
		return gamedata.map_graph.emap.at(train_data.line_idx);
	}

	const GraphIdx::EdgeProperties& get_edge_props() const
	{
		return gamedata.map_graph.graph[get_edge()];
	}

	GraphIdx::vertex_descriptor choose_target_NORMAL_FOOD() const
	{
		graphsolver.calculate(get_edge(), train_data.position);
	}

	GraphIdx::vertex_descriptor choose_target_NORMAL_ARMOR() const
	{
		graphsolver.calculate(get_edge(), train_data.position);
	}

	GraphIdx::vertex_descriptor choose_target_EMERGENCY_FOOD() const
	{
		graphsolver.calculate(get_edge(), train_data.position);
	}

	GraphIdx::vertex_descriptor choose_target_EMERGENCY_ARMOR() const
	{
		graphsolver.calculate(get_edge(), train_data.position);
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
		case State::STANDBY: return gamedata.map_graph.graph.null_vertex();
		}
	}


	std::optional<game_connector::Move> calculate_Turn() const
	{
		GraphIdx::vertex_descriptor target = choose_target();

		if (target == gamedata.map_graph.graph.null_vertex()) return std::nullopt;

		const auto solver = graphsolver.get_obj(target);
		GraphDijkstra::path_t path = graphsolver.get_path(target);

		if (path.size() == 0) return std::nullopt;

		if (solver.second == false)
		{
			GraphIdx::vertex_descriptor v = boost::source(get_edge(), gamedata.map_graph.graph);

			if (train_data.position == 0)
			{
				if (path.size() == 0) return std::nullopt;
				
				if (Graph::isSource(gamedata.map_graph.graph, v, path.front()))
				{
					return game_connector::Move{ Graph::get_edge(gamedata.map_graph.graph, v, path.front())->idx, 1, train_idx };
				}
				else
				{
					return game_connector::Move{ Graph::get_edge(gamedata.map_graph.graph, v, path.front())->idx, -1, train_idx };
				}
			}
			else
			{
				return game_connector::Move{ get_edge_props().idx, -1, train_idx };
			}
		}
		else
		{
			GraphIdx::vertex_descriptor v = boost::target(get_edge(), gamedata.map_graph.graph);


			if (train_data.position == gamedata.map_graph.graph[get_edge()].length)
			{
				if (path.size() == 0) return std::nullopt;

				if (Graph::isSource(gamedata.map_graph.graph, v, path.front()))
				{
					return game_connector::Move{ Graph::get_edge(gamedata.map_graph.graph, v, path.front())->idx, 1, train_idx };
				}
				else
				{
					return game_connector::Move{ Graph::get_edge(gamedata.map_graph.graph, v, path.front())->idx, -1, train_idx };
				}
			}
			else
			{
				return game_connector::Move{ get_edge_props().idx, 1, train_idx };
			}
		}
	}



private:
	const Types::train_idx_t train_idx;
	
	const Trains::Train& train_data;

public:
	const GameData& gamedata;
	const Trains::Train& gamedata_train;

	GraphEdgeDijkstra& graphsolver;

	TrainSolver::State state;
};