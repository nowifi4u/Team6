#pragma once

#include "../../graph/graph.h"
#include "../../graph/readable_only_pmap.h"

#include "../../graph/GraphVertexMap.h"

#include <boost/graph/dijkstra_shortest_paths.hpp>

#include <vector>
#include <deque>

class GraphDijkstra
{
public:

	using weight_map_t = boost::property_map<GraphIdx::Graph, Types::edge_length_t GraphIdx::EdgeProperties::*>::const_type;
	using ro_weight_map_t = boost::detail::readable_only_pmap<weight_map_t>;

	using index_map_t = boost::property_map<GraphIdx::Graph, boost::vertex_index_t>::type;

	using path_t = std::deque<GraphIdx::vertex_descriptor>;

	GraphDijkstra(const GraphIdx::Graph& graph)
		: graph_(graph), 
		vbegin(graph.null_vertex()),
		weightmap(boost::get(&GraphIdx::EdgeProperties::length, graph)),
		predecessors(boost::num_vertices(graph)),
		indexmap(boost::get(boost::vertex_index, graph)),
		distances(graph)
	{
		
	}

	void calculate(GraphIdx::vertex_descriptor v)
	{
		vbegin = v;

		boost::dijkstra_shortest_paths(
			graph_,
			v,
			boost::predecessor_map(boost::make_iterator_property_map(predecessors.begin(), indexmap))
			.distance_map(distances.get_map())
			.weight_map(weightmap)
			);
	}

	Types::edge_length_t& operator[](GraphIdx::vertex_descriptor v)
	{
		return distances[v];
	}

	const Types::edge_length_t& operator[](GraphIdx::vertex_descriptor v) const
	{
		return distances[v];
	}

	template <class Func>
	void for_each(Func f)
	{
		distances.for_each(graph_, f);
	}

	template <class Func>
	void for_each(Func f) const
	{
		distances.for_each(graph_, f);
	}

	path_t calculate_path(GraphIdx::vertex_descriptor vend) const
	{
		path_t path;
		for (GraphIdx::vertex_descriptor cur = vend;
			cur != graph_.null_vertex() 
			&& predecessors[cur] != cur 
			&& cur != vbegin;)
		{
			path.push_front(predecessors[cur]);
			cur = predecessors[cur];
		}
		return path;
	}

protected:

	

public:
	const GraphIdx::Graph& graph_;
	GraphIdx::vertex_descriptor vbegin;

	const index_map_t indexmap;

	const weight_map_t weightmap;

	std::vector<GraphIdx::vertex_descriptor> predecessors;
	
	GraphVertexMap<Types::edge_length_t> distances;

};
