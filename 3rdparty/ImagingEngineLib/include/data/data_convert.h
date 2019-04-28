/*=========================================================================

  Program:   ImagingEngine
  Module:    data_convert.h
  author: 	 zhangjian
  Brief:	 

=========================================================================*/
#pragma once

#include <vtkImageData.h>
#include "data_definition.h"

using namespace std;

namespace DW {
	namespace IMAGE{

		//show buffer is associated with one imaging object
		class DataConvertUtil
		{
		public:
			void PixelDataTovtkImageData(void* in_data, vtkImageData* out_data);
			void vtkImageDataToPixelData(vtkImageData* in_data, void* out_data);
		

		};
	}
}