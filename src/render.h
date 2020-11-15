#pragma once

#include "graph_t.h"

#include <boost/graph/planar_canonical_ordering.hpp>
#include <boost/graph/is_straight_line_drawing.hpp>
#include <boost/graph/chrobak_payne_drawing.hpp>
#include <boost/graph/boyer_myrvold_planar_test.hpp>

#include <boost/graph/make_maximal_planar.hpp>
#include <boost/graph/make_biconnected_planar.hpp>

#include <vector>

namespace gr {

	// Struct to hold the coordinates of the straight line embedding
	struct coord_t
	{
		std::size_t x;
		std::size_t y;
	};

	using vec_t = std::vector<edge_descriptor>;
	using embedding_t = std::vector<vec_t>;

	using straight_line_drawing_storage_t = std::vector<coord_t>;
	using straight_line_drawing_t = boost::iterator_property_map<
		straight_line_drawing_storage_t::iterator,
		boost::property_map<graph_t, boost::vertex_index_t>::type
	>;

	void renderCoords(graph_t g)
	{
		//Initialize the interior edge index
		edge_pmap_index_t e_index = boost::get(boost::edge_index, g);

		{
			edges_size_type edge_count = 0;
			edge_iterator ei, ei_end;
			for (boost::tie(ei, ei_end) = boost::edges(g); ei != ei_end; ++ei)
			{
				boost::put(e_index, *ei, edge_count);
				edge_count++;
			}
		}

		//Test for planarity; compute the planar embedding as a side-effect
		embedding_t embedding(boost::num_vertices(g));
		boost::boyer_myrvold_planarity_test(
			boost::boyer_myrvold_params::graph = g,
			boost::boyer_myrvold_params::embedding = &embedding[0]
		);


		boost::make_biconnected_planar(g, &embedding[0]);

		// Re-initialize the edge index, since we just added a few edges
		{
			edges_size_type edge_count = 0;
			edge_iterator ei, ei_end;
			for (boost::tie(ei, ei_end) = boost::edges(g); ei != ei_end; ++ei)
			{
				boost::put(e_index, *ei, edge_count);
				edge_count++;
			}
		}

		//Test for planarity again; compute the planar embedding as a side-effect
		boost::boyer_myrvold_planarity_test(
			boost::boyer_myrvold_params::graph = g,
			boost::boyer_myrvold_params::embedding = &embedding[0]
		);

		boost::make_maximal_planar(g, &embedding[0]);

		// Re-initialize the edge index, since we just added a few edges
		{
			edges_size_type edge_count = 0;
			edge_iterator ei, ei_end;
			for (boost::tie(ei, ei_end) = boost::edges(g); ei != ei_end; ++ei)
			{
				boost::put(e_index, *ei, edge_count);
				edge_count++;
			}
		}
		

		// Find a canonical ordering
		std::vector<vertex_descriptor> ordering;
		boost::planar_canonical_ordering(g, &embedding[0], std::back_inserter(ordering));

		//assert(ordering.size() > 2, "ordering size too small");

		// Set up a property map to hold the mapping from vertices to coord_t's
		straight_line_drawing_storage_t straight_line_drawing_storage(boost::num_vertices(g));
		straight_line_drawing_t straight_line_drawing(straight_line_drawing_storage.begin(), boost::get(boost::vertex_index, g));

		// Compute the straight line drawing
		boost::chrobak_payne_straight_line_drawing(g, embedding, ordering.begin(), ordering.end(), straight_line_drawing);

		{
			vertex_iterator vi, vend;
			for (boost::tie(vi, vend) = boost::vertices(g); vi != vend; ++vi)
			{
				coord_t coord(boost::get(straight_line_drawing, *vi));
				std::cout << *vi << " -> (" << coord.x << ", " << coord.y << ")" << std::endl;
			}
		}

		// Verify that the drawing is actually a plane drawing
		if (boost::is_straight_line_drawing(g, straight_line_drawing))
			std::cout << "Is a plane drawing." << std::endl;
		else
			std::cout << "Is not a plane drawing." << std::endl;
	}

} // namespace gr