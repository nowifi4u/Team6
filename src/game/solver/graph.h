#pragma once

#include "../../graph/graph.h"
#include "../../graph/readable_only_pmap.h"

#include "../../graph/GraphVertexMap.h"

#include <boost/graph/dijkstra_shortest_paths.hpp>

#include <vector>

class GraphDijkstra
{
public:

	using weight_map_t = boost::property_map<GraphIdx::Graph, Types::edge_length_t GraphIdx::EdgeProperties::*>::const_type;
	using ro_weight_map_t = boost::detail::readable_only_pmap<weight_map_t>;

	using index_map_t = boost::property_map<GraphIdx::Graph, boost::vertex_index_t>::type;

	GraphDijkstra(const GraphIdx& graph)
		: graph_(graph), 
		weightmap(boost::get(&GraphIdx::EdgeProperties::length, graph.graph)),
		predecessors(boost::num_vertices(graph.graph)),
		indexmap(boost::get(boost::vertex_index, graph.graph)),
		distances(graph.graph)
	{
		
	}

	void calculate(GraphIdx::vertex_descriptor v)
	{
		boost::dijkstra_shortest_paths(
			graph_.graph,
			v,
			boost::predecessor_map(boost::make_iterator_property_map(predecessors.begin(), indexmap))
			.distance_map(distances.get_map())
			.weight_map(weightmap)
			);
	}

protected:

	const GraphIdx& graph_;

public:
	const index_map_t indexmap;

	const weight_map_t weightmap;

	std::vector<GraphIdx::vertex_descriptor> predecessors;
	
	GraphVertexMap<Types::edge_length_t> distances;

};
