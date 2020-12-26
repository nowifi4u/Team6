#pragma once

#include "../data.h"
#include "graph_edge.h"
#include "../../utils/network/server_connector.h"

#include <optional>

class PathSolver
{
public:

	PathSolver(const GameData& gamedata)
		: gamedata(gamedata), 
		graphsolver(gamedata.graph())
	{
	}

	void init(GraphIdx::edge_descriptor epos, Types::edge_length_t pos)
	{
		graphsolver.calculate(epos, pos);
	}

	void init(Types::train_idx_t train_idx)
	{
		const Trains::Train& train_data = gamedata.self_data().trains.at(train_idx);
		GraphIdx::edge_descriptor epos = gamedata.map_graph.emap.at(train_data.line_idx);
		Types::edge_length_t pos = train_data.position;

		init(epos, pos);
	}

	Types::edge_length_t distance_to(GraphIdx::vertex_descriptor target) const
	{
		return graphsolver[target].first;
	}

	std::optional<server_connector::Move> calculate_Move(Types::train_idx_t train_idx, GraphIdx::vertex_descriptor target)
	{
		if (target == gamedata.graph().null_vertex()) return std::nullopt;

		bool solver_is_source = graphsolver.get_is_source(target);
		GraphDijkstra::path_edges_t solver_path_edges = graphsolver.get_path_edges(target);
		GraphDijkstra::path_t solver_path = graphsolver.get_path(target);

		const Trains::Train& train_data = gamedata.self_data().trains.at(train_idx);
		GraphIdx::edge_descriptor epos = gamedata.map_graph.emap.at(train_data.line_idx);
		Types::edge_length_t pos = train_data.position;

		if (solver_is_source == false)
		{
			GraphIdx::vertex_descriptor v = boost::source(epos, gamedata.graph());

			if (train_data.position == 0)
			{
				if (solver_path.size() == 0) return std::nullopt;

				if (Graph::isSource(gamedata.graph(), v, solver_path.front()))
				{
					return server_connector::Move{ gamedata.graph()[solver_path_edges.front()].idx, 1, train_idx };
				}
				else
				{
					return server_connector::Move{ gamedata.graph()[solver_path_edges.front()].idx, -1, train_idx };
				}
			}
			else
			{
				return server_connector::Move{ gamedata.graph()[epos].idx, -1, train_idx };
			}
		}
		else
		{
			GraphIdx::vertex_descriptor v = boost::target(epos, gamedata.graph());


			if (train_data.position == gamedata.graph()[epos].length)
			{
				if (solver_path.size() == 0) return std::nullopt;

				if (Graph::isSource(gamedata.graph(), v, solver_path.front()))
				{
					return server_connector::Move{ gamedata.graph()[solver_path_edges.front()].idx, 1, train_idx };
				}
				else
				{
					return server_connector::Move{ gamedata.graph()[solver_path_edges.front()].idx, -1, train_idx };
				}
			}
			else
			{
				return server_connector::Move{ gamedata.graph()[epos].idx, 1, train_idx };
			}
		}
	}

	bool is_train_nearby()
	{

	}

protected:

	const GameData& gamedata;
	GraphEdgeDijkstra graphsolver;
};