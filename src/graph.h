#pragma once

#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/properties.hpp>
#include <boost/property_map/property_map.hpp>
#include <boost/property_tree/ptree.hpp>

#include <vector>
#include <string>
#include <iostream>
#include <map>
#include <limits>


using boost::property_tree::ptree;

struct VertexProperties
{
		uint32_t idx;
		uint32_t post_idx;
};

struct EdgeProperties
{
		uint32_t idx;
		double length;
};

struct GraphProperties
{
		uint32_t idx;
		std::string name;
};

using Graph = boost::adjacency_list<
	boost::vecS, // edge container type
	boost::vecS, // verter container type
	boost::undirectedS,
	VertexProperties,
	EdgeProperties,
	GraphProperties
>;

using vertex_descriptor = boost::graph_traits<Graph>::vertex_descriptor;
using edge_descriptor = boost::graph_traits<Graph>::edge_descriptor;

using vertex_iterator = boost::graph_traits<Graph>::vertex_iterator;
using edge_iterator = boost::graph_traits<Graph>::edge_iterator;

using vertexMap = std::map<uint32_t, vertex_descriptor>;
using edgeMap = std::map<uint32_t, edge_descriptor>;
