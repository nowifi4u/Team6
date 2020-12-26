#pragma once

#include "../data.h"
#include "graph_edge.h"
#include "../../utils/network/server_connector.h"

#include <optional>

class PathSolver
{
public:

	PathSolver(const GameData& gamedata, GraphEdgeDijkstra& graphsolver)
		: gamedata(gamedata), graphsolver(graphsolver)
	{

	}

	void init(GraphIdx::edge_descriptor e, Types::edge_idx_t pos)
	{
		graphsolver.calculate(e, pos);
	}

	Types::edge_length_t distance_to(GraphIdx::vertex_descriptor target)
	{
		return graphsolver[target].first;
	}

	std::optional<server_connector::Move> calculate_Move(GraphIdx::edge_descriptor epos, Types::edge_idx_t pos, Types::train_idx_t train_idx, GraphIdx::vertex_descriptor target)
	{
		if (target == gamedata.graph().null_vertex()) return std::nullopt;

		const auto solver = graphsolver.get_obj(target);
		GraphDijkstra::path_t path = graphsolver.get_path(target);

		const Trains::Train& train_data = gamedata.self_data().trains.at(train_idx);

		if (solver.second == false)
		{
			GraphIdx::vertex_descriptor v = boost::source(epos, gamedata.graph());

			if (train_data.position == 0)
			{
				if (path.size() == 0) return std::nullopt;

				if (Graph::isSource(gamedata.graph(), v, path.front()))
				{
					return server_connector::Move{ Graph::get_edge_props(gamedata.graph(), v, path.front())->idx, 1, train_idx };
				}
				else
				{
					return server_connector::Move{ Graph::get_edge_props(gamedata.graph(), v, path.front())->idx, -1, train_idx };
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
				if (path.size() == 0) return std::nullopt;

				if (Graph::isSource(gamedata.graph(), v, path.front()))
				{
					return server_connector::Move{ Graph::get_edge_props(gamedata.graph(), v, path.front())->idx, 1, train_idx };
				}
				else
				{
					return server_connector::Move{ Graph::get_edge_props(gamedata.graph(), v, path.front())->idx, -1, train_idx };
				}
			}
			else
			{
				return server_connector::Move{ gamedata.graph()[epos].idx, 1, train_idx };
			}
		}
	}

protected:

	const GameData& gamedata;
	GraphEdgeDijkstra& graphsolver;
};