/*=========================================================================

  Program:   ImagingEngine
  Module:    image_plane.h
  author: 	 zhangjian
  Brief:	 The ImagePlane class represents the bounded plane of an image.
			 The plane stores the image orientation, origin, spacing, 
			 row and column length, and thickness of the image.

=========================================================================*/
#pragma once

#include "data/data_definition.h"
#include "data/vector3.h"
#include "data/image_orientation.h"

namespace DW {
	namespace IMAGE {

		class ImagePlane {

		public:
			ImagePlane();

			/// Returns the image plane orientation.
			const ImageOrientation& GetImageOrientation() const;
			/// Sets the image plane orientation.
			void SetImageOrientation(ImageOrientation imageOrientation);

			/// Returns the top left corner of the plane.
			const Point3f& GetOrigin() const;
			/// Sets the top left corner of the plane.
			void SetOrigin(Point3f origin);
			/// Sets the top left corner of the plane.
			void SetOrigin(float x, float y, float z);

			/// Returns the pixel spacing.
			void GetSpacing(double& x, double& y);
			/// Sets the pixel spacing.
			void SetSpacing(double x, double y);

			/// Returns the plane width.
			float GetRowLength() const;
			/// Sets the plane width.
			void SetRowLength(float length);

			/// Returns the plane height.
			float GetColumnLength() const;
			/// Sets the plane height.
			void SetColumnLength(float length);

			/// Returns the plane thickness.
			float GetThickness() const;
			/// Sets the plane thickness.
			void SetThickness(float thickness);

			/// Returns the plane center.
			Point3f GetCenter() const;
			/// Modifies the origin so that the given point becomes the center.
			void SetCenter(const Vector3f& center);
			/// Modifies the origin so that the given point becomes the center.
			void SetCenter(float x, float y, float z);

			bool operator ==(const ImagePlane& plane) const;
			bool operator !=(const ImagePlane& plane) const;

			/// Location of the bounds. Central = middle of the plane, Upper = middle of the plane + thickness/2, Lower = middle of the plane - thickness/2.
			enum CornersLocation { Central, Upper, Lower };

			/// Contains the 4 corners of an ImagePlane at the specified location.
			struct Corners
			{
				CornersLocation location;
				Point3f topLeft, topRight, bottomRight, bottomLeft;
			};

			/// Returns the corners of the plane in the specified location.
			Corners GetCorners(CornersLocation location) const;

			/// Computes the intersection between this ImagePlane and \a planeToIntersect.
			/// \param plane Plane to intersect with this one. It's considered an infinite plane.
			/// \param intersectionPoint1 The first intersection point will be stored here.
			/// \param intersectionPoint2 The second intersection point will be stored here.
			/// \param cornersLocation Specify which corners of this ImagePlane will be used to compute the intersection (upper by default)
			///                        (\a plane is always used with the central location).
			/// \return True if there are intersections, and false otherwise.
			bool GetIntersections(const ImagePlane* plane, Point3f& intersectionPoint1, Point3f& intersectionPoint2, CornersLocation cornersLocation = Upper) const;

			/// Returns the distance from the given point to this ImagePlane.
			double GetDistanceToPoint(const Point3f& point) const;

			/// Projects the given point onto this ImagePlane and returns the result.
			/// The projection consists in changing the coordinates so that the ImagePlane origin is the new (0,0,0),
			/// and the row and column vector the new X and Y axes, respectively.
			Point3f ProjectPoint(Point3f point, bool vtkReconstructionHack = false) const;

		private:
			/// The image plane orientation.
			ImageOrientation image_orientation_;
			/// Top left corner of the plane.
			Point3f origin_;
			/// Pixel spacing.
			double spacing_x, spacing_y;
			/// Plane width.
			double row_length_;
			/// Plane height.
			double column_length_;
			/// Plane thickness.
			double thickness_;

		};

	}
}