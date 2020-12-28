#pragma once

#include <src/Types.h>
#include <src/utils/Logging.h>

#include <nlohmann/json.hpp>
using nlohmann::json;

class base_json_encodable
{
public:

	virtual json encodeJSON() const = 0;
};
