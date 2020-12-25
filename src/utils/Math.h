#pragma once

namespace Math {

	template <class Ty>
	inline constexpr Ty sqr(const Ty& val)
	{
		return val * val;
	}

	template <class Ty>
	inline constexpr Ty min(const Ty& a, const Ty& b)
	{
		return (a < b) ? a : b;
	}

	template <class Ty>
	inline constexpr Ty max(const Ty& a, const Ty& b)
	{
		return (a > b) ? a : b;
	}

} // namespace Math
