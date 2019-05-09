/*=========================================================================

  Program:   ImagingEngine
  Module:    imaging_mpr.h
  author: 	 zhangjian
  Brief:	 

=========================================================================*/
#pragma once

#include "../data/data_definition.h"
#include "imaging_3d.h"

using namespace DW::IMAGE;

namespace DW {
	namespace Render{

		class MPRImaging : public IThreedImaging
		{
		public:
			MPRImaging(string id);
			~MPRImaging();

			/// 获取原始三维图像数据场
			VolData* GetData() override;
			/// 设置原始三维图像数据场 
			void SetData(VolData* data) override;
			/// 获取输出显示图像
			ShowBuffer* GetShowBuffer() override;
			/// 渲染图像
			void Render() override;
			/// 创建renderer
			void SetRenderer(IThreedRenderer* renderer) override;
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

		protected:
			void ConvertToHBITMAP() override;

		private:


		};
	}
}