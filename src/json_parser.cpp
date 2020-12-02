#include "json-parser.h"
void importGraph(const boost::property_tree::ptree& pt, GraphIdx& g)
{
	g.graph[boost::graph_bundle].name = pt.get<std::string>("name");	// name
	g.graph[boost::graph_bundle].idx = pt.get<uint32_t>("idx");		// idx

	// Read Vertex properties
	for (const auto& point : pt.get_child("points"))
	{
		uint32_t idx = point.second.get<uint32_t>("idx");

		GraphIdx::vertex_descriptor v = g.add_vertex(idx);

		g.graph[v].post_idx = point.second.get_optional<uint32_t>("post_idx").get_value_or(uint32_max);
	}

	// Read Edge properties
	for (const auto& line : pt.get_child("lines"))
	{
		uint32_t idx = line.second.get<uint32_t>("idx");
		auto pts = as_vector<uint32_t>(line.second, "points");

		GraphIdx::edge_descriptor e = g.add_edge(idx, pts[0], pts[1]);

		g.graph[e].length = line.second.get<double>("length");
	}
}

void importGraph(const std::string& filename, GraphIdx& g)
{
	ptree pt;

	boost::property_tree::read_json(filename, pt);

	return importGraph(pt, g);
}