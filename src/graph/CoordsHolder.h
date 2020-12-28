#pragma once

#include <src/graph/graph.h>

#include <boost/property_map/transform_value_property_map.hpp>
#include <boost/graph/topology.hpp>

#include "GraphVertexMap.h"

class CoordsHolder : public GraphVertexMap<typename boost::rectangle_topology<>::point_type>
{
public:

	//------------------------------ TYPEDEFS ------------------------------//

	using topology_type = boost::rectangle_topology<>;
	using point_type = topology_type::point_type;

	//------------------------------ IMPL ------------------------------//


public:

	CoordsHolder() : GraphVertexMap() {}

	CoordsHolder(const Graph::Graph& g) : GraphVertexMap(g) {}

	virtual ~CoordsHolder() = default;
};
