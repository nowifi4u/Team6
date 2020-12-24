#pragma once

#include <boost/ptr_container/ptr_vector.hpp>
#include <boost/ptr_container/ptr_map.hpp>

#include <nlohmann/json.hpp>

#include "ClassDefines.h"
#include "Types.h"

#include "graph.h"
#include "KKSCoordsCalculator.h"

#include "Logging.h"



//------------------------------ GAMEDATA ------------------------------//

struct GameData
{

	GraphIdx map_graph;
	CoordsHolder* map_graph_coords = nullptr;
	Types::position_t map_graph_width;
	Types::position_t map_graph_height;

	void clear()
	{
		map_graph.clear();

		if (map_graph_coords != nullptr)
		{
			delete map_graph_coords;
			map_graph_coords = nullptr;
		}
	}

	static void readJSON_L0(GameData& val, const json& j)
	{
		GraphIdx::readJSON_L0(val.map_graph, j);
	}

	//-------------------- CLIENT-SIDE COORDINATES --------------------//

	static void calculateCoordinates(GameData& val, double topology_width, double topology_height, double unit_edge_length)
	{
		val.map_graph_coords = (CoordsHolder*) new KKSCoordsCalculator(val.map_graph.graph, topology_width, topology_height, unit_edge_length);

		// Read Graph border size
		val.map_graph_width = topology_width;
		val.map_graph_height = topology_height;

		val.map_graph_coords->for_each(val.map_graph.graph, [&](CoordsHolder::point_type& point) {
			point[0] = (point[0] + topology_width) / 2;
			point[1] = (point[1] + topology_height) / 2;

			LOG("X:" << point[0] << " Y:" << point[1]);
			});
	}

};
