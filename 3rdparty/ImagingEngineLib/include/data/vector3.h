#pragma once

#include "tools/Common.h"

//==============================================================================
// Template Vector3
//==============================================================================
template<typename Real>
struct Vector3 {
	Vector3();
	Vector3(const Vector3&);
	Vector3(const Real);
	Vector3(const Real, const Real, const Real);

	Real& operator[](const uint32_t);
	const Real& operator[](const uint32_t) const;

	void operator+=(const Vector3&);
	void operator-=(const Vector3&);
	void operator*=(const Real);
	void operator/=(const Real);

	Vector3 operator-() const;

	Vector3 operator+(const Vector3&) const;
	Vector3 operator-(const Vector3&) const;
	Vector3 operator*(const Real) const;
	Vector3 operator/(const Real) const;

	template<typename T>
	friend Vector3 operator*(const Real, const Vector3&);

	bool operator==(const Vector3&) const;
	bool operator!=(const Vector3&) const;

	// Returns the vector module.
	Real length() const;

	Real* Ptr();
	const Real* Ptr() const;

	Real x, y, z;
};

//==============================================================================
// Type Definitions
//==============================================================================
typedef Vector3<char>           Vector3c;
typedef Vector3<unsigned char>  Vector3uc;
typedef Vector3<short>          Vector3s;
typedef Vector3<unsigned short> Vector3us;
typedef Vector3<int>            Vector3i;
typedef Vector3<unsigned int>   Vector3ui;
typedef Vector3<float>          Vector3f;
typedef Vector3<double>         Vector3d;

//==============================================================================
// Global Methods
//==============================================================================
template<typename Real>
Vector3<Real> operator*(const Real, const Vector3<Real>&);

template<typename Real>
void Negate(Vector3<Real>&, const Vector3<Real>&);

template<typename Real>
void Add(Vector3<Real>&, const Vector3<Real>&, const Vector3<Real>&);

template<typename Real>
void Subtract(Vector3<Real>&, const Vector3<Real>&, const Vector3<Real>&);

template<typename Real>
void Scale(Vector3<Real>&, const Vector3<Real>&, const Real);

template<typename Real>
void ScaleAdd(Vector3<Real>&,
			  const Vector3<Real>&,
			  const Vector3<Real>&,
			  const Real);

template<typename Real>
bool Compare(const Vector3<Real>&, const Vector3<Real>&);

template<typename Real>
Real Length(const Vector3<Real>&);

template<typename Real>
Real LengthSquared(const Vector3<Real>&);

template<typename Real>
Real Dot(const Vector3<Real>&, const Vector3<Real>&);

template<typename Real>
void Cross(Vector3<Real>&, const Vector3<Real>&, const Vector3<Real>&);

template<typename Real>
void Normalize(Vector3<Real>&);

template<typename Real>
void Lerp(Vector3<Real>&,
		  const Vector3<Real>&,
		  const Vector3<Real>&,
		  const Real);

template<typename Real>
void Hermite(Vector3<Real>&,
			 const Vector3<Real>&,
			 const Vector3<Real>&,
			 const Vector3<Real>&,
			 const Vector3<Real>&,
			 const Real);

template<typename Real>
void CatmullRom(Vector3<Real>&,
				const Vector3<Real>&,
				const Vector3<Real>&,
				const Vector3<Real>&,
				const Vector3<Real>&,
				const Real);

template<typename Real>
void Barycentric(Vector3<Real>&,
				 const Vector3<Real>&,
				 const Vector3<Real>&,
				 const Vector3<Real>&,
				 const Real,
				 const Real);

//------------------------------------------------------------------------------
// Vector3::Vector3
//------------------------------------------------------------------------------
template<typename Real>
inline
	Vector3<Real>::Vector3()
	: x(Real(0)), y(Real(0)), z(Real(0))
{
}

//------------------------------------------------------------------------------
// Vector3::Vector3
//------------------------------------------------------------------------------
template<typename Real>
inline
	Vector3<Real>::Vector3(const Vector3& v)
	: x(v.x), y(v.y), z(v.z)
{
}

//------------------------------------------------------------------------------
// Vector3::Vector3
//------------------------------------------------------------------------------
template<typename Real>
inline
	Vector3<Real>::Vector3(const Real s)
	: x(s), y(s), z(s)
{
}

//------------------------------------------------------------------------------
// Vector3::Vector3
//------------------------------------------------------------------------------
template<typename Real>
inline
	Vector3<Real>::Vector3(const Real x0, const Real y0, const Real z0)
	: x(x0), y(y0), z(z0)
{
}

