#ifndef __Box3_h__
#define __Box3_h__

#include "Vector3.h"

/*
 Box3
*/
template<typename Real>
class Box3
{
public:
    Box3();

    void SetCenter(const Vector3<Real>& center);
    Vector3<Real>& GetCenter();
    const Vector3<Real>& GetCenter() const;

    void SetAxis(const uint32_t i, const Vector3<Real>& axis);
    Vector3<Real>& GetAxis(const uint32_t i);
    const Vector3<Real>& GetAxis(const uint32_t i) const;

    void SetExtent(const uint32_t i, const float extent);
    Real& GetExtent(const uint32_t i);
    const Real& GetExtent(const uint32_t i) const;
    Vector3<Real> GetExtents();
    const Vector3<Real> GetExtents() const;

    void ComputeVertices(Vector3<Real> vertices[8]) const;

private:
    Vector3<Real>   _center;
    Vector3<Real>   _basis[3];
    Real            _extent[3];
};

typedef Box3<float> Box3f;
typedef Box3<double> Box3d;

//------------------------------------------------------------------------------
// Box3::Box3
//------------------------------------------------------------------------------
template<typename Real>
inline Box3<Real>::Box3()
{
}

//------------------------------------------------------------------------------
// Box3::SetCenter
//------------------------------------------------------------------------------
template<typename Real>
inline void Box3<Real>::SetCenter(const Vector3<Real>& center)
{
    _center = center;
}

//------------------------------------------------------------------------------
// Box3::GetCenter
//------------------------------------------------------------------------------
template<typename Real>
inline Vector3<Real>& Box3<Real>::GetCenter()
{
    return _center;
}

//------------------------------------------------------------------------------
// Box3::GetCenter
//------------------------------------------------------------------------------
template<typename Real>
inline const Vector3<Real>& Box3<Real>::GetCenter() const
{
    return _center;
}

//------------------------------------------------------------------------------
// Box3::SetAxis
//------------------------------------------------------------------------------
template<typename Real>
inline void Box3<Real>::SetAxis(const uint32_t i, const Vector3<Real>& axis)
{
    assert(i < 3);
    _basis[i] = axis;
}

//------------------------------------------------------------------------------
// Box3::GetAxis
//------------------------------------------------------------------------------
template<typename Real>
inline Vector3<Real>& Box3<Real>::GetAxis(const uint32_t i)
{
    assert(i < 3);
    return _basis[i];
}

//------------------------------------------------------------------------------
// Box3::GetAxis
//------------------------------------------------------------------------------
template<typename Real>
inline const Vector3<Real>& Box3<Real>::GetAxis(const uint32_t i) const
{
    assert(i < 3);
    return _basis[i];
}

//------------------------------------------------------------------------------
// Box3::SetExtent
//------------------------------------------------------------------------------
template<typename Real>
inline void Box3<Real>::SetExtent(const uint32_t i, const float extent)
{
    assert(i < 3);
    _extent[i] = extent;
}

//------------------------------------------------------------------------------
// Box3::GetExtent
//------------------------------------------------------------------------------
template<typename Real>
inline Real& Box3<Real>::GetExtent(const uint32_t i)
{
    assert(i < 3);
    return _extent[i];
}

//------------------------------------------------------------------------------
// Box3::GetExtent
//------------------------------------------------------------------------------
template<typename Real>
inline const Real& Box3<Real>::GetExtent(const uint32_t i) const
{
    assert(i < 3);
    return _extent[i];
}

//------------------------------------------------------------------------------
// Box3::GetExtents
//------------------------------------------------------------------------------
template<typename Real>
inline Vector3<Real> Box3<Real>::GetExtents()
{
    return Vector3<Real>(_extent[0], _extent[1], _extent[2]);
}

//------------------------------------------------------------------------------
// Box3::GetExtents
//------------------------------------------------------------------------------
template<typename Real>
inline const Vector3<Real> Box3<Real>::GetExtents() const
{
    return Vector3<Real>(_extent[0], _extent[1], _extent[2]);
}

//------------------------------------------------------------------------------
// Box3::ComputeVertices
//------------------------------------------------------------------------------
template<typename Real>
inline void Box3<Real>::ComputeVertices(Vector3<Real> vertices[8]) const
{
    // To do.
}

#endif
