#pragma once

#include "../data.h"
#include "../../utils/network/server_connector.h"

class TrainSolver
{
public:

	TrainSolver(const GameData& gamedata, Types::train_idx_t train_idx)
		: train_idx(train_idx), gamedata(gamedata), gamedata_train(gamedata.players.at(gamedata.player_idx).trains.at(train_idx))
	{

	}

	enum class Status
	{
		EFFICIENT_FOOD,
		EFFICIENT_ARMOR,
		EMERGENCY_FOOD,
		EMERGENCY_ARMOR,
		RETURN
	};

	Types::vertex_idx_t calc_destination_EFFICIENT_FOOD() const
	{

	}

	Types::vertex_idx_t calc_destination_EFFICIENT_ARMOR() const
	{

	}

	Types::vertex_idx_t calc_destination_EMERGENCY_FOOD() const
	{

	}

	Types::vertex_idx_t calc_destination_EMERGENCY_ARMOR() const
	{

	}

	Types::vertex_idx_t calc_destination() const
	{
		switch (status)
		{
		case Status::EFFICIENT_FOOD: return calc_destination_EFFICIENT_FOOD();
		case Status::EFFICIENT_ARMOR: return calc_destination_EFFICIENT_ARMOR();
		case Status::EMERGENCY_FOOD: return calc_destination_EMERGENCY_FOOD();
		case Status::EMERGENCY_ARMOR: return calc_destination_EMERGENCY_ARMOR();
		case Status::RETURN: return gamedata.home_idx;
		}
	}

protected:

	const Types::train_idx_t train_idx;

	const GameData& gamedata;
	const Trains::Train& gamedata_train;

public:

	Status status;
};