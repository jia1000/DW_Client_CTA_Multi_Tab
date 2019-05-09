/*=========================================================================

  Program:   ImagingEngine
  Module:    orientation_facade.h
  author: 	 zhangjian
  Brief:	 

=========================================================================*/
#pragma once

#include "data/data_definition.h"

using namespace std;

namespace DW {
	namespace IMAGE {

		class AnnotationFacade
		{
		public:
			~AnnotationFacade() { instance_ = nullptr; }
		
			static AnnotationFacade* Get() 
			{
				if (instance_ == nullptr)
					instance_ = new AnnotationFacade;
				return instance_;
			}

			/// Get orientation labels of the output image of IThreedImaging
			vector<string> GetOrientationLabels(string control_id);
			/// Convert image position to volume data position
			void ComputeImageToVolume(const float image_pos[2], float volume_pos[3]);
			/// Convert volume data position to image position
			void ComputeVolumeToImage(const float volume_pos[3], float image_pos[2]);
			/// Compute CT value by image position
			int ComputeCTValue(const float image_pos[2]);
			/// Compute CT value by volume data position
			int ComputeCTValue(const float volume_pos[3]);
			/// Compute max/min/average CT value by a set of image positions
			void ComputeCTValueByArea(vector<const float[2]> image_pos_set, float &max_ctv, float &min_ctv, float &avr_ctv);
			/// Compute length by two image positions
			float ComputeLength(const float image_pos1[2], const float image_pos2[2]);
			/// Compute length by two volume data positions
			float ComputeLength(const float volume_pos1[3], const float volume_pos2[3]);
			/// Compute area a set of image positions
			float ComputeArea(vector<const float[2]> image_pos_set);

		private:
			static AnnotationFacade* instance_;
		};

	}
}