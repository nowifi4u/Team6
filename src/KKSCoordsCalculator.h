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
	virtual PositionMap get_coords(Graph& g, double topology_width, double topology_height, double unit_edge_length);
};


