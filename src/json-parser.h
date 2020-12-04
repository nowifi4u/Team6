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

#define GENERATOR_readJSON_string(func)					\
template <class Ty>	inline								\
auto func##_string##(Ty& val, const std::string& data)	\
{														\
	std::istringstream in(data);						\
	ptree pt;											\
	boost::property_tree::read_json(in, pt);			\
	return Ty::##func##(val, pt);						\
}														\
														\
template <class Ty>	inline								\
auto func##_string##(const std::string& data)			\
{														\
	std::istringstream in(data);						\
	ptree pt;											\
	boost::property_tree::read_json(in, pt);			\
	return Ty::##func##(pt);							\
}														\
														\
template <class Ty>	inline								\
auto func##_file##(Ty& val, const std::string filename)	\
{														\
	std::ifstream in(filename);							\
	ptree pt;											\
	boost::property_tree::read_json(in, pt);			\
	return Ty::##func##(val, pt);						\
}														\
														\
template <class Ty>	inline								\
auto func##_file##(const std::string& filename)			\
{														\
	std::ifstream in(filename);							\
	ptree pt;											\
	boost::property_tree::read_json(in, pt);			\
	return Ty::##func##(pt);							\
}

GENERATOR_readJSON_string(readJSON_Login)
GENERATOR_readJSON_string(readJSON_L0)
GENERATOR_readJSON_string(readJSON_L10)
GENERATOR_readJSON_string(readJSON_L1)
GENERATOR_readJSON_string(updateJSON)


#undef GENERATOR_readJSON_string