/*=========================================================================

  Program:   ImagingEngine
  Module:    pick_point.h
  author: 	 zhangjian
  Brief:	 选择点命令

=========================================================================*/
#pragma once

#include "data_definition.h"
#include "vector3.h"
#include "imaging_3d.h"
#include "actor_3d.h"
#include "render/pick_3d.h"

using namespace DW::Render;
using namespace std;

namespace DW {
	namespace ACTOR{
		
		class ActorArgs;

		///
		// 选择点命令
		///
		class PickPointActor : public IThreeDActor
		{
		public:
			PickPointActor();
			~PickPointActor();
			// 执行命令
			void Execute(ActorArgs* args) override;
			
		protected:

		};

		///
		// 拾取点命令
		///
		class PickPointArgs : public ActorArgs
		{
		public:
			PickPointArgs() {}
			~PickPointArgs() {}
			float GetImageX() { return image_x; }
			void SetImageX(float x) { image_x = x; }
			float GetImageY() { return image_y; }
			void SetImageY(float y) { image_y = y; }
			Vector3d GetPicked3DPoint() { return volume_coordinate_; }
			void SetGetPicked3DPoint(float x, float y, float z){
				volume_coordinate_[0] = x;
				volume_coordinate_[1] = y;
				volume_coordinate_[2] = z;
			}
		protected:
			// 屏幕坐标x
			float image_x;
			// 屏幕坐标y
			float image_y;
			// 返回的三维数据场坐标
			Vector3d volume_coordinate_;
		};
	}
}