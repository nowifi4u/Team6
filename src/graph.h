#pragma once

#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/properties.hpp>
#include <boost/property_map/property_map.hpp>

#include <vector>
#include <map>

#include "ClassDefines.h"
#include "Types.h"

#include <nlohmann/json.hpp>
using nlohmann::json;

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

	GraphProperties& graph_props()
	{
		return graph[boost::graph_bundle];
	}





	template <class Func>
	void for_each_vertex_iterator(Func f)
	{
		vertex_iterator vi, vend;
		for (boost::tie(vi, vend) = boost::vertices(graph); vi != vend; ++vi)
		{
			f(vi);
		}
	}

	template <class Func>
	void for_each_edge_iterator(Func f)
	{
		edge_iterator ei, eend;
		for (boost::tie(ei, eend) = boost::edges(graph); ei != eend; ++ei)
		{
			f(ei);
		}
	}

	template <class Func>
	void for_each_vertex_descriptor(Func f)
	{
		vertex_iterator vi, vend;
		for (boost::tie(vi, vend) = boost::vertices(graph); vi != vend; ++vi)
		{
			f(*vi);
		}
	}

	template <class Func>
	void for_each_edge_descriptor(Func f)
	{
		edge_iterator ei, eend;
		for (boost::tie(ei, eend) = boost::edges(graph); ei != eend; ++ei)
		{
			f(*ei);
		}
	}

	template <class Func>
	void for_each_vertex_props(Func f)
	{
		vertex_iterator vi, vend;
		for (boost::tie(vi, vend) = boost::vertices(graph); vi != vend; ++vi)
		{
			f(graph[*vi]);
		}
	}

	template <class Func>
	void for_each_edge_props(Func f)
	{
		edge_iterator ei, eend;
		for (boost::tie(ei, eend) = boost::edges(graph); ei != eend; ++ei)
		{
			f(graph[*ei]);
		}
	}



	static void readJSON_L0(GraphIdx& g, const json& j)
	{
		// Read Vertex properties
		for (const json& ji : j["points"])
		{
			Types::vertex_idx_t idx = ji["idx"].get<Types::vertex_idx_t>();

			GraphIdx::vertex_descriptor v = g.add_vertex(idx);

			g.graph[v].post_idx = ji["post_idx"].is_null() ? UINT32_MAX : ji["post_idx"].get<Types::post_idx_t>();
		}

		// Read Edge properties
		for (const json& ji : j["lines"])
		{
			Types::edge_idx_t idx = ji["idx"].get<Types::edge_idx_t>();
			Types::vertex_idx_t vidx1 = ji["points"][0].get<Types::vertex_idx_t>();
			Types::vertex_idx_t vidx2 = ji["points"][1].get<Types::vertex_idx_t>();

			GraphIdx::edge_descriptor e = g.add_edge(idx, vidx1, vidx2);

			ji["length"].get_to(g.graph[e].length);
		}
	}

	static void readJSON_L10(GraphIdx& g, const json& j)
	{
		// Read Graph border size
		g.graph[boost::graph_bundle].size_width = j["size"][0].get<Types::position_t>();
		g.graph[boost::graph_bundle].size_height = j["size"][1].get<Types::position_t>();

		// Read Vertex coordinates
		for (const json& ji : j["coordinates"])
		{
			Types::vertex_idx_t idx = ji["idx"].get<Types::vertex_idx_t>();
			VertexProperties& vertex = g.get_vertex(idx);

			ji["x"].get_to(vertex.pos_x);
			ji["y"].get_to(vertex.pos_y);
		}
	}
};