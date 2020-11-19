#include "json-parser.h"
#include "KKSCoordsCalculator.h"
#include <iomanip>
#include <memory>
#include <SFML/Graphics.hpp>


int main()
{
	Graph g;

	vertexMap vMap;
	edgeMap eMap;

	//catch json_parser_error!!!
	importGraph("small_graph.json", g, vMap, eMap);

	KKSCoordsCalculator b;
	
	PositionMap pos = b.get_coords(g, 200, 200, 3);

	return 0;
}

