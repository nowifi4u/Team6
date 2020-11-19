#pragma once

#include <boost/property_tree/json_parser.hpp>

#include "graph.h"

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



void importGraph(const boost::property_tree::ptree& pt, Graph& g, vertexMap& vMap, edgeMap& eMap)
{
	g[boost::graph_bundle].name= pt.get<std::string>("name");	// name
	g[boost::graph_bundle].idx = pt.get<uint32_t>("idx");		// idx

	// Read Vertex properties
	for (const auto& point : pt.get_child("points"))
	{
		uint32_t idx = point.second.get<uint32_t>("idx");

		vertex_descriptor v = boost::add_vertex(g);

		g[v].idx = idx;
		g[v].post_idx = point.second.get_optional<uint32_t>("post_idx").get_value_or(uint32_max);
		
		vMap[idx] = v;
	}

	// Read Edge properties
	for (const auto& line : pt.get_child("lines"))
	{
		uint32_t idx = line.second.get<uint32_t>("idx");
		auto pts = as_vector<uint32_t>(line.second, "points");

		edge_descriptor e = boost::add_edge(vMap[pts[0]], vMap[pts[1]], g).first;

		g[e].idx = idx;
		g[e].length = line.second.get<double>("length");

		eMap[idx] = e;
	}
}



void importGraph(const std::string& filename, Graph& g, vertexMap& vMap, edgeMap& eMap)
{
	ptree pt;
	
	boost::property_tree::read_json(filename, pt);

	importGraph(pt, g, vMap, eMap);
}

		
