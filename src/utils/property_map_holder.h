#pragma once

#include <boost/property_map/transform_value_property_map.hpp>
#include <boost/graph/topology.hpp>

#include "../graph/graph.h"

template <class Ty>
class property_map_holder
{
public:

	using PositionVec = std::vector<Ty>;
	using PositionMap = boost::iterator_property_map<PositionVec::iterator, boost::property_map<GraphIdx::Graph, boost::vertex_index_t>::type>;

protected:

	PositionVec m_vec;
	PositionMap m_map;

public:

	property_map_holder() = default;

	property_map_holder(GraphIdx::Graph& g)
	{
		init(g);
	}

	void init(GraphIdx::Graph& g)
	{
		m_vec = PositionVec(num_vertices(g));
		m_map = PositionMap(m_vec.begin(), get(boost::vertex_index, g));
	}

	virtual ~property_map_holder() = default;

	PositionMap& get_map()
	{
		return m_map;
	}

	Ty& operator[](GraphIdx::vertex_descriptor v)
	{
		return m_map[v];
	}

	const Ty& operator[](GraphIdx::vertex_descriptor v) const
	{
		return m_map[v];
	}

	template <class Func>
	void for_each(GraphIdx::Graph& g, Func f)
	{
		GraphIdx::vertex_iterator vi, vend;
		for (boost::tie(vi, vend) = boost::vertices(g); vi != vend; ++vi)
		{
			f(m_map[*vi]);
		}
	}

	template <class Func>
	void for_each(const GraphIdx::Graph& g, Func f) const
	{
		GraphIdx::vertex_iterator vi, vend;
		for (boost::tie(vi, vend) = boost::vertices(g); vi != vend; ++vi)
		{
			f(m_map[*vi]);
		}
	}
};
