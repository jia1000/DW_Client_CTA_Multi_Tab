/*=========================================================================

  Program:   ImagingEngine
  Module:    measure_3d.h
  author: 	 zhangjian
  Brief:	 定义测量功能的接口

=========================================================================*/
#pragma once

#include "data/vector2.h"
#include "data/vector3.h"
#include "data/vol_data.h"

namespace DW {
	namespace Render {

		class IThreedMeasureHandler
		{
		public:
			///
			// 计算三维数据场体积
			///
			virtual float GetVoxelVolume() { return 0.0; };
			///
			// 计算指定范围内数据场体积
			///
			virtual float GetVoxelVolume(BoundingBox*) { return 0.0; };
			///
			// 计算数据场内两点之间长度
			///
			virtual float CalculateLength(Vector3d&, Vector3d&) { return 0.0; };
		};
	}
}