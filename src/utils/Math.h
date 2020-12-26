#pragma once

#include <cmath>

namespace Math {

	template <class Ty>
	inline constexpr Ty sqr(const Ty& val)
	{
		return val * val;
	}

	template <class Ty>
	inline constexpr Ty distance(const Ty& a, const Ty& b)
	{
		std::sqrt(a * a + b * b);
	}

} // namespace Math
