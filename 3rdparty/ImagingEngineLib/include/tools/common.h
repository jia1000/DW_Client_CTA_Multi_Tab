#pragma once

#include <cassert>
#include <cmath>

typedef unsigned char		uint8_t;
typedef unsigned short		uint16_t;
typedef unsigned int		uint32_t;
typedef unsigned long long	uint64_t;

template<typename T> void Swap(T &a, T &b);
template<typename T> T Abs(const T &t);
template<typename T> T Max(const T &a, const T &b);
void Lerp(float& r, const float a, const float b, const float s);
uint16_t ShortSwap(const uint16_t i);
uint32_t LongSwap(const uint32_t i);
uint64_t LongLongSwap(const uint64_t i);
float FloatSwap(const float f);
float DoubleSwap(const float d);

//------------------------------------------------------------------------------
// Swap
//------------------------------------------------------------------------------
template<typename T>
inline void
	Swap(T& a, T& b)
{
	T t(a);
	a = b;
	b = a;
}

//------------------------------------------------------------------------------
// Abs
//------------------------------------------------------------------------------
template<typename T>
inline T Abs(const T &t)
{
	return t < T(0) ? -t : t;
}

//------------------------------------------------------------------------------
// Max
//------------------------------------------------------------------------------
template<typename T>
inline T Max(const T &a, const T &b)
{
	return a > b ? a : b;
}

//------------------------------------------------------------------------------
// Lerp
//------------------------------------------------------------------------------
inline void Lerp(float& r, const float a, const float b, const float s)
{
	r = a + s * (b - a);
}

//------------------------------------------------------------------------------
// SwapShort
//------------------------------------------------------------------------------
inline uint16_t
	ShortSwap(const uint16_t i)
{
	union
	{
		uint16_t	i;
		uint8_t		b[4];
	} one, two;

	one.i = i;
	two.b[0] = one.b[1];
	two.b[1] = one.b[0];
	return two.i;
}

//------------------------------------------------------------------------------
// LongSwap
//------------------------------------------------------------------------------
inline uint32_t
	LongSwap(const uint32_t i)
{
	union
	{
		uint32_t	i;
		uint8_t		b[4];
	} one, two;

	one.i = i;
	two.b[0] = one.b[3];
	two.b[1] = one.b[2];
	two.b[2] = one.b[1];
	two.b[3] = one.b[0];
	return two.i;
}

//------------------------------------------------------------------------------
// LongLongSwap
//------------------------------------------------------------------------------
inline uint64_t
	LongLongSwap(const uint64_t i)
{
	union
	{
		uint64_t	i;
		uint8_t		b[8];
	} one, two;

	one.i = i;
	two.b[0] = one.b[7];
	two.b[1] = one.b[6];
	two.b[2] = one.b[5];
	two.b[3] = one.b[4];
	two.b[4] = one.b[3];
	two.b[5] = one.b[2];
	two.b[6] = one.b[1];
	two.b[7] = one.b[0];
	return two.i;
}

//------------------------------------------------------------------------------
// FloatSwap
//------------------------------------------------------------------------------
inline float
	FloatSwap(const float f)
{
	union
	{
		float f;
		uint8_t b[4];
	} one, two;

	one.f = f;
	two.b[0] = one.b[3];
	two.b[1] = one.b[2];
	two.b[2] = one.b[1];
	two.b[3] = one.b[0];
	return two.f;
}

//------------------------------------------------------------------------------
// DoubleSwap
//------------------------------------------------------------------------------
inline float
	DoubleSwap(const float d)
{
	union
	{
		float		d;
		uint8_t		b[8];
	} one, two;

	one.d = d;
	two.b[0] = one.b[7];
	two.b[1] = one.b[6];
	two.b[2] = one.b[5];
	two.b[3] = one.b[4];
	two.b[4] = one.b[3];
	two.b[5] = one.b[2];
	two.b[6] = one.b[1];
	two.b[7] = one.b[0];
	return two.d;
}