//------------------------------------------------------------------------------
// Vector3::operator[]
//------------------------------------------------------------------------------
template<typename Real>
inline Real&
	Vector3<Real>::operator[](const uint32_t i)
{
	assert(i <= 2);
	return *(&x + i);
}

//------------------------------------------------------------------------------
// Vector3::operator[]
//------------------------------------------------------------------------------
template<typename Real>
inline const Real&
	Vector3<Real>::operator[](const uint32_t i) const
{
	assert(i <= 2);
	return *(&x + i);
}

//------------------------------------------------------------------------------
// Vector3::operator+=
//------------------------------------------------------------------------------
template<typename Real>
inline void
	Vector3<Real>::operator+=(const Vector3& v)
{
	x += v.x;
	y += v.y;
	z += v.z;
}

//------------------------------------------------------------------------------
// Vector3::operator-=
//------------------------------------------------------------------------------
template<typename Real>
inline void
	Vector3<Real>::operator-=(const Vector3& v)
{
	x -= v.x;
	y -= v.y;
	z -= v.z;
}

//------------------------------------------------------------------------------
// Vector3::operator*=
//------------------------------------------------------------------------------
template<typename Real>
inline void
	Vector3<Real>::operator*=(const Real s)
{
	x *= s;
	y *= s;
	z *= s;
}

//------------------------------------------------------------------------------
// Vector3::operator/=
//------------------------------------------------------------------------------
template<typename Real>
inline void
	Vector3<Real>::operator/=(const Real s)
{
	Real sInv = Real(1) / s;
	x *= sInv;
	y *= sInv;
	z *= sInv;
}

//------------------------------------------------------------------------------
// Vector3::operator-
//------------------------------------------------------------------------------
template<typename Real>
inline Vector3<Real>
	Vector3<Real>::operator-() const
{
	return Vector3(-x, -y, -z);
}

//------------------------------------------------------------------------------
// Vector3::operator+
//------------------------------------------------------------------------------
template<typename Real>
inline Vector3<Real>
	Vector3<Real>::operator+(const Vector3& v) const
{
	return Vector3(x + v.x, y + v.y, z + v.z);
}

//------------------------------------------------------------------------------
// Vector3::operator-
//------------------------------------------------------------------------------
template<typename Real>
inline Vector3<Real>
	Vector3<Real>::operator-(const Vector3& v) const
{
	return Vector3(x - v.x, y - v.y, z - v.z);
}

//------------------------------------------------------------------------------
// Vector3::operator*
//------------------------------------------------------------------------------
template<typename Real>
inline Vector3<Real>
	Vector3<Real>::operator*(const Real s) const
{
	return Vector3(s * x, s * y, s * z);
}

//------------------------------------------------------------------------------
// Vector3::operator/
//------------------------------------------------------------------------------
template<typename Real>
inline Vector3<Real>
	Vector3<Real>::operator/(const Real s) const
{
	Real sInv = Real(1) / s;
	return Vector3(sInv * x, sInv * y, sInv * z);
}

//------------------------------------------------------------------------------
// Vector3::operator==
//------------------------------------------------------------------------------
template<typename Real>
inline bool
	Vector3<Real>::operator==(const Vector3<Real>& v) const
{
	return Abs(x - v.x) < MathTool::kEpsilon &&
		Abs(y - v.y) < MathTool::kEpsilon &&
		Abs(z - v.z) < MathTool::kEpsilon;
}

//------------------------------------------------------------------------------
// Vector3::operator!=
//------------------------------------------------------------------------------
template<typename Real>
inline bool
	Vector3<Real>::operator!=(const Vector3<Real>& v) const
{
	return !(*this == v);
}

//------------------------------------------------------------------------------
// Vector3::Ptr
//------------------------------------------------------------------------------
template<typename Real>
inline Real*
	Vector3<Real>::Ptr()
{
	return &x;
}

//------------------------------------------------------------------------------
// Vector3::Ptr
//------------------------------------------------------------------------------
template<typename Real>
inline const Real*
	Vector3<Real>::Ptr() const
{
	return &x;
}

//------------------------------------------------------------------------------
// Vector3::length
//------------------------------------------------------------------------------
template<typename Real>
inline Real Vector3<Real>::length() const
{
	return sqrt(x * x + y * y + z * z);
}

