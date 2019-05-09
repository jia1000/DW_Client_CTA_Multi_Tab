/*=========================================================================

  Program:   ImagingEngine
  Module:    rotate_cpr.h
  author: 	 zhangjian
  Brief:	 

=========================================================================*/
#pragma once

#include "data_definition.h"
#include "vector3.h"
#include "imaging_3d.h"
#include "actor_3d.h"

using namespace DW;
using namespace DW::ACTOR;

namespace DW {
	namespace ACTOR {
		
		///
		// VR旋转命令
		///
		class CPRRotateActor : public IThreeDActor
		{
		public:
			CPRRotateActor();
			~CPRRotateActor();
			// 执行命令
			void Execute(ActorArgs* args) override;
		protected:

		};

		///
		// CPR旋转命令参数
		///
		class CPRRotateArgs : public ActorArgs
		{
		public:
			CPRRotateArgs() {}
			~CPRRotateArgs() {}
			void SetOffetX(float x) { offet_x = x; }
			float GetOffetX() { return offet_x; }
			void SetOffetY(float y) { offet_y = y; }
			float GetOffetY() { return offet_y; }
			void SetAngle(float angle) { angle_ = angle; }
			float GetAngle() { return angle_; }
			void SetRotationAxis(float* vec) 
			{ 
				axis_[0] = vec[0];
				axis_[1] = vec[1]; 
				axis_[2] = vec[2]; 
			}
			float* GetRotationAxis() { return axis_; }
		protected:
			// 鼠标位移x
			float offet_x;
			// 鼠标位移y
			float offet_y;
			// 旋转角度
			float angle_;
			// 计算出来的旋转轴
			float* axis_;
		};

	}
}