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

	using weight_map_t = boost::property_map<Graph::Graph, Types::edge_length_t Graph::EdgeProperties::*>::const_type;
	using ro_weight_map_t = boost::detail::readable_only_pmap<weight_map_t>;

	using index_map_t = boost::property_map<Graph::Graph, boost::vertex_index_t>::type;

	using path_t = std::deque<Graph::vertex_descriptor>;

	using path_edges_t = std::deque<Graph::edge_descriptor>;

	GraphDijkstra(const Graph::Graph& graph)
		: graph_(graph), 
		vbegin(graph.null_vertex()),
		weightmap(boost::get(&Graph::EdgeProperties::length, graph)),
		predecessors(boost::num_vertices(graph)),
		indexmap(boost::get(boost::vertex_index, graph)),
		distances(graph)
	{
		
	}

	void calculate(Graph::vertex_descriptor v)
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

	Types::edge_length_t& operator[](Graph::vertex_descriptor v)
	{
		return distances[v];
	}

	const Types::edge_length_t& operator[](Graph::vertex_descriptor v) const
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

	path_t calculate_path(Graph::vertex_descriptor vend) const
	{
		path_t path;
		for (Graph::vertex_descriptor cur = vend;
			cur != graph_.null_vertex() 
			&& predecessors[cur] != cur 
			&& cur != vbegin;)
		{
			path.push_front(predecessors[cur]);
			cur = predecessors[cur];
		}
		return path;
	}

	path_edges_t calculate_path_edges(Graph::vertex_descriptor vend) const
	{
		path_edges_t path;
		for (Graph::vertex_descriptor cur = vend;
			cur != graph_.null_vertex()
			&& cur != vbegin;)
		{
			path.push_front(Graph::get_edge(graph_, cur, predecessors[cur]).value());
			cur = predecessors[cur];
		}
		return path;
	}

protected:

	

public:
	const Graph::Graph& graph_;
	Graph::vertex_descriptor vbegin;

	const index_map_t indexmap;

	const weight_map_t weightmap;

	std::vector<Graph::vertex_descriptor> predecessors;
	
	GraphVertexMap<Types::edge_length_t> distances;

};
