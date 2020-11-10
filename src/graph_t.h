#pragma once

#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/adjacency_matrix.hpp>
#include <boost/graph/properties.hpp>

namespace gr {

	using boost::property_tree::ptree;

	struct VertexProps
	{
		std::string name;
		uint32_t post_idx;
		uint64_t color;
	};

	struct EdgeProps
	{
		std::string name;
		double length;
		uint64_t color;
	};

	struct GraphProps
	{
		std::string name;
		uint32_t idx;
	};

	using graph_t = boost::adjacency_list<
		boost::vecS, // edge container type
		boost::vecS, // verte container type
		boost::directedS,
		VertexProps,
		EdgeProps,
		GraphProps
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

}