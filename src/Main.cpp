#include "json-parser.h"
#include "KKSCoordsCalculator.h"
#include <iostream>
#include <iomanip>
#include <memory>
#include <SFML/Graphics.hpp>


#include <iomanip>

int main()
{
	Graph g;
	vertexMap vMap;
	edgeMap eMap;

	importGraph("small_graph.json", g, vMap, eMap);
	
	KKSCoordsCalculator b;
	b.calculate(g, 200, 200, 3);

	for_each_vertex(g, [&g, &b](vertex_descriptor v) {
		const VertexProperties& vertex = g[v];
		std::cout << v
			<< " idx:" << vertex.idx
			<< " length:" << (int32_t)vertex.post_idx
			<< " pos:[" << b[v][0] << ' ' << b[v][1] << ']'
			<< std::endl;
		});

	return 0;
}