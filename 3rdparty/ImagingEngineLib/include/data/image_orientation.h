/*=========================================================================

  Program:   ImagingEngine
  Module:    image_orientation.h
  author: 	 zhangjian
  Brief:	 

=========================================================================*/
#pragma once

#include "data/data_definition.h"
#include "data/vector3.h"

using namespace std;

namespace DW {
	namespace IMAGE {

		/// This class Encaptures the attribute DICOM Image Orientation (Patient) (0020.0037) which defines the direction of the vectors of the first row and column
		/// Of the image with respect to the patient. For more information see PS 3.3, Section C. 7.6.2.1.1.
		/// In DICOM Format This attribute consists of 6 values separated by ' \ ', being the first 3 the vector of the row and the last 3 the vector of the column.
		/// This class also saves the normal plan that comprises these two vectors.
		class ImageOrientation
		{
		public:
			ImageOrientation(const string& orientation);
			ImageOrientation(const Vector3d& rowVector, const Vector3d& columnVector);
			~ImageOrientation();

			/// row and column vector
			Vector3d GetRowVector() const;
			Vector3d GetColumnVector() const;
			Vector3d GetNormalVector() const;
			/// Set orientation string of dicom format
			/// Assigns the orientation provided in the format stipulated by the DICOM: 6 numeric values separated by ' \ ' or an empty string.
			/// If the string is not in the expected format, it re - initialize the vector values and is returned false, true otherwise.
			bool SetImageOrientationString(const string &image_orientation);
			/// Get orientation string as dicom format
			string GetImageOrientationAsString() const;
			/// Initialize vectors to (0, 0, 0)
			void SetVectorValuesToDefault();
			/// Assigns the orientation through the 2 3D vectors in the row and column directions respectively
			void SetRowAndColumnVectors(const Vector3d &row_vector, const Vector3d &column_vector);
			/// Equality Operator
			bool operator==(const ImageOrientation& image_orientation) const;

		private:
			Vector3d row_vector_;
			Vector3d column_vector_;
			Vector3d normal_vector_;

		};
		
	}
}
