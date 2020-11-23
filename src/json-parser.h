#pragma once

#include <boost/property_tree/json_parser.hpp>

#include "graph.h"

using boost::property_tree::ptree;

static constexpr uint32_t uint32_max = std::numeric_limits<uint32_t>::max();

template <typename Ty>
std::vector<Ty> as_vector(const ptree& pt, const ptree::key_type& key)
{
	std::vector<Ty> res;
	res.reserve(pt.get_child(key).size());
	for (auto& item : pt.get_child(key))
	{
		res.push_back(item.second.get_value<Ty>());
	}
	return res;
}



void importGraph(const boost::property_tree::ptree& pt, GraphIdx& g)
{
	g.graph[boost::graph_bundle].name= pt.get<std::string>("name");	// name
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

		
