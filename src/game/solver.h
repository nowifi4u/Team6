#pragma once

#include "solver/train.h"
#include "../utils/network/server_connector.h"


class GameSolver
{
public:

	GameSolver(const GameData& gamedata, server_connector& connector)
		: gamedata(gamedata), 
		connector(connector), 
		pathsolver(gamedata),
		tick(0)
	{
		for (const auto& [train_idx, train_data] : gamedata.self_data().trains)
		{
			trainsolvers.emplace_back(gamedata, train_idx, deltas_market, deltas_storage);
		}
	}

	void reset_deltas()
	{
		Graph::for_each_vertex_descriptor(gamedata.graph(), [&](Graph::vertex_descriptor v) {
			deltas_market[v] = 0;
			deltas_storage[v] = 0;
			});
	}

	void calculate()
	{
		tick++;

		reset_deltas();

		calculate_states();

		for (auto& train_solver : trainsolvers)
		{


			std::optional<server_connector::Move> move = train_solver.calculate_Turn();

			if (move.has_value()) connector.async_send_Move(move.value());
		}
	}

	void calculate_states() {
		Types::Epoch epoch = get_epoch();
		if (epoch <= 3)  // 0,1,2,3
		{
			for (TrainSolver& ts : trainsolvers) {
				if (ts.gamedata_train.goods_type == Trains::Armor && ts.gamedata_train.goods > 35) {
					ts.state = TrainSolver::State::RETURN;
				}
				ts.state = TrainSolver::State::NORMAL_ARMOR;
			}
		}
		else if (epoch == 4) { //4 epoch: 

			check_first_free();
			if (food_epoch4_ts_idx != std::numeric_limits<uint32_t>::max()) {

				TrainSolver& ts = trainsolvers[food_epoch4_ts_idx];						   //1 train collexts food

				if (ts.gamedata_train.goods_type == Trains::Product && ts.gamedata_train.goods > 0) {
					ts.state = TrainSolver::State::RETURN;
				}

				ts.state = TrainSolver::State::NORMAL_FOOD;
			}

			for (size_t i = 0; i < trainsolvers.size(); ++i) {								//3 other - armor
				if (i != food_epoch4_ts_idx)
				{
					TrainSolver& ts = trainsolvers[i];
					if (ts.gamedata_train.goods_type == Trains::Armor && ts.gamedata_train.goods > 35)
					{
						ts.state = TrainSolver::State::RETURN;
					}
					ts.state = TrainSolver::State::NORMAL_FOOD;
				}
			}

		}
		else { //epoch 5,6,7 (partly 8) - collect food 
			for (TrainSolver& ts : trainsolvers) {
				if (ts.gamedata_train.goods != 0)		//train has already visited market for food
					ts.state = TrainSolver::State::RETURN;
				else
					ts.state = TrainSolver::State::NORMAL_FOOD;
			}
		}
	}

	Types::Epoch get_epoch() {
		return tick / 60;
	}

	size_t getTrainSolverIndex(Types::train_idx_t t) {
		for (size_t i = 0; i < trainsolvers.size(); ++i) {
			if (trainsolvers[i].gamedata_train.idx == t)
				return i;
		}
		return std::numeric_limits<uint32_t>::max();
	}

	void check_first_free() {
		if (food_epoch4_ts_idx == std::numeric_limits<uint32_t>::max())
		{
			for (size_t i = 0; i < trainsolvers.size(); ++i) {
				if (is_at_home(trainsolvers[i])) {
					food_epoch4_ts_idx = i;
					return;
				}
			}
		}
	}

	bool is_at_home(TrainSolver& ts) {
		auto v_idx = gamedata.home_idx;
		auto v = gamedata.map_graph.vmap.at(v_idx);
		for (auto& p : gamedata.map_graph.emap) {
			if (ts.gamedata_train.line_idx == p.first) {
				auto e = p.second;
				auto u = boost::source(e, gamedata.map_graph.graph);
				auto t = boost::target(e, gamedata.map_graph.graph);
				if (
					(v == u && ts.gamedata_train.position == 0)||
					(v == t && ts.gamedata_train.position == (gamedata.map_graph.graph[e].length - 1))
					)
				{
					return true;
				}
			}
		}
		return false;
	}


protected:
	const GameData& gamedata;
	server_connector& connector;

	std::vector<TrainSolver> trainsolvers;

	GraphVertexMap<double> deltas_market;
	GraphVertexMap<double> deltas_storage;

	PathSolver pathsolver;
	
	Types::tick_t tick;
	size_t food_epoch4_ts_idx;
};
