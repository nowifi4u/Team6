#pragma once

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

#include "graph_t.h"

#include <string>
#include <iostream>
#include <map>
#include <limits>

namespace gr {

	static constexpr uint32_t uint32_null = std::numeric_limits<uint32_t>::max();

	void importGraph(const boost::property_tree::ptree& pt, graph_t& g, vertexMap_t& vertexMap, edgeMap_t& edgeMap)
	{
		ptree pt = jsonParserInit(filename);

		// Read Graph properties
		properties(g).name = pt.get<std::string>("name");	// name
		properties(g).idx = pt.get<uint32_t>("idx");		// idx

		// Read Vertex properties
		for (const auto& point : pt.get_child("points"))
		{
			uint32_t idx = point.second.get<uint32_t>("idx");

			vertex_descriptor v = boost::add_vertex(g);

			properties(g, v).idx = idx;
			properties(g, v).post_idx = point.second.get_optional<uint32_t>("post_idx").get_value_or(uint32_null);

			vertexMap[idx] = v;
		}

		// Read Edge properties
		for (const auto& line : pt.get_child("lines"))
		{
			uint32_t idx = line.second.get<uint32_t>("idx");
			auto pts = as_vector<uint32_t>(line.second, "points");

			edge_descriptor e = boost::add_edge(vertexMap[pts[0]], vertexMap[pts[1]], g).first;

			properties(g, e).idx = idx;
			properties(g, e).length = line.second.get<double>("length");

			edgeMap[idx] = e;
		}
	}

} // namespace gr