/*=========================================================================

  Program:   ImagingEngine
  Module:    orthogonal_plane.cpp
  author: 	 zhangjian
  Brief:	 

=========================================================================*/
#include "data/orthogonal_plane.h"

using namespace DW::IMAGE;

OrthogonalPlane::OrthogonalPlane(Plane plane)
	: plane_(plane)
{
}

void OrthogonalPlane::GetXYZIndexes(int &x, int &y, int &z) const
{
	x = GetXIndex();
	y = GetYIndex();
	z = GetZIndex();
}

int OrthogonalPlane::GetXIndex() const
{
	switch (plane_)
	{
	case XYPlane:
		return 0;

	case YZPlane:
		return 1;

	case XZPlane:
		return 0;

	default:
		return -1;
	}
}

int OrthogonalPlane::GetYIndex() const
{
	switch (plane_)
	{
	case XYPlane:
		return 1;

	case YZPlane:
		return 2;

	case XZPlane:
		return 2;

	default:
		return -1;
	}
}

int OrthogonalPlane::GetZIndex() const
{
	switch (plane_)
	{
	case XYPlane:
		return 2;

	case YZPlane:
		return 0;

	case XZPlane:
		return 1;

	default:
		return -1;
	}
}

OrthogonalPlane::operator Plane() const
{
	return plane_;
}
