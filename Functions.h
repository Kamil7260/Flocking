#pragma once
#define M_PI 3.14159265358979323846
#include <cmath>

template<typename T>
const T& clamp(const T& value, const T& min, const T& max) {
	if (value < min)
		return min;
	if (value > max)
		return max;
	return value;
}

template<typename T>
constexpr T radToDeg(T rad) { return rad * (180.f / M_PI); }

template<typename T>
float vectorAngle(T x, T y) {
	if (x == 0) 
		return (y > 0) ? 90
		: (y == 0) ? 0
		: 270;
	else if (y == 0) 
		return (x >= 0) ? 0
		: 180;
	float ret = radToDeg(atanf((float)y / x));
	if (x < 0 && y < 0) 
		ret = 180 + ret;
	else if (x < 0) 
		ret = 180 + ret; 
	else if (y < 0)
		ret = 270 + (90 + ret);
	return ret;
}

template<typename T>
float vectorAngle(const T& ref) {
	if (ref.x == 0)
		return (ref.y > 0) ? 90
		: (ref.y == 0) ? 0
		: 270;
	else if (ref.y == 0)
		return (ref.x >= 0) ? 0
		: 180;
	float ret = radToDeg(atanf((float)ref.y / ref.x));
	if (ref.x < 0 && ref.y < 0)
		ret = 180 + ret;
	else if (ref.x < 0)
		ret = 180 + ret;
	else if (ref.y < 0)
		ret = 270 + (90 + ret);
	return ret;
}