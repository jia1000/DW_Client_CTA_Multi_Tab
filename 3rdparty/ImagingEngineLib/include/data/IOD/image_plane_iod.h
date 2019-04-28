/*=========================================================================

  Program:   ImagingEngine
  Module:    image_plane_iod.h
  author: 	 zhangjian
  Brief:	 

=========================================================================*/
#include "data_definition.h"
#include "iod.h"

using namespace std;

namespace DW {
	namespace IMAGE{
		///
		// ImagePlaneIOD 
		///
		class ImagePlaneIOD : public BaseIOD
		{
		public:
			ImagePlaneIOD();
			virtual ~ImagePlaneIOD();
		protected:

		private:
			string pixel_spacing_;
			string image_orientation_patient_;
			string image_position_patient_;
			float slice_thickness_;
			float slice_position_;
		};
	}
}