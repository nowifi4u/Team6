#pragma once

#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/properties.hpp>
#include <boost/property_map/property_map.hpp>

#include <vector>
#include <map>
#include <optional>

#include <src/utils/ClassDefines.h>
#include <src/Types.h>
#include <src/utils/Logging.h>

#include <nlohmann/json.hpp>
using nlohmann::json;


namespace Graph {

	struct VertexProperties
	{
		Types::vertex_idx_t idx;
		Types::post_idx_t post_idx;
	};

	struct EdgeProperties
	{
		Types::edge_idx_t idx;
		Types::edge_length_t length;
	};

	struct GraphProperties
	{
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



	template <class Func>
	void for_each_vertex_iterator(Graph& graph, Func f)
	{
		vertex_iterator vi, vend;
		for (boost::tie(vi, vend) = boost::vertices(graph); vi != vend; ++vi)
		{
			f(vi);
		}
	}

	template <class Func>
	void for_each_vertex_iterator(const Graph& graph, Func f)
	{
		vertex_iterator vi, vend;
		for (boost::tie(vi, vend) = boost::vertices(graph); vi != vend; ++vi)
		{
			f(vi);
		}
	}

	template <class Func>
	void for_each_edge_iterator(Graph& graph, Func f)
	{
		edge_iterator ei, eend;
		for (boost::tie(ei, eend) = boost::edges(graph); ei != eend; ++ei)
		{
			f(ei);
		}
	}

	template <class Func>
	void for_each_edge_iterator(const Graph& graph, Func f)
	{
		edge_iterator ei, eend;
		for (boost::tie(ei, eend) = boost::edges(graph); ei != eend; ++ei)
		{
			f(ei);
		}
	}

	template <class Func>
	void for_each_vertex_descriptor(Graph& graph, Func f)
	{
		vertex_iterator vi, vend;
		for (boost::tie(vi, vend) = boost::vertices(graph); vi != vend; ++vi)
		{
			f(*vi);
		}
	}

	template <class Func>
	void for_each_vertex_descriptor(const Graph& graph, Func f)
	{
		vertex_iterator vi, vend;
		for (boost::tie(vi, vend) = boost::vertices(graph); vi != vend; ++vi)
		{
			f(*vi);
		}
	}

	template <class Func>
	void for_each_edge_descriptor(Graph& graph, Func f)
	{
		edge_iterator ei, eend;
		for (boost::tie(ei, eend) = boost::edges(graph); ei != eend; ++ei)
		{
			f(*ei);
		}
	}

	template <class Func>
	void for_each_edge_descriptor(const Graph& graph, Func f)
	{
		edge_iterator ei, eend;
		for (boost::tie(ei, eend) = boost::edges(graph); ei != eend; ++ei)
		{
			f(*ei);
		}
	}

	template <class Func>
	void for_each_vertex_props(Graph& graph, Func f)
	{
		vertex_iterator vi, vend;
		for (boost::tie(vi, vend) = boost::vertices(graph); vi != vend; ++vi)
		{
			f(graph[*vi]);
		}
	}

	template <class Func>
	void for_each_vertex_props(const Graph& graph, Func f)
	{
		vertex_iterator vi, vend;
		for (boost::tie(vi, vend) = boost::vertices(graph); vi != vend; ++vi)
		{
			f(graph[*vi]);
		}
	}

	template <class Func>
	void for_each_edge_props(Graph& graph, Func f)
	{
		edge_iterator ei, eend;
		for (boost::tie(ei, eend) = boost::edges(graph); ei != eend; ++ei)
		{
			f(graph[*ei]);
		}
	}

	template <class Func>
	void for_each_edge_props(const Graph& graph, Func f)
	{
		edge_iterator ei, eend;
		for (boost::tie(ei, eend) = boost::edges(graph); ei != eend; ++ei)
		{
			f(graph[*ei]);
		}
	}

	inline bool isSource(const Graph& g, vertex_descriptor v, vertex_descriptor ve)
	{
		if (boost::edge(v, ve, g).second)
		{
			return v == boost::source(boost::edge(v, ve, g).first, g);
		}
		else return false;
	}

	inline std::optional<edge_descriptor> get_edge(const Graph& g, vertex_descriptor v, vertex_descriptor ve)
	{
		if (boost::edge(v, ve, g).second == true) return boost::edge(v, ve, g).first;
		else if (boost::edge(ve, v, g).second == true) return boost::edge(ve, v, g).first;
		else return std::nullopt;
	}

