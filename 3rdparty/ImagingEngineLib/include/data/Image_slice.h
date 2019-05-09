/*=========================================================================

  Program:   ImagingEngine
  Module:    ImageSlice.h
  author: 	 zhangjian
  Brief:	 

=========================================================================*/
#pragma once

#include "data/data_definition.h"
#include "vector3.h"
#include "data/patient_orientation.h"

using namespace std;

namespace DW {
	namespace IMAGE {

		class ImageSlice
		{
		public:
		protected:
		private:
			/// SOPInstanceUID (0008.0018)
			string sop_instance_uid_;
			/// ---------------------------------------------------------------------------
			/// General Information about the image. C. 7.6 General Image Module-PS 3.3.

			/// The Number that identifies the image. (0020.0013) Type 2
			string instance_number_;
			/// Anatomical Orientation of the image's rows and columns (LR/AP/HF). Required If the image does not require Image Orientation (Patient) (0020,0037) and Image Position (Patient) (0020,0032). See C. 6.7.1.1.1. (0020.0020) Type 2C.
			PatientOrientation patient_orientation_;
			/// pixel spacing
			double pixel_spacing_[2];
			/// slice thickness
			double slice_thickness_;
			/// rows & columns
			int rows_, columns_;
			/// Image Type
			string image_type_;
		};

	}
}