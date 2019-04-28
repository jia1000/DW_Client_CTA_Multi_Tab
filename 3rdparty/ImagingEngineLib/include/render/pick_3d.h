/*=========================================================================

  Program:   ImagingEngine
  Module:    IThreeDPick.h
  author: 	 zhangjian
  Brief:	 定义三维点和二维点的转换

=========================================================================*/
#pragma once

#include "data/vector2.h"
#include "data/vector3.h"

namespace DW {
	namespace Render {

		class IThreedPickHandler
		{
		public:
			///
			// 图像坐标点转换位三维数据场中的点
			///
			virtual bool PickByImagePos(const Vector2i&, Vector3d&) = 0;
			///
			// 图像坐标点转换位三维数据场中的点
			///
			virtual bool PickByImagePos(const int&, const int&, Vector3d&) = 0;
			///
			// 三维点转换到生成后的图像坐标点
			///
			virtual bool PickBy3DPoint(const Vector3d&, int&, int&) = 0;
		};

	}
}