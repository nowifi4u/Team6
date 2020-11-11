#pragma once

#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/properties.hpp>
#include <boost/graph/graphviz.hpp>
#include <boost/property_map/property_map.hpp>

namespace gr {

	using boost::property_tree::ptree;

	struct VertexProperties;
	struct EdgeProperties;
	struct GraphProperties;

	using graph_t = boost::adjacency_list<
		boost::vecS, // edge container type
		boost::vecS, // verte container type
		boost::directedS,
		VertexProperties,
		EdgeProperties,
		GraphProperties
	>;

	using vertex_descriptor = boost::graph_traits<graph_t>::vertex_descriptor;
	using edge_descriptor = boost::graph_traits<graph_t>::edge_descriptor;

	using vertex_iterator = boost::graph_traits<graph_t>::vertex_iterator;
	using edge_iterator = boost::graph_traits<graph_t>::edge_iterator;

	struct VertexProperties
	{
		vertex_descriptor descriptor;
		uint32_t idx;
		uint32_t post_idx;
		//uint64_t color; - �� ����� ������� ����������� ���������, ���������� Exterior Properties
	};

	struct EdgeProperties
	{
		edge_descriptor descriptor;
		uint32_t idx;
		double length;
		uint64_t color;
	};

	struct GraphProperties
	{
		std::string name;
		//uint32_t idx;  - ����� �� �� ���� ���� ������?
	};

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

	VertexProperties& properties(graph_t& graph, vertex_descriptor v)
	{
		return graph[v];
	}

	EdgeProperties& properties(graph_t& graph, edge_descriptor e)
	{
		return graph[e];
	}

	VertexProperties& properties(graph_t& graph, vertex_iterator v)
	{
		return properties(graph, *v);
	}

	EdgeProperties& properties(graph_t& graph, edge_iterator e)
	{
		return properties(graph, *e);
	}

	GraphProperties& properties(graph_t& graph)
	{
		return *graph.m_property;
	}

	std::ostream& writeGraphDot(std::ostream& out, graph_t graph)
	{
		boost::dynamic_properties dp;

		dp.property("idx", boost::get(&VertexProperties::idx, graph));
		dp.property("post_idx", boost::get(&VertexProperties::post_idx, graph));
		dp.property("color", boost::get(&VertexProperties::color, graph));

		dp.property("idx", boost::get(&EdgeProperties::idx, graph));
		dp.property("length", boost::get(&EdgeProperties::length, graph));
		dp.property("color", boost::get(&EdgeProperties::color, graph));

		dp.property("name", boost::make_constant_property<graph_t*>(graph.m_property->name));
		dp.property("idx", boost::make_constant_property<graph_t*>(graph.m_property->idx));

		boost::write_graphviz_dp(out, graph, dp);

		return out;
	}

} // namespace gr
