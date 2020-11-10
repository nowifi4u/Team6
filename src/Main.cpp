#include "json-parser.h"

#include <boost/graph/graphviz.hpp>

#include <iomanip>

int main()
{

	gr::ptree pt;
	boost::property_tree::read_json("small_graph.json", pt);

	gr::graph_t graph;
	gr::vertexMap_t vertexMap;
	gr::edgeMap_t edgeMap;

	gr::importGraph(pt, graph, vertexMap, edgeMap);

	auto graph_vertices = boost::vertices(graph);

	std::cout << "vertices:" << std::endl;
	gr::vertex_iterator v, vend;
	for (boost::tie(v, vend) = boost::vertices(graph); v != vend; ++v)
	{
		const gr::VertexProperties& vertex = graph[*v];

		std::cout << "id:" << *v
			<< " name:" << vertex.name
			<< " post_idx:" << vertex.post_idx
			<< " color:" << vertex.color
			<< std::endl;
	}

	std::cout << std::endl << "edges:" << std::endl;
	gr::edge_iterator e, eend;
	for (boost::tie(e, eend) = boost::edges(graph); e != eend; ++e)
	{
		const gr::EdgeProperties& edge = graph[*e];

		std::cout << "id:" << *e
			<< " name:" << edge.name
			<< " length:" << edge.length
			<< " color:" << edge.color
			<< std::endl;
	}

	std::cout << std::endl;
	boost::write_graphviz(std::cout, graph);

	return 0;
}