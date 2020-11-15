#pragma once

#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/adjacency_matrix.hpp>
#include <boost/graph/properties.hpp>
#include <boost/graph/graphviz.hpp>
#include <boost/property_map/property_map.hpp>

enum vertex_properties_t { vertex_properties };
enum edge_properties_t { edge_properties };
namespace boost {
	BOOST_INSTALL_PROPERTY(vertex, properties);
	BOOST_INSTALL_PROPERTY(edge, properties);
}

namespace gr {

	using boost::property_tree::ptree;

	//-------------------- graph storage typedefs --------------------//

	struct _VertexProperties
	{
		uint32_t idx;
		uint32_t post_idx;
		uint64_t color;
	};

	using VertexProperties =
		boost::property<boost::vertex_index_t, int,
		boost::property<vertex_properties_t, _VertexProperties
		>>;

	struct _EdgeProperties
	{
		uint32_t idx;
		double length;
		uint64_t color;
	};

	using EdgeProperties =
		boost::property<boost::edge_index_t, int,
		boost::property<edge_properties_t, _EdgeProperties
		>>;


	struct GraphProperties
	{
		uint32_t idx;
		std::string name;
	};

	using graph_t = boost::adjacency_list<
		boost::vecS, // edge container type
		boost::vecS, // verter container type
		boost::undirectedS,
		VertexProperties,
		EdgeProperties,
		GraphProperties
	>;

	//-------------------- graph_traits typedefs --------------------//

	using vertex_descriptor = boost::graph_traits<graph_t>::vertex_descriptor;
	using edge_descriptor = boost::graph_traits<graph_t>::edge_descriptor;

	using vertex_iterator = boost::graph_traits<graph_t>::vertex_iterator;
	using edge_iterator = boost::graph_traits<graph_t>::edge_iterator;

	using vertices_size_type = boost::graph_traits<graph_t>::vertices_size_type;
	using edges_size_type = boost::graph_traits<graph_t>::edges_size_type;

	//-------------------- graph property_map typedefs --------------------//

	using vertex_pmap_index_t = boost::property_map<graph_t, boost::vertex_index_t>::type;
	using vertex_pmap_properties_t = boost::property_map<graph_t, vertex_properties_t>::type;

	using edge_pmap_index_t = boost::property_map<graph_t, boost::edge_index_t>::type;
	using edge_pmap_properties_t = boost::property_map<graph_t, edge_properties_t>::type;

	//-------------------- graph std::map typedefs --------------------//

	using vertexMap_t = std::map<uint32_t, vertex_descriptor>;
	using edgeMap_t = std::map<uint32_t, edge_descriptor>;

	template <class Ty>
	std::vector<Ty> as_vector(const ptree& pt, const ptree::key_type& key)
	{
		std::vector<Ty> res;
		res.reserve(pt.get_child(key).size());
		for (auto& item : pt.get_child(key))
		{
			res.push_back(item.second.get_value<Ty>());
		}
		return res;
	}

	_VertexProperties& properties(graph_t& graph, vertex_descriptor v)
	{
		return boost::get(vertex_properties, graph, v);
	}

	const _VertexProperties& properties(const graph_t& graph, vertex_descriptor v)
	{
		return boost::get(vertex_properties, graph, v);
	}

	_EdgeProperties& properties(graph_t& graph, edge_descriptor e)
	{
		return boost::get(edge_properties, graph, e);
	}

	const _EdgeProperties& properties(const graph_t& graph, edge_descriptor e)
	{
		return boost::get(edge_properties, graph, e);
	}

	_VertexProperties& properties(graph_t& graph, vertex_iterator v)
	{
		return properties(graph, *v);
	}

	const _VertexProperties& properties(const graph_t& graph, vertex_iterator v)
	{
		return properties(graph, *v);
	}

	_EdgeProperties& properties(graph_t& graph, edge_iterator e)
	{
		return properties(graph, *e);
	}

	const _EdgeProperties& properties(const graph_t& graph, edge_iterator e)
	{
		return properties(graph, *e);
	}

	GraphProperties& properties(graph_t& graph)
	{
		return *(graph.m_property);
	}

	const GraphProperties& properties(const graph_t& graph)
	{
		return *(graph.m_property);
	}

	std::ostream& writeGraphDot(std::ostream& out, graph_t graph)
	{
		boost::dynamic_properties dp;

		dp.property("index", boost::get(boost::vertex_index, graph));
		dp.property("idx", boost::get(&_VertexProperties::idx, graph));
		dp.property("post_idx", boost::get(&_VertexProperties::post_idx, graph));
		dp.property("color", boost::get(&_VertexProperties::color, graph));

		dp.property("idx", boost::get(&_EdgeProperties::idx, graph));
		dp.property("length", boost::get(&_EdgeProperties::length, graph));
		dp.property("color", boost::get(&_EdgeProperties::color, graph));

		dp.property("name", boost::make_constant_property<graph_t*>(graph.m_property->name));
		dp.property("idx", boost::make_constant_property<graph_t*>(graph.m_property->idx));

		boost::write_graphviz_dp(out, graph, dp);

		return out;
	}

	void print_graph(std::ostream& out, const gr::graph_t& graph) {

		auto graph_vertices = boost::vertices(graph);

		out << "vertices:" << std::endl;
		gr::vertex_iterator v, vend;
		for (boost::tie(v, vend) = boost::vertices(graph); v != vend; ++v)
		{
			const gr::VertexProperties& vertex = graph[*v];

			out << "id:" << *v
				<< " name:" << vertex.name
				<< " post_idx:" << vertex.post_idx
				<< std::endl;
		}

		out << std::endl << "edges:" << std::endl;
		gr::edge_iterator e, eend;
		for (boost::tie(e, eend) = boost::edges(graph); e != eend; ++e)
		{
			const gr::EdgeProperties& edge = graph[*e];

			out << "id:" << *e
				<< " name:" << edge.name
				<< " length:" << edge.length
				<< std::endl;
		}

		out << std::endl;
	}

} // namespace gr

