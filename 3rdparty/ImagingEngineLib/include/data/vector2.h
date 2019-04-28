#pragma once

#include "tools/Common.h"

//==============================================================================
// Template Vector2
//==============================================================================
template<typename Real>
struct Vector2 {
	Vector2();
	Vector2(const Vector2&);
	Vector2(const Real);
	Vector2(const Real, const Real);

	Real& operator[](const int);
	const Real& operator[](const int) const;

	void operator+=(const Vector2&);
	void operator-=(const Vector2&);
	void operator*=(const Real);
	void operator/=(const Real);

	Vector2 operator-() const;

	Vector2 operator+(const Vector2&) const;
	Vector2 operator-(const Vector2&) const;
	Vector2 operator*(const Real) const;
	Vector2 operator/(const Real) const;

	bool operator==(const Vector2&) const;
	bool operator!=(const Vector2&) const;

	Real* Ptr();
	const Real* Ptr() const;

	Real x, y;
};

//==============================================================================
// Type Definitions
//==============================================================================
typedef Vector2<char>           Vector2c;
typedef Vector2<unsigned char>  Vector2uc;
typedef Vector2<short>          Vector2s;
typedef Vector2<unsigned short> Vector2us;
typedef Vector2<int>            Vector2i;
typedef Vector2<unsigned int>   Vector2ui;
typedef Vector2<float>          Vector2f;
typedef Vector2<double>         Vector2d;

//==============================================================================
// Global Methods
//==============================================================================
template<typename Real>
void Negate(Vector2<Real>&, const Vector2<Real>&);

template<typename Real>
void Add(Vector2<Real>&, const Vector2<Real>&, const Vector2<Real>&);

template<typename Real>
void Subtract(Vector2<Real>&, const Vector2<Real>&, const Vector2<Real>&);

template<typename Real>
void Scale(Vector2<Real>&, const Vector2<Real>&, const Real);

template<typename Real>
void ScaleAdd(Vector2<Real>&,
			  const Vector2<Real>&,
			  const Vector2<Real>&,
			  const Real);

template<typename Real>
bool Compare(const Vector2<Real>&, const Vector2<Real>&);

template<typename Real>
Real Length(const Vector2<Real>&);

template<typename Real>
Real InverseLength(const Vector2<Real>&);

template<typename Real>
Real LengthSquared(const Vector2<Real>&);

template<typename Real>
Real Dot(const Vector2<Real>&, const Vector2<Real>&);

template<typename Real>
void Normalize(Vector2<Real>&);

template<typename Real>
void Lerp(Vector2<Real>&,
		  const Vector2<Real>&,
		  const Vector2<Real>&,
		  const Real);

template<typename Real>
void Hermite(Vector2<Real>&,
			 const Vector2<Real>&,
			 const Vector2<Real>&,
			 const Vector2<Real>&,
			 const Vector2<Real>&,
			 const Real);

template<typename Real>
void CatmullRom(Vector2<Real>&,
				const Vector2<Real>&,
				const Vector2<Real>&,
				const Vector2<Real>&,
				const Vector2<Real>&,
				const Real);

template<typename Real>
void Barycentric(Vector2<Real>&,
				 const Vector2<Real>&,
				 const Vector2<Real>&,
				 const Vector2<Real>&,
				 const Real,
				 const Real);

template<typename Real>
inline
	Vector2<Real>::Vector2()
	: x(0), y(0)
{
}

template<typename Real>
inline
	Vector2<Real>::Vector2(const Vector2<Real>& v)
	: x(v.x), y(v.y)
{
}

template<typename Real>
inline
	Vector2<Real>::Vector2(const Real s)
	: x(s), y(s)
{
}

template<typename Real>
inline
	Vector2<Real>::Vector2(const Real x0, const Real y0)
	: x(x0), y(y0)
{
}

template<typename Real>
inline Real&
	Vector2<Real>::operator[](const int i)
{
	assert(i >= 0 && i <= 1);
	return *(&x + i);
}

template<typename Real>
inline const Real&
	Vector2<Real>::operator[](const int i) const
{
	assert(i >= 0 && i <= 1);
	return *(&x + i);
}

template<typename Real>
inline void
	Vector2<Real>::operator+=(const Vector2<Real>& v)
{
	x += v.x;
	y += v.y;
}

template<typename Real>
inline void
	Vector2<Real>::operator-=(const Vector2<Real>& v)
{
	x -= v.x;
	y -= v.y;
}

