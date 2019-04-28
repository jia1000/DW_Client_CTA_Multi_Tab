/*=========================================================================

  Program:   ImagingEngine
  Module:    general_image_iod.h
  author: 	 zhangjian
  Brief:	 

=========================================================================*/
#pragma once

#include "data_definition.h"
#include "iod.h"

using namespace std;

namespace DW {
	namespace IMAGE{
		///
		// GeneralImageIOD
		///
		class GeneralImageIOD : public BaseIOD
		{
		public:
			GeneralImageIOD();
			virtual ~GeneralImageIOD();
		protected:

		private:
			string instance_number_;
			string image_type_;
			
		};
	}
}