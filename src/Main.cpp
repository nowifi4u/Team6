#include "GameData.h"

#include <iostream>
#include <iomanip>
#include <memory>
#include <SFML/Graphics.hpp>


#include <iomanip>

int main()
{
	GameData gamedata;

	std::cout << "Importing GameData..." << std::endl;

	ptree_from_file("small_graph.json", [&](const ptree& pt) {
		GameData::readJSON_L0(gamedata, pt);
		});
	
	std::cout << "Calculating coordinates..." << std::endl;

	std::cout << "Drawing Graph with coordinates..." << std::endl;

	gamedata.graph.for_each_vertex([&](auto v) {
		const auto& vertex = gamedata.graph.graph[v];
		std::cout << v
			<< " idx:" << vertex.idx
			<< " length:" << (int32_t)vertex.post_idx
			<< " pos:[" << vertex.pos_x << ' ' << vertex.pos_y << ']'
			<< std::endl;
		});

	return 0;
}