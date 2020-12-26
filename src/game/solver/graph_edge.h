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

	std::pair<Types::edge_length_t, bool> operator[](GraphIdx::vertex_descriptor v)
	{
		if (s[v] < t[v]) return std::make_pair(s[v], false);
		else return std::make_pair(t[v], true);
	}

	std::pair<const GraphDijkstra&, bool> get_obj(GraphIdx::vertex_descriptor v)
	{
		if (s[v] < t[v]) return std::make_pair(s, false);
		else return std::make_pair(t, true);
	}

	GraphDijkstra::path_t get_path(GraphIdx::vertex_descriptor vend)
	{
		if (s[vend] < t[vend]) return s.calculate_path(vend);
		else return t.calculate_path(vend);
	}
};
