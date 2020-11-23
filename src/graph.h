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

struct GraphIdx
{
public:

	//------------------------------ TYPEDEFS ------------------------------//

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

	static inline constexpr uint32_t uint32_max = std::numeric_limits<uint32_t>::max();

	//------------------------------ IMPL ------------------------------//

public:

	vertexMap vmap;
	edgeMap emap;
	Graph graph;

public:

	GraphIdx() = default;

	GraphIdx(const GraphIdx& g)
		: vmap(g.vmap), 
		emap(g.emap), 
		graph(g.graph) {}

	GraphIdx(GraphIdx&& g) noexcept
		: vmap(std::move(g.vmap)),
		emap(std::move(g.emap)),
		graph(boost::move(g.graph)) {}

	virtual ~GraphIdx() {}

	void clear()
	{
		vmap.clear();
		emap.clear();
		graph.clear();
	}

	std::pair<edge_descriptor, bool> get_edge(uint32_t vidx1, uint32_t vidx2)
	{
		return boost::edge(vmap[vidx1], vmap[vidx2], graph);
	}

	vertex_descriptor add_vertex(uint32_t vidx)
	{
		vertex_descriptor v = boost::add_vertex(graph);
		graph[v].idx = vidx;
		vmap[vidx] = v;
		return v;
	}

	edge_descriptor add_edge(uint32_t eidx, uint32_t vidx1, uint32_t vidx2)
	{
		auto er = boost::add_edge(vmap[vidx1], vmap[vidx2], graph);
		if (er.second == false) throw std::runtime_error("Edge already exists");

		edge_descriptor e = er.first;
		graph[e].idx = eidx;
		emap[eidx] = e;
		return e;
	}

	template <class Func>
	void for_each_vertex(Func f)
	{
		vertex_iterator vi, vend;
		for (boost::tie(vi, vend) = boost::vertices(graph); vi != vend; ++vi)
		{
			f(*vi);
		}
	}

	template <class Func>
	void for_each_edge(Func f)
	{
		edge_iterator ei, eend;
		for (boost::tie(ei, eend) = boost::edges(graph); ei != eend; ++ei)
		{
			f(*ei);
		}
	}

};