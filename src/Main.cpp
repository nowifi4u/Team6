#include "json-parser.h"
#include "KKSCoordsCalculator.h"
#include <iostream>
#include <iomanip>
#include <memory>
#include <SFML/Graphics.hpp>
#include "Game.h"


#include <iomanip>
#include "Main.h"

void TestGraph() {
	GraphIdx g;

	std::cout << "Importing Graph..." << std::endl;

	importGraph("GraphExamples/small_graph.json", g);

	std::cout << "Calculating coordinates..." << std::endl;

	KKSCoordsCalculator b;
	b.calculate(g.graph, 200, 200, 3);

	std::cout << "Drawing Graph with coordinates..." << std::endl;

	g.for_each_vertex([&g, &b](auto v) {
		const auto& vertex = g.graph[v];
		std::cout << v
			<< " idx:" << vertex.idx
			<< " post_idx:" << (int32_t)vertex.post_idx
			<< " pos:[" << b[v][0] << ' ' << b[v][1] << ']'
			<< std::endl;
		});
}

int main()
{
	Game game; // Creating our game object.
	game.renderCalculations();
	while (!game.getWindow()->isDone()) {
		// Game loop.
		game.handleInput();
		game.update();
		game.render();
		game.restartClock();
	}
	return 0;
}