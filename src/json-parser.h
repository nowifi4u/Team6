#pragma once

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

#include "graph_t.h"

#include <string>
#include <iostream>
#include <vector>
#include <map>
#include <limits>

namespace gr {

	static constexpr uint32_t uint32_t_max = std::numeric_limits<uint32_t>::max();

	ptree jsonParserInit(const char* filename) {
		gr::ptree pt;
		boost::property_tree::read_json("small_graph.json", pt); 
		return pt;
	}

	graph_t importGraph(const char* filename, vertexMap_t& vertexMap, edgeMap_t& edgeMap)
	{
		ptree pt = jsonParserInit(filename);

		// Read Graph properties
		properties(graph).name = pt.get<std::string>("name");	// name
		properties(graph).idx = pt.get<uint32_t>("idx");		// idx

		// Read Vertex properties
		for (const auto& point : pt.get_child("points"))
		{
			uint32_t idx = point.second.get<uint32_t>("idx");

			vertex_descriptor v = boost::add_vertex(graph);

			properties(graph, v).descriptor = v;
			properties(graph, v).idx = idx;
			properties(graph, v).post_idx = point.second.get_optional<uint32_t>("post_idx").get_value_or(uint32_t_max);

			vertexMap[idx] = v;
		}

		// Read Edge properties
		for (const auto& line : pt.get_child("lines"))
		{
			uint32_t idx = line.second.get<uint32_t>("idx");
			auto pts = as_vector<uint32_t>(line.second, "points");

			edge_descriptor e = boost::add_edge(vertexMap[pts[0]], vertexMap[pts[1]], graph).first;

			properties(graph, e).descriptor = e;
			properties(graph, e).idx = idx;
			properties(graph, e).length = line.second.get<double>("length");

			edgeMap[idx] = e;
		}

		graph.m_property->name = pt.get<std::string>("name"); // name

		return graph;
	}

} // namespace gr