/*=========================================================================

  Program:   ImagingEngine
  Module:    imaging_builder_vr.h
  author: 	 zhangjian
  Brief:	 

=========================================================================*/
#pragma once

#include "data_definition.h"
#include "imaging_builder_3d.h"

using namespace DW::IMAGE;

namespace DW {
	namespace Builder{

		class VRImagingBuilder : public IThreedImagingBuilder
		{
		public:
			VRImagingBuilder(void);
			~VRImagingBuilder();
			void BuildData(IDicomReader* data) override;
			void BuildRenderer() override;
			IThreedImaging* GetImaging() override;
		protected:
						
		};
	}
}