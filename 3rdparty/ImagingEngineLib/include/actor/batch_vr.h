/*=========================================================================

  Program:   ImagingEngine
  Module:    batch_vr.h
  author: 	 zhangjian
  Brief:	 VR批处理命令

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
		
		class ActorArgs;

		///
		// VR旋转命令
		///
		class VRBatchActor : public IThreeDActor
		{
		public:
			VRBatchActor();
			~VRBatchActor();
			// 执行命令
			void Execute(ActorArgs* args) override;
		protected:

		};

		///
		// VR批处理命令参数
		///
		class VRBatchArgs : public ActorArgs
		{
		public:
			VRBatchArgs() {}
			~VRBatchArgs() {}
			void SetImageNumber(uint16_t num) { image_number_ = num; }
			uint16_t GetImageNumber() { return image_number_; }
			void SetRotationDegree(float degree) { rotation_degree_ = degree; }
			float GetRotationDegree() { return rotation_degree_; }
			void SetRotationAxis(float* axis) { rotation_axis_ = axis; }
			float* GetRotationAxis() { return rotation_axis_; }
			void SetRotationCenter(Point3d pnt) { rotation_center_ = pnt; }
			Point3d GetRotationCenter() { return rotation_center_; }
			
		protected:
			// 生成图像数目
			uint16_t image_number_;
			// 转动角度
			float rotation_degree_;
			// 旋转轴
			float* rotation_axis_;
			// 旋转中心
			Point3d rotation_center_;
		};
	}
}