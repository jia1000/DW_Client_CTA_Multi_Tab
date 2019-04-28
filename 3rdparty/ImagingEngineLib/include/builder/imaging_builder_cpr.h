/*=========================================================================

  Program:   ImagingEngine
  Module:    imaging_builder_cpr.h
  author: 	 zhangjian
  Brief:	 

=========================================================================*/
#pragma once

#include "data_definition.h"
#include "imaging_builder_3d.h"

using namespace DW::IMAGE;

namespace DW {
	namespace Builder{

		class CPRImagingBuilder : public IThreedImagingBuilder
		{
		public:
			CPRImagingBuilder(void);
			~CPRImagingBuilder();
			void BuildData(IDicomReader* data) override;
			void BuildRenderer() override;
			IThreedImaging* GetImaging() override;
		protected:
						
		};
	}
}