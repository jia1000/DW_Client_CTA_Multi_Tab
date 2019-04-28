/*=========================================================================

  Program:   ImagingEngine
  Module:    imaging_3d.h
  author: 	 zhangjian
  Brief:	 表示一个三维图像，输入三维数据，执行具体的渲染过程，并输出显示结果
			 提供用于操作图像的接口，如旋转/放缩/平移/窗宽窗位等

=========================================================================*/
#pragma once

#include "data_definition.h"
//#include "imaging_base.h"
#include "vol_data.h"
#include "renderer_3d.h"
#include "render_param.h"
#include "show_buffer.h"
#include "vector3.h"

using namespace DW::IMAGE;

namespace DW {
	namespace Render{

		class IThreedImaging //: public DWImaging
		{
		public:
			// 获取图像类型
			virtual RenderMode GetRenderMode() = 0;
			// 获取原始三维图像数据场
			virtual VolData* GetData() = 0;
			// 获取输出显示图像
			virtual ShowBuffer* GetShowBuffer() = 0;
			// 设置原始三维图像数据场 
			virtual void SetData(VolData* data) = 0;

			// 渲染图像
			virtual void Render() = 0;
			// 放缩
			virtual void Zoom(int scale) = 0;
			// 移动
			virtual void Move(int distance) = 0;
			// 平面内旋转
			virtual void Rotate(float angle) = 0;
			// 绕着中心轴旋转一定角度
			virtual void Rotate3D(Vector3d axis, float angle) = 0;
			// 窗宽窗位
			virtual void WindowWidthLevel(int width, int level) = 0;
			// 创建renderer
			virtual void SetRenderer(IThreedRenderer* renderer) {renderer_=renderer;}
			// 获取renderer
			virtual IThreedRenderer* GetRenderer() {return renderer_;}
		protected:
			// 图像类型
			RenderMode render_mode_;
			// 三维数据
			VolData* volume_data_;
			// 执行渲染的对象
			IThreedRenderer* renderer_;
			// 用于UI显示的结果
			ShowBuffer* show_buffer_;

		private:


		};
	}
}