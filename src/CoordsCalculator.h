#pragma once

#include "graph.h"

#include <boost/property_map/transform_value_property_map.hpp>
#include <boost/graph/topology.hpp>

#include <algorithm>



class CoordsCalculator
{
public:

	//------------------------------ TYPEDEFS ------------------------------//

	using topology_type = boost::rectangle_topology<>;
	using point_type = topology_type::point_type;

	using PositionVec = std::vector< point_type >;
	using PositionMap = boost::iterator_property_map< PositionVec::iterator, boost::property_map< GraphIdx::Graph, boost::vertex_index_t >::type >;

	//------------------------------ IMPL ------------------------------//

protected:

	PositionVec m_position_vec;
	PositionMap m_position;

public:

	CoordsCalculator() = default;

	virtual ~CoordsCalculator() = default;

	virtual void calculate(GraphIdx::Graph& g, double topology_width, double topology_height, double unit_edge_length) = 0;

	PositionMap& get_map()
	{
		return m_position;
	}

	point_type& operator[](GraphIdx::vertex_descriptor v)
	{
		return m_position[v];
	}

	const point_type& operator[](GraphIdx::vertex_descriptor v) const
	{
		return m_position[v];
	}
};



