#include "json-parser.h"
#include "render.h"


#include <iomanip>

int main()
{
	gr::graph_t g;
	gr::vertexMap_t vmap;
	gr::edgeMap_t emap;

	gr::importGraph("small_graph.json", g, vmap, emap);

	gr::print_graph(std::cout, g);

	std::cout << std::endl;
	//gr::writeGraphDot(std::cout, graph);
	gr::renderCoords(g);
	

	return 0;
}