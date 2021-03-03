#pragma once

namespace QuadTreeUtility
{
	template<typename T>
	constexpr T sqr(T a) {
		return a * a;
	}

	template<typename T>
	constexpr T pow(T a, std::size_t n) {
		return n == 0 ? 1 : sqr(pow(a, n / 2)) * (n % 2 == 0 ? 1 : a);
	}
}