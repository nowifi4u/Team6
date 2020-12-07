#pragma once

#include <cstdint>
#include <string>

namespace Types
{

	using tick_t = int64_t;

	using vertex_idx_t = uint32_t;
	using edge_idx_t = uint32_t;

	using player_uid_t = std::string;
	using train_idx_t = uint32_t;
	using post_idx_t = uint32_t;
	using edge_length_t = uint32_t;

	using product_t = uint64_t;
	using population_t = uint32_t;
	using armor_t = uint32_t;

	using position_t = int32_t;

} // namespace Types