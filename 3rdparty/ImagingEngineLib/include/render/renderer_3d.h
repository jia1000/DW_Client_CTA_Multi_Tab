/*=========================================================================

  Program:   ImagingEngine
  Module:    renderer_3d.h
  author: 	 zhangjian
  Brief:	 定义渲染接口，流程是接收三维数据，执行Render方法，输出显示结果ShowBuffer对象

=========================================================================*/
#pragma once

#include "data_definition.h"
#include "render_param.h"
#include "vol_data.h"
#include "camera.h"
#include "light.h"
#include "render/pick_3d.h"
#include "render/measure_3d.h"
#include "render/orientation_3d.h"
#include "data/show_buffer.h"
#include "data/render_mode.h"

using namespace DW::IMAGE;

namespace DW {
	namespace Render{
		///
		// 接收数据，render，输出pixeldata
		///
		class IThreedRenderer
		{
		public:
			//TODO: change name to setinput 
			virtual void SetData(VolData* data) = 0;
			virtual void Render() = 0;
			virtual ShowBuffer* GetShowBuffer() = 0;
			virtual void SetRenderParam(RenderParam* param) { render_param_ = param; }
			virtual RenderParam* GetRenderParam() { return render_param_; }
			virtual Camera* GetCamera() { return camera_; }
			virtual RenderMode GetRenderMode() 
			{
				return render_mode_;
			}
			/// Convert world coordinate to display coordinate
			virtual void ComputeWorldToDisplay(Point3f& world_pos, Point3f& display_pos) = 0;
			virtual void ComputeWorldToDisplay(double x, double y, double z, double display_point[3]) = 0;

			///
			// 返回各种handler
			///
			virtual IThreedPickHandler* GetThreedPickHandler() { return NULL; }
			virtual IThreedMeasureHandler* GetThreedMeasureHandlerr() { return NULL; }
			virtual IThreedOrientationHandler* GetThreedOrientationHandlerr() { return NULL; }

			/// 应用窗口窗位/放缩/平移/
			virtual void BufferTransform() {}

		protected:
			// 图像渲染方式
			RenderMode render_mode_;
			// 原始体数据
			VolData* volume_data_;
			// 绘制参数
			RenderParam* render_param_;
			// 图像显示缓存
			ShowBuffer* show_buffer_;
			// 相机
			Camera* camera_;
			// 灯光
			Light* light_;
			bool is_first_render_;
			// 存储缓存图片的地址
			string dump_file_name_;

		private:


		};
	}
}