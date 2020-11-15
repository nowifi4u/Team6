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

	std::cout << "vertices:" << std::endl;
	gr::vertex_iterator v, vend;
	for (boost::tie(v, vend) = boost::vertices(g); v != vend; ++v)
	{
		const gr::_VertexProperties& vertex = gr::properties(g, v);

		std::cout << "id:" << *v
			<< " idx:" << vertex.idx
			<< " post_idx:" << (int32_t)vertex.post_idx
			<< " color:" << vertex.color
			<< std::endl;
	}

	std::cout << std::endl << "edges:" << std::endl;
	gr::edge_iterator e, eend;
	for (boost::tie(e, eend) = boost::edges(g); e != eend; ++e)
	{
		const gr::_EdgeProperties& edge = gr::properties(g, e);

		std::cout << "id:" << *e
			<< " idx:" << edge.idx
			<< " length:" << edge.length
			<< " color:" << edge.color
			<< std::endl;
	}

	std::cout << std::endl;
	//gr::writeGraphDot(std::cout, graph);
	gr::renderCoords(g);
	

	return 0;
}