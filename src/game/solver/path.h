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

	std::optional<server_connector::Move> calculate_Move(GraphIdx::vertex_descriptor target)
	{
		auto solver = graphsolver.get_obj(target);
		auto path = graphsolver.get_path(target);
	}

public:

	const GameData& gamedata;
	GraphEdgeDijkstra& graphsolver;


protected:


};