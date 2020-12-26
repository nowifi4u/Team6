#pragma once

#include "solver/train.h"
#include "../utils/network/server_connector.h"


class GameSolver
{
public:

	GameSolver(const GameData& gamedata, server_connector& connector)
		: gamedata(gamedata), connector(connector), tick(0)
	{
		for (const auto& [train_idx, train_data] : gamedata.self_data().trains)
		{
			trainsolvers.emplace_back(gamedata, connector, train_idx, deltas_market, deltas_storage);
		}
	}

	void reset_deltas()
	{
		gamedata.map_graph.for_each_vertex_descriptor([&](GraphIdx::vertex_descriptor v) {
			deltas_market[v] = 0;
			deltas_storage[v] = 0;
			});
	}

	void calculate()
	{
		reset_deltas();

		for (auto& train_solver : trainsolvers)
		{
			std::optional<server_connector::Move> move = train_solver.calculate_Turn();

			if (move.has_value()) connector.async_send_Move(move.value());
		}
	}


protected:
	const GameData& gamedata;
	server_connector& connector;

	std::vector<TrainSolver> trainsolvers;

	GraphVertexMap<double> deltas_market;
	GraphVertexMap<double> deltas_storage;
	
	Types::tick_t tick;
};
