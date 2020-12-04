#include "graph.h"
#include "KKSCoordsCalculator.h"
#include <iostream>
#include <iomanip>
#include <memory>
#include <SFML/Graphics.hpp>


#include <iomanip>

int main()
{
	GraphIdx g;

	std::cout << "Importing Graph..." << std::endl;

	readJSON_L0_file(g, "small_graph.json");
	
	std::cout << "Calculating coordinates..." << std::endl;

	KKSCoordsCalculator b;
	b.calculate(g.graph, 200, 200, 3);

	std::cout << "Drawing Graph with coordinates..." << std::endl;

	g.for_each_vertex([&g, &b](auto v) {
		const auto& vertex = g.graph[v];
		std::cout << v
			<< " idx:" << vertex.idx
			<< " length:" << (int32_t)vertex.post_idx
			<< " pos:[" << b[v][0] << ' ' << b[v][1] << ']'
			<< std::endl;
		});

	return 0;
}