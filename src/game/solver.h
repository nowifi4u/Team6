#pragma once

#include "solver/train.h"
#include "../utils/network/server_connector.h"


class GameSolver
{
public:

	GameSolver(const GameData& gamedata, server_connector& connector)
		: gamedata(gamedata), connector(connector)
	{
		for (const auto& [train_idx, train_data] : gamedata.self_data().trains)
		{
			trainsolvers.emplace(train_idx, gamedata, connector, train_idx, deltas_market, deltas_storage);
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

		for (auto& [train_idx, train_solver] : trainsolvers)
		{
			std::optional<server_connector::Move> move = train_solver.calculate_Turn();

			if (move.has_value()) connector.send_Move(move.value());
		}
	}


protected:
	const GameData& gamedata;
	server_connector& connector;

	std::map<Types::train_idx_t, TrainSolver> trainsolvers;

	GraphVertexMap<double> deltas_market;
	GraphVertexMap<double> deltas_storage;
};
