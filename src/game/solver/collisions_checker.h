#pragma once

#include <src/game/solver/train.h>
#include <vector>

class CollisionsChecker {


	static bool solve_towards_collision(TrainSolver& t1, TrainSolver& t2, const GameData& gamedata) {

		std::vector<Graph::edge_descriptor> for_delete;

		auto& tuple1 = t1.possible_move.value();
		auto& tuple2 = t2.possible_move.value();

		auto& path1 = std::get<0>(tuple1);
		auto& path2 = std::get<0>(tuple2);

		for (size_t i = 0; i < path1.size() - 1; ++i) {
			for (size_t j = 0; j < path2.size() - 1; ++j) {
				if (path1[i] == path2[j + 1] && path1[i + 1] == path2[j]) {
					auto opt = Graph::get_edge(gamedata.graph(), path1[i], path1[i + 1]);
					if (opt.has_value())
						for_delete.push_back(opt.value());
				}
			}
		}

		if (!for_delete.empty()) {
			t2.pathsolver.exclude_edges.clear();

			for (Graph::edge_descriptor edge : for_delete)
			{
				t2.pathsolver.exclude_edges.insert(gamedata.graph()[edge].idx);
			}
			t2.calculate_Turn();
			
			return true;
		}
		return false;
	}

	static void check_and_solve(TrainSolver& t1, TrainSolver& t2, const GameData& gamedata) {
		if (t1.possible_move.has_value() && t2.possible_move.has_value()) {
			const GraphIdx& g = gamedata.map_graph;

			auto& tuple1 = t1.possible_move.value();
			auto& tuple2 = t2.possible_move.value();

			auto& path1 = std::get<0>(tuple1);
			auto& path2 = std::get<0>(tuple2);

			auto& path_edges1 = std::get<1>(tuple1);
			auto& path_edges2 = std::get<1>(tuple2);

			auto& move1 = std::get<2>(tuple1);
			auto& move2 = std::get<2>(tuple2);

			bool b = solve_towards_collision(t1, t2, gamedata);

			if (b) {
				//todo: change Move itself

				return;
			}
			auto l1 = t1.gamedata_train.line_idx;
			auto l2 = t2.gamedata_train.line_idx;

			auto move_l1 = move1.line_idx;
			auto move_l2 = move2.line_idx;

			//окажутся ли на одной позиции одного ребра?

			if (move_l1 == move_l2) {
				size_t position1 = UINT32_MAX, position2 = UINT32_MAX;
				if (l1 != move_l1) {
					if (boost::source(g.emap.at(l1), g.graph) == boost::source(g.emap.at(move_l1), g.graph))
						position1 = 0;
					else if (boost::target(g.emap.at(l1), g.graph) == boost::source(g.emap.at(move_l1), g.graph))
						position1 = 0;
					else if (boost::source(g.emap.at(l1), g.graph) == boost::target(g.emap.at(move_l1), g.graph))
						position1 = g.graph[g.emap.at(move_l1)].length - 1;
					else if (boost::target(g.emap.at(l1), g.graph) == boost::target(g.emap.at(move_l1), g.graph))
						position1 = g.graph[g.emap.at(move_l1)].length - 1;
				}
				if (l2 != move_l2) {
					if (boost::source(g.emap.at(l2), g.graph) == boost::source(g.emap.at(move_l2), g.graph))
						position2 = 0;
					else if (boost::target(g.emap.at(l2), g.graph) == boost::source(g.emap.at(move_l2), g.graph))
						position2 = 0;
					else if (boost::source(g.emap.at(l2), g.graph) == boost::target(g.emap.at(move_l2), g.graph))
						position2 = g.graph[g.emap.at(move_l2)].length - 1;
					else if (boost::target(g.emap.at(l2), g.graph) == boost::target(g.emap.at(move_l2), g.graph))
						position2 = g.graph[g.emap.at(move_l2)].length - 1;
				}
				if (position1 == UINT32_MAX)
					position1 = t1.gamedata_train.position;
				if (position2 == UINT32_MAX)
					position2 = t2.gamedata_train.position;

				if (position1 + move1.speed == position2 + move2.speed) {
					if (move2.speed == 0) {
						t1.possible_move = std::nullopt;
					}
					else {
						t2.possible_move = std::nullopt;
					}
				}
			}
			else { //столкновение в вершине
				Graph::vertex_descriptor v1 = UINT32_MAX, v2 = UINT32_MAX;

				if (t2.gamedata_train.position == 0 && move1.speed == 0)
					v1 = boost::source(g.emap.at(l1), g.graph);
				if (t2.gamedata_train.position == 1 && move1.speed == -1)
					v1 = boost::source(g.emap.at(l1), g.graph);
				if (t2.gamedata_train.position == g.graph[g.emap.at(l1)].length - 1 && move1.speed == 0)
					v1 = boost::target(g.emap.at(l1), g.graph);
				if (t2.gamedata_train.position == g.graph[g.emap.at(l1)].length - 2 && move1.speed == 1)
					v1 = boost::target(g.emap.at(l1), g.graph);

				if (t2.gamedata_train.position == 0 && move2.speed == 0)
					v2 = boost::source(g.emap.at(l2), g.graph);
				if (t2.gamedata_train.position == 1 && move2.speed == -1)
					v2 = boost::source(g.emap.at(l2), g.graph);
				if (t2.gamedata_train.position == g.graph[g.emap.at(l2)].length - 1 && move2.speed == 0)
					v2 = boost::target(g.emap.at(l2), g.graph);
				if (t2.gamedata_train.position == g.graph[g.emap.at(l2)].length - 2 && move2.speed == 1)
					v2 = boost::target(g.emap.at(l2), g.graph);

				if (v1 != UINT32_MAX && v1 == v2) {
					if (move2.speed == 0) {
						t1.possible_move = std::nullopt;
					}
					else {
						t2.possible_move = std::nullopt;
					}
				}
			}
		}
	}
public:
	static void check_and_solve(std::vector<TrainSolver>& trainsolvers, const GameData& gamedata) {
		for (size_t i = 0; i < trainsolvers.size(); ++i) {
			for (size_t j = 0; j < trainsolvers.size(); ++j) {
				check_and_solve(trainsolvers[i], trainsolvers[j], gamedata);
			}
		}
	}
};
