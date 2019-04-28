/*=========================================================================

  Program:   ImagingEngine
  Module:    renderer_straightened_cpr.h
  author: 	 zhangjian
  Brief:	 CPR图像Straightened类型渲染类

=========================================================================*/
#pragma once
#include "vtk_include.h"
#include "data_definition.h"
#include "render/renderer_cpr.h"

using namespace DW::IMAGE;

namespace DW {
	namespace Render {
		///
		// 平面拉直CPR
		///
		class StraightededCPRRenderer : public CPRRenderer
		{
		public:
			StraightededCPRRenderer();
			~StraightededCPRRenderer();
		protected:
			void DoRender() override;
		private:

		};

	}
}