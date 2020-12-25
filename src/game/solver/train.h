#pragma once

#include "../data.h"
#include "graph.h"

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

	TrainSolver(const GameData& gamedata, GraphDijkstra& graphsolver, Types::train_idx_t train_idx)
		: gamedata(gamedata), 
		graphsolver(graphsolver),
		train_idx(train_idx), gamedata_train(gamedata.players.at(gamedata.player_idx).trains.at(train_idx)) {}

	std::pair<GraphIdx::vertex_descriptor, Types::edge_length_t> get_edge_source() const
	{

	}

	GraphIdx::vertex_descriptor choose_target_NORMAL_FOOD() const
	{
		
	}

	GraphIdx::vertex_descriptor choose_target_NORMAL_ARMOR() const
	{
		
	}

	GraphIdx::vertex_descriptor choose_target_EMERGENCY_FOOD() const
	{
		
	}

	GraphIdx::vertex_descriptor choose_target_EMERGENCY_ARMOR() const
	{
		
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
		case State::STANDBY: return UINT32_MAX;
		}
	}

private:
	const Types::train_idx_t train_idx;

public:
	const GameData& gamedata;
	const Trains::Train& gamedata_train;

	const GraphDijkstra& graphsolver;

	TrainSolver::State state;
};