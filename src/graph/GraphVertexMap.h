#pragma once

#include "graph.h"

#include <boost/property_map/transform_value_property_map.hpp>
#include <boost/graph/topology.hpp>

#include <vector>

template <class Ty>
class GraphVertexMap
{
public:

	//------------------------------ TYPEDEFS ------------------------------//

	using PositionVec = std::vector<Ty>;
	using PositionMap = typename boost::iterator_property_map< typename PositionVec::iterator, boost::property_map<Graph::Graph, boost::vertex_index_t>::type>;

	//------------------------------ IMPL ------------------------------//

protected:

	PositionVec m_position_vec;
	PositionMap m_position;

public:

	GraphVertexMap() = default;

	GraphVertexMap(const Graph::Graph& g)
	{
		init(g);
	}

	void init(const Graph::Graph& g)
	{
		m_position_vec = PositionVec(num_vertices(g));
		m_position = PositionMap(m_position_vec.begin(), get(boost::vertex_index, g));
	}

	virtual ~GraphVertexMap() = default;

	PositionMap& get_map()
	{
		return m_position;
	}

	const PositionMap& get_map() const
	{
		return m_position;
	}

	Ty& operator[](Graph::vertex_descriptor v)
	{
		return m_position[v];
	}

	const Ty& operator[](Graph::vertex_descriptor v) const
	{
		return m_position[v];
	}

	template <class Func>
	void for_each(Graph::Graph& g, Func f)
	{
		Graph::vertex_iterator vi, vend;
		for (boost::tie(vi, vend) = boost::vertices(g); vi != vend; ++vi)
		{
			f(m_position[*vi]);
		}
	}

	template <class Func>
	void for_each(const Graph::Graph& g, Func f) const
	{
		Graph::vertex_iterator vi, vend;
		for (boost::tie(vi, vend) = boost::vertices(g); vi != vend; ++vi)
		{
			f(m_position[*vi]);
		}
	}
};
