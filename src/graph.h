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

template <class _Func>
void for_each_vertex(Graph& g, _Func f)
{
	vertex_iterator vi, vend;
	for (boost::tie(vi, vend) = boost::vertices(g); vi != vend; ++vi)
	{
		f(*vi);
	}
}

template <class _Func>
void for_each_edge(Graph& g, _Func f)
{
	edge_iterator ei, eend;
	for (boost::tie(ei, eend) = boost::edges(g); ei != eend; ++ei)
	{
		f(*ei);
	}
}

template <class _Func>
void for_each_vertex(const vertexMap& vmap, _Func f)
{
	for (const auto& vp : vmap)
	{
		f(vp);
	}
}

template <class _Func>
void for_each_edge(const edgeMap& emap, _Func f)
{
	for (const auto& ep : emap)
	{
		f(ep);
	}
}