	inline EdgeProperties* get_edge_props(Graph& g, vertex_descriptor v, vertex_descriptor ve)
	{
		const auto edge = get_edge(g, v, ve);
		if (edge.has_value()) return &g[edge.value()];
		else return nullptr;
	}

	inline const EdgeProperties* get_edge_props(const Graph& g, vertex_descriptor v, vertex_descriptor ve)
	{
		const auto edge = get_edge(g, v, ve);
		if (edge.has_value()) return &g[edge.value()];
		else return nullptr;
	}

}


struct GraphIdx
{
public:

	//------------------------------ TYPEDEFS ------------------------------//

	template <class _K, class _T>
	using idxmap_t = std::map<_K, _T>;

	using vertexMap = idxmap_t<uint32_t, Graph::vertex_descriptor>;
	using edgeMap = idxmap_t<uint32_t, Graph::edge_descriptor>;

	//------------------------------ IMPL ------------------------------//

public:

	vertexMap vmap;
	edgeMap emap;
	Graph::Graph graph;

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

	std::pair<Graph::edge_descriptor, bool> find_edge(Types::vertex_idx_t vidx1, Types::vertex_idx_t vidx2) const
	{
		return boost::edge(vmap.at(vidx1), vmap.at(vidx2), graph);
	}

	Graph::vertex_descriptor add_vertex(Types::vertex_idx_t vidx)
	{
		Graph::vertex_descriptor v = boost::add_vertex(graph);
		graph[v].idx = vidx;
		vmap[vidx] = v;
		return v;
	}

	Graph::edge_descriptor add_edge(Types::edge_idx_t eidx, Types::vertex_idx_t vidx1, Types::vertex_idx_t vidx2)
	{
		auto er = boost::add_edge(vmap.at(vidx1), vmap.at(vidx2), graph);
		if (er.second == false) throw std::runtime_error("Edge already exists");

		Graph::edge_descriptor e = er.first;
		graph[e].idx = eidx;
		emap[eidx] = e;
		return e;
	}

	Graph::VertexProperties& get_vertex(Types::vertex_idx_t vidx)
	{
		return graph[vmap.at(vidx)];
	}

	const Graph::VertexProperties& get_vertex(Types::vertex_idx_t vidx) const
	{
		return graph[vmap.at(vidx)];
	}

	Graph::EdgeProperties& get_edge(Types::vertex_idx_t vidx1, Types::vertex_idx_t vidx2)
	{
		auto e = find_edge(vidx1, vidx2);
		if (e.second == false) throw std::runtime_error("Edge does not exist");
		return graph[e.first];
	}

	const Graph::EdgeProperties& get_edge(Types::vertex_idx_t vidx1, Types::vertex_idx_t vidx2) const
	{
		auto e = find_edge(vidx1, vidx2);
		if (e.second == false) throw std::runtime_error("Edge does not exist");
		return graph[e.first];
	}

	Graph::EdgeProperties& get_edge(Types::edge_idx_t eidx)
	{
		return graph[emap.at(eidx)];
	}

	const Graph::EdgeProperties& get_edge(Types::edge_idx_t eidx) const
	{
		return graph[emap.at(eidx)];
	}

	Graph::GraphProperties& graph_props()
	{
		return graph[boost::graph_bundle];
	}

	const Graph::GraphProperties& graph_props() const
	{
		return graph[boost::graph_bundle];
	}


	static void readJSON_L0(GraphIdx& g, const json& j)
	{
		// Read Vertex properties
		for (const json& ji : j["points"])
		{
			Types::vertex_idx_t idx = ji["idx"].get<Types::vertex_idx_t>();

			Graph::vertex_descriptor v = g.add_vertex(idx);

			g.graph[v].post_idx = ji["post_idx"].is_null() ? UINT32_MAX : ji["post_idx"].get<Types::post_idx_t>();
		}

		// Read Edge properties
		for (const json& ji : j["lines"])
		{
			Types::edge_idx_t idx = ji["idx"].get<Types::edge_idx_t>();
			Types::vertex_idx_t vidx1 = ji["points"][0].get<Types::vertex_idx_t>();
			Types::vertex_idx_t vidx2 = ji["points"][1].get<Types::vertex_idx_t>();

			Graph::edge_descriptor e = g.add_edge(idx, vidx1, vidx2);

			ji["length"].get_to(g.graph[e].length);
		}
	}
};

