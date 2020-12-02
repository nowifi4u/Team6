#pragma once

#include <boost/property_tree/json_parser.hpp>

#include "graph.h"

using boost::property_tree::ptree;

static constexpr uint32_t uint32_max = std::numeric_limits<uint32_t>::max();

template <typename Ty>
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



void importGraph(const boost::property_tree::ptree&, GraphIdx&);


void importGraph(const std::string&, GraphIdx&);

		