template<typename Real>
inline void
	Vector2<Real>::operator*=(const Real s)
{
	x *= s;
	y *= s;
}

template<typename Real>
inline void
	Vector2<Real>::operator/=(const Real s)
{
	Real sInv = Real(1) / s;
	x *= sInv;
	y *= sInv;
}

template<typename Real>
inline Vector2<Real>
	Vector2<Real>::operator-() const
{
	return Vector2(-x, -y);
}

template<typename Real>
inline Vector2<Real>
	Vector2<Real>::operator+(const Vector2<Real>& v) const
{
	return Vector2(x + v.x, y + v.y);
}

template<typename Real>
inline Vector2<Real>
	Vector2<Real>::operator-(const Vector2<Real>& v) const
{
	return Vector2(x - v.x, y - v.y);
}

template<typename Real>
inline Vector2<Real>
	Vector2<Real>::operator*(const Real s) const
{
	return Vector2(s * x, s * y);
}

template<typename Real>
inline Vector2<Real>
	Vector2<Real>::operator/(const Real s) const
{
	Real sInv = Real(1) / s;
	return Vector2(sInv * x, sInv * y);
}

template<typename Real>
inline Vector2<Real>
	operator*(const Real s, const Vector2<Real>& v)
{
	return Vector2<Real>(s * v.x, s * v.y);
}

template<typename Real>
inline bool
	Vector2<Real>::operator==(const Vector2<Real>& v) const
{
	return false;
	//return FloatUtil<Real>::Compare(x, v.x) &&
	//     FloatUtil<Real>::Compare(y, v.y);
}

template<typename Real>
inline bool
	Vector2<Real>::operator!=(const Vector2<Real>& v) const
{
	return !(*this == v);
}

template<typename Real>
inline Real*
	Vector2<Real>::Ptr()
{
	return &x;
}

template<typename Real>
inline const Real*
	Vector2<Real>::Ptr() const
{
	return &x;
}

template<typename Real>
inline void
	Negate(Vector2<Real>& r, const Vector2<Real>& v)
{
	r.x = -v.x;
	r.y = -v.y;
}

template<typename Real>
inline void
	Add(Vector2<Real>& r, const Vector2<Real>& u, const Vector2<Real>& v)
{
	r.x = u.x + v.x;
	r.y = u.y + v.y;
}

template<typename Real>
inline void
	Subtract(Vector2<Real>& r, const Vector2<Real>& u, const Vector2<Real>& v)
{
	r.x = u.x - v.x;
	r.y = u.y - v.y;
}

template<typename Real>
inline void
	Scale(Vector2<Real>& r, const Vector2<Real>& u, const Real s)
{
	r.x = u.x * s;
	r.y = u.y * s;
}

template<typename Real>
inline void
	ScaleAdd(Vector2<Real>& r,
	const Vector2<Real>& u,
	const Vector2<Real>& v,
	const Real s)
{
	r.x = u.x + s * v.x;
	r.y = u.y + s * v.y;
}

template<typename Real>
inline bool
	Compare(const Vector2<Real>& u, const Vector2<Real>& v)
{
	return false;
	//return FloatUtil<Real>::Compare(u.x, v.x) &&
	//     FloatUtil<Real>::Compare(v.y, v.y);
}

template<typename Real>
inline Real
	Length(const Vector2<Real>& v)
{
	return Real(sqrt(v.x*v.x + v.y*v.y));
	//return FloatUtil<Real>::Sqrt(v.x * v.x + v.y * v.y);
}

template<typename Real>
inline Real
	InverseLength(const Vector2<Real>& v)
{
	return Real(1.0/sqrt(v.x*v.x+v.y*v.y));
	//return FloatUtil<Real>::InvSqrt(v.x * v.x + v.y * v.y);
}

template<typename Real>
inline Real
	LengthSquared(const Vector2<Real>& v)
{
	return v.x * v.x + v.y * v.y;
}

template<typename Real>
inline Real
	Dot(const Vector2<Real>& u, const Vector2<Real>& v)
{
	return u.x * v.x + u.y * v.y;
}

template<typename Real>
inline void
	Normalize(Vector2<Real>& v)
{
	Real invLen = InverseLength(v);
	v.x *= invLen;
	v.y *= invLen;
}

template<typename Real>
inline void
	Lerp(Vector2<Real>& r,
	const Vector2<Real>& u,
	const Vector2<Real>& v,
	const Real s)
{
	r.x = u.x + s * (v.x - u.x);
	r.y = u.y + s * (v.y - u.y);
}