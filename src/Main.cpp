#include "json-parser.h"

#include <boost/graph/graphviz.hpp>

#include <iomanip>

int main()
{
	
	gr::vertexMap_t vertexMap;
	gr::edgeMap_t edgeMap;

	gr::graph_t graph = gr::importGraph("small_graph.json", vertexMap, edgeMap);

	gr::print_graph(std::cout, graph);

	//boost::write_graphviz(std::cout, graph);

	return 0;
}