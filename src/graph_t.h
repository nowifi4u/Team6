#pragma once

#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/adjacency_matrix.hpp>
#include <boost/graph/properties.hpp>

namespace gr {

	using boost::property_tree::ptree;

	struct VertexProperties
	{
		std::string name;
		uint32_t post_idx;
		//uint64_t color; - не будем излешне перегружать структуру, используем Exterior Properties
	};

	struct EdgeProperties
	{
		std::string name;
		double length;
		uint64_t color;
	};

	struct GraphProperties
	{
		std::string name;
		//uint32_t idx;  - вроде же не было пока такого?
	};

	using graph_t = boost::adjacency_list<
		boost::vecS, // edge container type
		boost::vecS, // verte container type
		boost::directedS,
		VertexProperties,
		EdgeProperties,
		GraphProperties
	>;

	using vertex_t = boost::graph_traits<graph_t>::vertex_descriptor;
	using edge_t = boost::graph_traits<graph_t>::edge_descriptor;

	using vertex_iterator = boost::graph_traits<graph_t>::vertex_iterator;
	using edge_iterator = boost::graph_traits<graph_t>::edge_iterator;

	//using vertex_biiterator = std::pair<vertex_iterator, vertex_iterator>;
	//using vertex_biiterator = std::pair<edge_iterator, edge_iterator>;

	using vertexMap_t = std::map<uint32_t, vertex_t>;
	using edgeMap_t = std::map<uint32_t, edge_t>;

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
}

