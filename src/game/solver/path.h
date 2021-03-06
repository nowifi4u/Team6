#pragma once

#include "../data.h"
#include "graph_edge.h"
#include "../../utils/network/server_connector.h"

#include <optional>
#include <cmath>

class PathSolver
{
public:

	PathSolver(const GameData& gamedata)
		: gamedata(gamedata),
		graphsolver(gamedata.graph(), exclude_edges)
	{
	}

	void init(Graph::edge_descriptor epos, Types::edge_length_t pos)
	{
		graphsolver.calculate(epos, pos);
	}

	void init(Types::train_idx_t train_idx)
	{
		const Trains::Train& train_data = gamedata.self_data().trains.at(train_idx);
		Graph::edge_descriptor epos = gamedata.map_graph.emap.at(train_data.line_idx);
		Types::edge_length_t pos = train_data.position;

		init(epos, pos);
	}

	Types::edge_length_t distance_to(Graph::vertex_descriptor target) const
	{
		return graphsolver.get_distance(target);
	}

	std::optional<std::tuple<GraphDijkstra::path_t, GraphDijkstra::path_edges_t, server_connector::Move, Graph::vertex_descriptor>> 
		calculate_Move(Types::train_idx_t train_idx, Graph::vertex_descriptor target) const
	{
		if (target == gamedata.graph().null_vertex()) return std::nullopt;

		
		bool solver_is_source = graphsolver.get_is_source(target);
		GraphDijkstra::path_edges_t solver_path_edges = graphsolver.get_path_edges(target);
		GraphDijkstra::path_t solver_path = graphsolver.get_path(target);

		const Trains::Train& train_data = gamedata.self_data().trains.at(train_idx);
		Graph::edge_descriptor epos = gamedata.map_graph.emap.at(train_data.line_idx);
		Types::edge_length_t pos = train_data.position;

		if (solver_is_source == false)
		{
			Graph::vertex_descriptor v = boost::source(epos, gamedata.graph());

			if (train_data.position == 0)
			{
				if (solver_path.size() == 0) return std::nullopt;

				if (Graph::isSource(gamedata.graph(), v, solver_path.front()))
				{
					return std::make_tuple(solver_path, solver_path_edges, server_connector::Move{ gamedata.graph()[solver_path_edges.front()].idx, 1, train_idx }, target);
				}
				else
				{
					return std::make_tuple(solver_path, solver_path_edges, server_connector::Move{ gamedata.graph()[solver_path_edges.front()].idx, -1, train_idx }, target);
				}
			}
			else
			{
				return std::make_tuple(solver_path, solver_path_edges, server_connector::Move{ gamedata.graph()[epos].idx, -1, train_idx }, target);
			}
		}
		else
		{
			Graph::vertex_descriptor v = boost::target(epos, gamedata.graph());


			if (train_data.position == gamedata.graph()[epos].length)
			{
				if (solver_path.size() == 0) return std::nullopt;

				if (Graph::isSource(gamedata.graph(), v, solver_path.front()))
				{
					return std::make_tuple(solver_path, solver_path_edges, server_connector::Move{ gamedata.graph()[solver_path_edges.front()].idx, 1, train_idx }, target);
				}
				else
				{
					return std::make_tuple(solver_path, solver_path_edges, server_connector::Move{ gamedata.graph()[solver_path_edges.front()].idx, -1, train_idx }, target);
				}
			}
			else
			{
				return std::make_tuple(solver_path, solver_path_edges, server_connector::Move{ gamedata.graph()[epos].idx, 1, train_idx }, target);
			}
		}
	}

	/*bool is_train_nearby(Types::train_idx_t train_idx, Types::edge_length_t dist) const
	{
		const Trains::Train& train_data = gamedata.self_data().trains.at(train_idx);
		const Graph::EdgeProperties& train_edge = gamedata.map_graph.get_edge(train_data.line_idx);

		if (train_data.position <= dist)
		{
			for (const auto& [itrain, itrain_data] : gamedata.trains)
			{
			}
		}
	}*/

	bool is_train_at_vertex(Types::train_idx_t train_idx, Graph::vertex_descriptor vertex) const
	{
		const Trains::Train& train_data = gamedata.self_data().trains.at(train_idx);
		const Types::position_t train_pos = train_data.position;

		if (train_pos == 0)
		{
			return Graph::any_of_out_edge(gamedata.graph(), vertex, [&](Graph::edge_descriptor edge) {
				return true;
				});
		}
		else
		{
			return Graph::any_of_in_edge(gamedata.graph(), vertex, [&](Graph::edge_descriptor edge) {
				return (train_pos == gamedata.graph()[edge].length);
				});
		}
	}

protected:

	const GameData& gamedata;

public:
	GraphDijkstra::weightmap_transform_t exclude_edges;
	GraphEdgeDijkstra graphsolver;
};
