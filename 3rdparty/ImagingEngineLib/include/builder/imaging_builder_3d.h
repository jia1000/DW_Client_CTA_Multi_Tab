/*=========================================================================

  Program:   ImagingEngine
  Module:    imaging_builder_3d.h
  author: 	 zhangjian
  Brief:	 构造IThreedImaging对象的类

=========================================================================*/
#pragma once

#include "data_definition.h"
#include "vol_data.h"
#include "imaging_3d.h"
#include "render_param.h"
#include "io/dcm_reader.h"

using namespace DW::IMAGE;
using namespace DW::Render;
using namespace DW::IO;

namespace DW {
	namespace Builder{

		///
		// define a interface to hanlder constructing imaging objects
		///
		class IThreedImagingBuilder
		{
		public:
			virtual void BuildData(IDicomReader* data) = 0;
			virtual void BuildRenderer(string param = "") = 0;
			//virtual void BuildWidget();
			//virtual void BuildEvent();
			virtual IThreedImaging* GetImaging() = 0;
		protected:
			IThreedImaging* imaging_;
			IThreedRenderer* renderer_;
			RenderParam* render_param_;
		};
	}
}