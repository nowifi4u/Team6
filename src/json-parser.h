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

	graph_t importGraph(const boost::property_tree::ptree& pt, graph_t& graph, vertexMap_t& vertexMap, edgeMap_t& edgeMap)
	{
		// Read Graph properties
		graph.m_property->name = pt.get<std::string>("name"); // name
		graph.m_property->idx = pt.get<uint32_t>("idx"); // idx

		// Read Vertex properties
		for (const auto& point : pt.get_child("points"))
		{
			uint32_t idx = point.second.get<uint32_t>("idx");

			vertex_t v = boost::add_vertex(VertexProperties
				{
				std::to_string(idx),															// name
				point.second.get_optional<uint32_t>("post_idx").get_value_or(uint32_t_max)		// post_idx
				}, graph);

			vertexMap[idx] = v;
		}

		// Read Edge properties
		for (const auto& line : pt.get_child("lines"))
		{
			uint32_t idx = line.second.get<uint32_t>("idx");
			auto pts = as_vector<uint32_t>(line.second, "points");

			edge_t e = boost::add_edge(vertexMap[pts[0]], vertexMap[pts[1]], EdgeProperties
				{
				std::to_string(idx),					// name
				line.second.get<double>("length")		// length
				}, graph).first;

			edgeMap[idx] = e;
		}

		return graph;
	}

} // namespace gr