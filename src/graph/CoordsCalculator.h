#pragma once

#include <src/graph/CoordsHolder.h>

class CoordsCalculator : public CoordsHolder
{
public:

	CoordsCalculator() : CoordsHolder() {}

	CoordsCalculator(GraphIdx::Graph& g) : CoordsHolder(g) {}

	virtual ~CoordsCalculator() = default;

	virtual void calculate(GraphIdx::Graph& g, double topology_width, double topology_height, double unit_edge_length) = 0;

};
