#pragma once

#include <boost/dll/runtime_symbol_info.hpp>

static inline std::string WORKING_DIRECTORY = boost::dll::program_location().parent_path().string() + "/";
