/*=========================================================================

  Program:   ImagingEngine
  Module:    imaging_vr.h
  author: 	 zhangjian
  Brief:	 表示一个具体的VR图像
			 VR图像可以有多种重建方法，由不同的渲染对象renderer来实现

=========================================================================*/
#pragma once

#include "data_definition.h"
#include "imaging_3d.h"
#include "vector3.h"

using namespace DW::IMAGE;

namespace DW {
	namespace Render{

		class VRImaging : public IThreedImaging
		{
		public:
			VRImaging(string id);
			~VRImaging();

			/// 获取原始三维图像数据场
			VolData* GetData() override;
			/// 设置原始三维图像数据场 
			void SetData(VolData* data) override;
			/// 获取输出显示图像
			ShowBuffer* GetShowBuffer() override;
			/// 渲染图像
			void Render() override;
			/// 放缩
			void Zoom(float scale) override;
			/// 放缩到适应窗口
			void ZoomToFitWindow() override;
			/// 移动
			void Move(float dx, float dy) override;
			/// 平移
			void Move(float motionVector[3]) override;
			/// 平面内旋转
			void Rotate(float angle) override;
			/// 绕着中心轴旋转一定角度
			void Rotate3D(Vector3f axis, float angle) override;
			/// 窗宽窗位
			void WindowWidthLevel(int width, int level) override;
			void WindowWidthLevelOffset(int offset_width, int offset_level) override;
			/// 创建renderer
			void SetRenderer(IThreedRenderer* renderer) override;

		protected:
			void ConvertToHBITMAP() override;

		private:


		};
	}
}