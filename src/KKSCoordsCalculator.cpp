#include "KKSCoordsCalculator.h"

void KKSCoordsCalculator::calculate(Graph& g, double topology_width, double topology_height, double unit_edge_length)
{
	PositionVec position_vec(num_vertices(g));
	PositionMap position(position_vec.begin(), get(boost::vertex_index, g));
	boost::minstd_rand gen;
	topology_type topo(gen, 0, 0, topology_width, topology_width);

	circle_graph_layout(g, position, std::min(topology_width, topology_height));

	bool kamada = kamada_kawai_spring_layout(
		g,
		position,
		boost::make_transform_value_property_map(to_double(), get(&EdgeProperties::length, g)),
		topo,
		boost::edge_length(unit_edge_length),
		KamadaKawaiDone()
	);


	m_position_vec = std::move(position_vec);
	m_position = boost::move(position);
}