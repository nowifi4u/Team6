#pragma once

#include "graph.h"

#include <algorithm>

class GraphEdgeDijkstra
{
	GraphDijkstra s, t;

public:

	GraphEdgeDijkstra(const GraphIdx::Graph& graph)
		: s(graph), t(graph) {}

	void calculate(GraphIdx::edge_descriptor e, Types::edge_idx_t pos)
	{
		{
			s.calculate(boost::source(e, s.graph_));
			Types::edge_length_t delta_s = pos;
			s.for_each([&](Types::edge_length_t& di) {
				di += delta_s;
				});
		}

		{
			t.calculate(boost::target(e, t.graph_));
			Types::edge_length_t delta_t = s.graph_[e].length - pos;
			t.for_each([&](Types::edge_length_t& di) {
				di += delta_t;
				});
		}
	}

	bool get_is_source(GraphIdx::vertex_descriptor vend) const
	{
		return (s[vend] < t[vend]);
	}

	Types::edge_length_t get_distance(GraphIdx::vertex_descriptor vend) const
	{
		if (get_is_source(vend)) return s[vend];
		else return t[vend];
	}

	const GraphDijkstra& get_obj(GraphIdx::vertex_descriptor vend) const
	{
		if (get_is_source(vend)) return s;
		else return t;
	}

	GraphDijkstra::path_t get_path(GraphIdx::vertex_descriptor vend) const
	{
		if (get_is_source(vend)) return s.calculate_path(vend);
		else return t.calculate_path(vend);
	}

	GraphDijkstra::path_edges_t get_path_edges(GraphIdx::vertex_descriptor vend) const
	{
		if (get_is_source(vend)) return s.calculate_path_edges(vend);
		else return t.calculate_path_edges(vend);
	}
};
