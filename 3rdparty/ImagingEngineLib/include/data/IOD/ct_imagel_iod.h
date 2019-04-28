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
		// CTImageIOD
		///
		class CTImageIOD : public BaseIOD
		{
		public:
			CTImageIOD();
			virtual ~CTImageIOD();
		protected:

		private:
			string image_type_;
			string samples_per_pixel_;
			string photometric_interpretation_;
			uint16_t bits_allocated_;
			uint16_t bits_stored_;
			uint16_t high_bits_;
			float rescale_intercept_;
			float rescale_slope_;
			string rescale_type_;
			float data_collection_diameter_;
			float data_collection_center_patient_;
			float reconstruction_diameter_;
			float reconstruction_target_center_patient_;
		};
	}
}