#include "json-parser.h"
#include "render.h"


#include <iomanip>

int main()
{

	gr::ptree pt;
	boost::property_tree::read_json("small_graph.json", pt);

	gr::graph_t g;
	gr::vertexMap_t vmap;
	gr::edgeMap_t emap;

	gr::importGraph(pt, g, vmap, emap);

	auto graph_vertices = boost::vertices(g);

	gr::print_graph(std::cout, g);

	std::cout << std::endl;
	//gr::writeGraphDot(std::cout, graph);
	gr::renderCoords(g);
	

	return 0;
}