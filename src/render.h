#pragma once

#include "graph_t.h"

#include <boost/graph/planar_canonical_ordering.hpp>
#include <boost/graph/is_straight_line_drawing.hpp>
#include <boost/graph/chrobak_payne_drawing.hpp>
#include <boost/graph/boyer_myrvold_planar_test.hpp>

#include <vector>

namespace gr {

	struct coord_t
	{
		std::size_t x;
		std::size_t y;
	};

	using embedding_storage_t = std::vector<std::vector<edge_descriptor>>;
	using embedding_t = boost::iterator_property_map<
		embedding_storage_t::iterator,
		boost::property_map<graph_t, vertex_descriptor>::type>;

	using straight_line_drawing_storage_t = std::vector<coord_t>;
	using straight_line_drawing_t = boost::iterator_property_map<
		straight_line_drawing_storage_t::iterator,
		boost::property_map<graph_t, vertex_descriptor>::type>;

	void renderCoords(graph_t& graph)
	{
		// Create the planar embedding
		embedding_storage_t embedding_storage(boost::num_vertices(graph));
		embedding_t embedding(embedding_storage.begin(), boost::get(&VertexProperties::descriptor, graph));
	
		boost::boyer_myrvold_planarity_test(boost::boyer_myrvold_params::graph = graph, 
			boost::boyer_myrvold_params::embedding = embedding);

		// Find a canonical ordering
		std::vector<vertex_descriptor> ordering;
		boost::planar_canonical_ordering(graph, embedding, std::back_inserter(ordering));

		// Set up a property map to hold the mapping from vertices to coord_t's
		straight_line_drawing_storage_t straight_line_drawing_storage(boost::num_vertices(graph));
		straight_line_drawing_t straight_line_drawing(straight_line_drawing_storage.begin(), boost::get(boost::vertex_index, graph));
		
		boost::chrobak_payne_straight_line_drawing(graph, embedding, ordering.begin(), ordering.end(), straight_line_drawing);
	
		vertex_iterator vi, vend;
		for (boost::tie(vi, vend) = boost::vertices(graph); vi != vend; ++vi)
		{
			coord_t coord(boost::get(straight_line_drawing, *vi));
			std::cout << *vi << " -> (" << coord.x << ", " << coord.y << ")" << std::endl;
		}

		if (boost::is_straight_line_drawing(graph, straight_line_drawing))
			std::cout << "Is a plane drawing." << std::endl;
		else
			std::cout << "Is not a plane drawing." << std::endl;
	}

} // namespace gr