#pragma once

#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/properties.hpp>
#include <boost/property_map/property_map.hpp>

#include <vector>
#include <map>

#include "ClassDefines.h"
#include "Types.h"

#include "json-parser.h"

#include "Logging.h"

struct GraphIdx
{
public:

	//------------------------------ TYPEDEFS ------------------------------//

	struct VertexProperties
	{
		Types::vertex_idx_t idx;
		Types::post_idx_t post_idx;
		Types::position_t pos_x;
		Types::position_t pos_y;
	};

	struct EdgeProperties
	{
		Types::edge_idx_t idx;
		Types::edge_length_t length;
	};

	struct GraphProperties
	{
		Types::position_t size_width;
		Types::position_t size_height;
	};

	using Graph = boost::adjacency_list<
		boost::vecS, // vertex container type
		boost::vecS, // edge container type
		boost::undirectedS,
		VertexProperties,
		EdgeProperties,
		GraphProperties
	>;

	using vertex_descriptor = boost::graph_traits<Graph>::vertex_descriptor;
	using edge_descriptor = boost::graph_traits<Graph>::edge_descriptor;

	using vertex_iterator = boost::graph_traits<Graph>::vertex_iterator;
	using edge_iterator = boost::graph_traits<Graph>::edge_iterator;

	template <class _K, class _T>
	using idxmap_t = std::map<_K, _T>;

	using vertexMap = idxmap_t<uint32_t, vertex_descriptor>;
	using edgeMap = idxmap_t<uint32_t, edge_descriptor>;

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

	CLASS_VIRTUAL_DESTRUCTOR(GraphIdx);

	void clear()
	{
		vmap.clear();
		emap.clear();
		graph.clear();
	}

	std::pair<edge_descriptor, bool> find_edge(Types::vertex_idx_t vidx1, Types::vertex_idx_t vidx2) throw(std::out_of_range)
	{
		return boost::edge(vmap.at(vidx1), vmap.at(vidx2), graph);
	}

	vertex_descriptor add_vertex(Types::vertex_idx_t vidx)
	{
		vertex_descriptor v = boost::add_vertex(graph);
		graph[v].idx = vidx;
		vmap[vidx] = v;
		return v;
	}

	edge_descriptor add_edge(Types::edge_idx_t eidx, Types::vertex_idx_t vidx1, Types::vertex_idx_t vidx2) throw(std::runtime_error)
	{
		auto er = boost::add_edge(vmap.at(vidx1), vmap.at(vidx2), graph);
		if (er.second == false) throw std::runtime_error("Edge already exists");

		edge_descriptor e = er.first;
		graph[e].idx = eidx;
		emap[eidx] = e;
		return e;
	}

	VertexProperties& get_vertex(Types::vertex_idx_t vidx) throw(std::out_of_range)
	{
		return graph[vmap.at(vidx)];
	}

	EdgeProperties& get_edge(Types::vertex_idx_t vidx1, Types::vertex_idx_t vidx2) throw(std::out_of_range, std::runtime_error)
	{
		auto e = find_edge(vidx1, vidx2);
		if (e.second == false) throw std::runtime_error("Edge does not exist");
		return graph[e.first];
	}

	EdgeProperties& get_edge(Types::edge_idx_t eidx) throw(std::out_of_range)
	{
		return graph[emap.at(eidx)];
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



	static void readJSON_L0(GraphIdx& g, const ptree& pt)
	{
		// Read Vertex properties
		ptree_array_foreach(pt, "points", [&](const ptree& pt) {
			Types::vertex_idx_t idx = pt.get<Types::vertex_idx_t>("idx");

			GraphIdx::vertex_descriptor v = g.add_vertex(idx);

			g.graph[v].post_idx = pt.get_optional<Types::post_idx_t>("post_idx").get_value_or(UINT32_MAX);
			});

		// Read Edge properties
		ptree_array_foreach(pt, "lines", [&](const ptree& pt) {
			Types::edge_idx_t idx = pt.get<Types::edge_idx_t>("idx");
			auto pts = ptree_as_vector<Types::vertex_idx_t>(pt, "points");

			GraphIdx::edge_descriptor e = g.add_edge(idx, pts[0], pts[1]);

			g.graph[e].length = pt.get<Types::edge_length_t>("length");
			});
	}

	static void readJSON_L10(GraphIdx& g, const ptree& pt)
	{
		// Read Graph border size
		g.graph[boost::graph_bundle].size_width = pt.get_child("size").get<Types::position_t>("width");
		g.graph[boost::graph_bundle].size_height = pt.get_child("size").get<Types::position_t>("height");

		// Read Vertex coordinates
		ptree_array_foreach(pt, "coordinates", [&](const ptree& pt) {
			Types::vertex_idx_t idx = pt.get<Types::vertex_idx_t>("idx");
			VertexProperties& vertex = g.get_vertex(idx);

			vertex.pos_x = pt.get<Types::position_t>("x");
			vertex.pos_y = pt.get<Types::position_t>("y");
			});
	}
};