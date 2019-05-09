/*=========================================================================

  Program:   ImagingEngine
  Module:    orthogonal_plane.h
  author: 	 zhangjian
  Brief:	 

=========================================================================*/
#pragma once

namespace DW {
	namespace IMAGE {

		/**
			Class that defines an orthogonal plane.
		 */
		class OrthogonalPlane {

		public:
			/// Definition of the 3 orthogonal planes.
			enum Plane { XYPlane = 2, YZPlane = 0, XZPlane = 1 };

			OrthogonalPlane(Plane plane = XYPlane);

			/// Returns the indices of the coordinate axes corresponding to this orthogonal plane.
			void GetXYZIndexes(int &x, int &y, int &z) const;
			int GetXIndex() const;
			int GetYIndex() const;
			int GetZIndex() const;

			/// Conversion operator: allows to use an OrthogonalPlane in a switch.
			operator Plane() const;

		private:
			Plane plane_;

		};

	}
}
