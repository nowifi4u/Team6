#pragma once
#include "CoordsCalculator.h"

constexpr double eps = 0.01;

struct KamadaKawaiDone
{
	KamadaKawaiDone() : last_delta() {}

	template<typename Graph>
	bool operator()(double delta_p,
		typename boost::graph_traits<Graph>::vertex_descriptor /*p*/,
		const Graph& /*g*/,
		bool global)
	{
		if (global) {
			double diff = last_delta - delta_p;
			if (diff < 0) diff = -diff;
			last_delta = delta_p;
			return diff < 0.01;
		}
		else {
			return delta_p < eps;
		}
	}

	double last_delta;
};

struct to_double {
	double operator()(int i) const 
	{ 
		return i; 
	} 
};


class KKSCoordsCalculator :
    public CoordsCalculator
{
public:

	KKSCoordsCalculator() : CoordsCalculator() {}

	void calculate(GraphIdx::Graph& g, double topology_width, double topology_height, double unit_edge_length)
	{
		PositionVec position_vec(num_vertices(g));
		PositionMap position(position_vec.begin(), get(boost::vertex_index, g));
		boost::minstd_rand gen;
		topology_type topo(gen, 0, 0, topology_width, topology_width);

		circle_graph_layout(g, position, std::min(topology_width, topology_height));

		bool kamada = kamada_kawai_spring_layout(
			g,
			position,
			boost::make_transform_value_property_map(to_double(), get(&GraphIdx::EdgeProperties::length, g)),
			topo,
			boost::edge_length(unit_edge_length),
			KamadaKawaiDone()
		);


		m_position_vec = std::move(position_vec);
		m_position = boost::move(position);
	}
};

