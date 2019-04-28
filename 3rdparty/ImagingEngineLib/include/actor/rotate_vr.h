/*=========================================================================

  Program:   ImagingEngine
  Module:    rotate_vr.h
  author: 	 zhangjian
  Brief:	 VR图像旋转命令

=========================================================================*/
#pragma once

#include "data_definition.h"
#include "vector3.h"
#include "imaging_3d.h"
#include "actor_3d.h"

using namespace DW::Render;
using namespace std;

namespace DW {
	namespace ACTOR{
		
		///
		// VR旋转命令
		///
		class VRRotateActor : public IThreeDActor
		{
		public:
			VRRotateActor();
			~VRRotateActor();
			// 执行命令
			void Execute(ActorArgs* args) override;
			// 绕着指定轴旋转: not in use
			void Rotate(Vector3d axis, float angle);
			// 绕着平面轴旋转: not in use
			void Rotate(float x, float y, float angle);
		protected:

		};

		///
		// VR旋转命令参数
		///
		class VRRotateArgs : public ActorArgs
		{
		public:
			VRRotateArgs() {}
			~VRRotateArgs() {}
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