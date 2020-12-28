#pragma once

#include "graph.h"

#include <boost/property_map/transform_value_property_map.hpp>
#include <boost/graph/topology.hpp>

#include <vector>

template <class Ty>
class GraphEdgeMap
{
public:

	//------------------------------ TYPEDEFS ------------------------------//

	using PositionVec = std::vector<Ty>;
	using PositionMap = typename boost::iterator_property_map< typename PositionVec::iterator, boost::property_map<Graph::Graph, boost::edge_bundle_t>::type>;

	//------------------------------ IMPL ------------------------------//

protected:

	PositionVec m_position_vec;
	PositionMap m_position;

public:

	GraphEdgeMap() = default;

	GraphEdgeMap(const Graph::Graph& g)
	{
		init(g);
	}

	static void init(const Graph::Graph& g, PositionVec& position_vec, PositionMap& position_map)
	{
		m_position_vec = PositionVec(boost::num_edges(g));
		m_position = PositionMap(m_position_vec.begin(), get(boost::edge_bundle, g));
	}

	void init(const Graph::Graph& g)
	{
		m_position_vec = PositionVec(boost::num_edges(g));
		m_position = PositionMap(m_position_vec.begin(), get(boost::edge_bundle, g));
	}

	virtual ~GraphEdgeMap() = default;

	PositionVec& get_vec()
	{
		return m_position_vec;
	}

	const PositionVec& get_vec() const
	{
		return m_position_vec;
	}

	PositionMap& get_map()
	{
		return m_position;
	}

	const PositionMap& get_map() const
	{
		return m_position;
	}

	Ty& operator[](Graph::edge_descriptor v)
	{
		return m_position[v];
	}

	const Ty& operator[](Graph::edge_descriptor v) const
	{
		return m_position[v];
	}

	template <class Func>
	void for_each(Graph::Graph& g, Func f)
	{
		Graph::for_each_edge_descriptor(g, [&](Graph::edge_descriptor e) {
			f(m_position[e]);
			});
	}

	template <class Func>
	void for_each(const Graph::Graph& g, Func f) const
	{
		Graph::for_each_edge_descriptor(g, [&](Graph::edge_descriptor e) {
			f(m_position[e]);
			});
	}
};
