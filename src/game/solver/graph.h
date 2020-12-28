#pragma once

#include <src/graph/graph.h>
#include <src/graph/readable_only_pmap.h>

#include <src/graph/GraphVertexMap.h>

#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/property_map/transform_value_property_map.hpp>

#include <vector>
#include <deque>
#include <set>

class GraphDijkstra
{
public:

	using weight_map_t = boost::property_map<Graph::Graph, Types::edge_length_t Graph::EdgeProperties::*>::const_type;
	using ro_weight_map_t = detail::readable_only_pmap<weight_map_t>;

	using weightmap_transform_t = std::set<Types::edge_idx_t>;

	using index_map_t = boost::property_map<Graph::Graph, boost::vertex_index_t>::type;

	GraphDijkstra(const Graph::Graph& graph, const std::set<Types::edge_idx_t>& weighmap_transform)
		: graph_(graph), 
		weightmap_transform(weightmap_transform),
		vbegin(graph.null_vertex()),
		weightmap(boost::get(&Graph::EdgeProperties::length, graph)),
		predecessors_vec(boost::num_vertices(graph)),
		predecessors(predecessors_vec.begin(), boost::get(boost::vertex_index, graph)),
		distances_vec(boost::num_vertices(graph)),
		distances(distances_vec.begin(), boost::get(boost::vertex_index, graph))
	{
		
	}

	void calculate(Graph::vertex_descriptor v)
	{
		vbegin = v;

		boost::dijkstra_shortest_paths(
			graph_,
			v,
			boost::predecessor_map(predecessors)
			.distance_map(distances)
			.weight_map(boost::make_transform_value_property_map([&](const Graph::EdgeProperties& edge) { 
				return (weightmap_transform.find(edge.idx) == weightmap_transform.end() ? INFINITY : edge.length);
				}, get(boost::edge_bundle, graph_)))
			);
	}

	double& operator[](Graph::vertex_descriptor v)
	{
		return distances[v];
	}

	const double& operator[](Graph::vertex_descriptor v) const
	{
		return distances[v];
	}

	template <class Func>
	void for_each(Func f)
	{
		Graph::for_each_vertex_descriptor(graph_, [&](Graph::vertex_descriptor v) {
			f(distances[v]);
			});
	}

	template <class Func>
	void for_each(Func f) const
	{
		Graph::for_each_vertex_descriptor(graph_, [&](Graph::vertex_descriptor v) {
			f(distances[v]);
			});
	}

	using path_t = std::deque<Graph::vertex_descriptor>;

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

	using path_edges_t = std::deque<Graph::edge_descriptor>;

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

	const weight_map_t weightmap;
	const std::set<Types::edge_idx_t>& weightmap_transform;

	GraphVertexMap< Graph::vertex_descriptor>::PositionVec predecessors_vec;
	GraphVertexMap< Graph::vertex_descriptor>::PositionMap predecessors;
	
	GraphVertexMap<double>::PositionVec distances_vec;
	GraphVertexMap<double>::PositionMap distances;

};
