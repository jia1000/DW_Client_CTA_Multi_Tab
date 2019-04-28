/*=========================================================================

  Program:   ImagingEngine
  Module:    batch_vr.h
  author: 	 zhangjian
  Brief:	 

=========================================================================*/
#pragma once

#include "data_definition.h"
#include "batch.h"
#include "data/camera.h"

using namespace DW::IMAGE;

namespace DW {
	namespace Batch{
		///
		// VR批处理
		///
		class VRBatchGenerator : public IBatchGenerator
		{
		public:
			VRBatchGenerator();
			~VRBatchGenerator();
			void Execute(BatchArgs*) override;
			void SetCallback(IBatchCallback*) override;
			void Destroy() override;
		protected:
			
		private:
			
		};

		///
		// VR批处理参数
		///
		class VRBatchArgs : public BatchArgs
		{
		public:
			void SetCamera(Camera* start, Camera* end) {
				from_camera_ = start;
				to_camera_ = end;
			}
			Camera* GetStartCamera() {return from_camera_;}
			Camera* GetEndCamera() {return to_camera_;}
			void SetImageCount(int count) {image_count_=count;}
			int GetImageCount() {return image_count_;}
			void SetStepAngle(float angle) {step_angle_=angle;}
			float GetStepAngle() {return step_angle_;}
			void SetAxis(float x, float y, float z) {
				rotate_axis_[0] = x;
				rotate_axis_[1] = y;
				rotate_axis_[2] = z;
			}
			void GetAxis(float& x, float& y, float& z) {
			}
		private:
			Camera* from_camera_;
			Camera* to_camera_;
			int image_count_;
			float step_angle_;
			Vector3f rotate_axis_;
		};
	}
}