#pragma once

#include <boost/property_tree/json_parser.hpp>
#include <sstream>
#include <fstream>

using boost::property_tree::ptree;


[[nodiscard]] inline
size_t ptree_array_size(const ptree& pt, const ptree::key_type& key)
{
	return pt.get_child(key).size();
}

template <class Func> inline
void ptree_array_foreach(const ptree& pt, const ptree::key_type& key, Func f)
{
	for (const auto& item : pt.get_child(key))
	{
		f(item.second);
	}
}

template <typename Ty> inline
std::vector<Ty> ptree_as_vector(const ptree& pt, const ptree::key_type& key)
{
	std::vector<Ty> res;
	res.reserve(ptree_array_size(pt, key));
	ptree_array_foreach(pt, key, [&res](const ptree& ipt) {
		res.push_back(ipt.get_value<Ty>());
		});
	return res;
}

template <class Ty, class Func> inline
auto ptree_from_string(const std::string& data, Func f)
{
	std::istringstream in(data);
	ptree pt;
	boost::property_tree::read_json(in, pt);
	return f(pt);
}

template <class Func> inline
auto ptree_from_file(const std::string& filename, Func f)
{
	ptree pt;
	boost::property_tree::read_json(filename, pt);
	return f(pt);
}