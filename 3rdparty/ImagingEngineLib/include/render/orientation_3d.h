/*=========================================================================

  Program:   ImagingEngine
  Module:    orientation_3d.h
  author: 	 zhangjian
  Brief:	 定义图像方向功能的接口

=========================================================================*/
#pragma once

#include "data/vector2.h"
#include "data/vector3.h"
#include "data/vol_data.h"

namespace DW {
	namespace Render {

		class IThreedOrientationHandler
		{
		public:
			///
			// 计算当前生成图像的四个方向显示值
			// params: left, right, top, bottom
			///
			virtual void GetOrientationTags(Vector3d&, Vector3d&, Vector3d&, Vector3d&) = 0;
		};
	}
}