//------------------------------------------------------------------------------
// operator *
//------------------------------------------------------------------------------
template<typename Real>
inline Vector3<Real>
	operator*(const Real s, const Vector3<Real>& v)
{
	return Vector3<Real>(s * v.x, s * v.y, s * v.z);
}

//------------------------------------------------------------------------------
// Negate
//------------------------------------------------------------------------------
template<typename Real>
inline void
	Negate(Vector3<Real>& r, const Vector3<Real>& v)
{
	r.x = -v.x;
	r.y = -v.y;
	r.z = -v.z;
}

//------------------------------------------------------------------------------
// Add
//------------------------------------------------------------------------------
template<typename Real>
inline void
	Add(Vector3<Real>& r, const Vector3<Real>& u, const Vector3<Real>& v)
{
	r.x = u.x + v.x;
	r.y = u.y + v.y;
	r.z = u.z + v.z;
}

//------------------------------------------------------------------------------
// Subtract
//------------------------------------------------------------------------------
template<typename Real>
inline void
	Subtract(Vector3<Real>& r, const Vector3<Real>& u, const Vector3<Real>& v)
{
	r.x = u.x - v.x;
	r.y = u.y - v.y;
	r.z = u.z - v.z;
}

//------------------------------------------------------------------------------
// Scale
//------------------------------------------------------------------------------
template<typename Real>
inline void
	Scale(Vector3<Real>& r, const Vector3<Real>& u, const Real s)
{
	r.x = u.x * s;
	r.y = u.y * s;
	r.z = u.z * s;
}

//------------------------------------------------------------------------------
// ScaleAdd
//------------------------------------------------------------------------------
template<typename Real>
inline void
	ScaleAdd(Vector3<Real>& r,
	const Vector3<Real>& u,
	const Vector3<Real>& v,
	const Real s)
{
	r.x = u.x + s * v.x;
	r.y = u.y + s * v.y;
	r.z = u.z + s * v.z;
}

//------------------------------------------------------------------------------
// Compare
//------------------------------------------------------------------------------
template<typename Real>
inline bool
	Compare(const Vector3<Real>& u, const Vector3<Real>& v)
{
	return Abs(u.x - v.x) < MathTool::kEpsilon &&
		Abs(u.y - v.y) < MathTool::kEpsilon &&
		Abs(u.z - v.z) < MathTool::kEpsilon;
}

//------------------------------------------------------------------------------
// Length
//------------------------------------------------------------------------------
template<typename Real>
inline Real
	Length(const Vector3<Real>& v)
{
	return Real(sqrt(v.x*v.x + v.y*v.y + v.z*v.z));
	//return FloatUtil<Real>::Sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

//------------------------------------------------------------------------------
// InverseLength
//------------------------------------------------------------------------------
template<typename Real>
inline Real
	InverseLength(const Vector3<Real>& v)
{
	return Real(1.0/sqrt(v.x*v.x+v.y*v.y+v.z*v.z));
	//return FloatUtil<Real>::InverseSqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

//------------------------------------------------------------------------------
// LengthSquared
//------------------------------------------------------------------------------
template<typename Real>
inline Real
	LengthSquared(const Vector3<Real>& v)
{
	return v.x * v.x + v.y * v.y + v.z * v.z;
}

//------------------------------------------------------------------------------
// Dot
//------------------------------------------------------------------------------
template<typename Real>
inline Real
	Dot(const Vector3<Real>& u, const Vector3<Real>& v)
{
	return u.x * v.x + u.y * v.y + u.z * v.z;
}

//------------------------------------------------------------------------------
// Cross
//------------------------------------------------------------------------------
template<typename Real>
inline void
	Cross(Vector3<Real>& r, const Vector3<Real>& u, const Vector3<Real>& v)
{
	r.x = u.y * v.z - u.z * v.y;
	r.y = u.z * v.x - u.x * v.z;
	r.z = u.x * v.y - u.y * v.x;
}

//------------------------------------------------------------------------------
// Normalize
//------------------------------------------------------------------------------
template<typename Real>
inline void
	Normalize(Vector3<Real>& v)
{
	Real invLen = InverseLength(v);
	v.x *= invLen;
	v.y *= invLen;
	v.z *= invLen;
}

//------------------------------------------------------------------------------
// Lerp
//------------------------------------------------------------------------------
template<typename Real>
inline void
	Lerp(Vector3<Real>& r,
	const Vector3<Real>& u,
	const Vector3<Real>& v,
	const Real s)
{
	r.x = u.x + s * (v.x - u.x);
	r.y = u.y + s * (v.y - u.y);
	r.z = u.z + s * (v.z - u.z);
}