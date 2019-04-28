/*=========================================================================

  Program:   ImagingEngine
  Module:    imaging_cpr.h
  author: 	 zhangjian
  Brief:	 表示一个具体的CPR图像
			 CPR图像分为Stretched(拉伸CPR)和Straightened(拉直CPR)两种	，两种CPR图像通过不同的渲染对象renderer来实现
			 参考以下文章
			 Kanitsar et al. "CPR - Curved Planar Reformation", Proc. IEEE  Visualization, 2002, 37-44

=========================================================================*/
#pragma once

#include "../data/data_definition.h"
#include "imaging_3d.h"

using namespace DW::IMAGE;

namespace DW {
	namespace Render{

		class CPRImaging : public IThreedImaging
		{
		public:
			CPRImaging();
			~CPRImaging();
			// 获取图像类型
			RenderMode GetRenderMode() override {return RenderMode::CPR;}
			// 获取原始三维图像数据场
			VolData* GetData() override;
			// 设置原始三维图像数据场 
			void SetData(VolData* data) override;
			// 获取输出显示图像
			ShowBuffer* GetShowBuffer() override;
			// 渲染图像
			void Render() override;
			// 创建renderer
			void SetRenderer(IThreedRenderer* renderer) override;
			// 放缩
			void Zoom(int scale) override {}
			// 移动
			void Move(int distance) override {}
			// 平面内旋转
			void Rotate(float angle) override {}
			// 绕着中心轴旋转一定角度
			void Rotate3D(Vector3d axis, float angle) override {}
			// 窗宽窗位
			void WindowWidthLevel(int width, int level) override {}
		protected:

		private:


		};
	}
}