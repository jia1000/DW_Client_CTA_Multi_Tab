/*=========================================================================

  Program:   ImagingEngine
  Module:    render_param.h
  author: 	 zhangjian
  Brief:	 成像参数基类，可用于渲染对象

=========================================================================*/
#pragma once

#include "data_definition.h"
#include "data/camera.h"
#include "data/light.h"
#include "data/render_mode.h"

using namespace DW::IMAGE;

namespace DW {
	namespace Render{

		class RenderParam
		{
		public:
			RenderParam()
			{
				width_ = 512;
				height_ = 512;
				scale_factor_ = 1.0f;
				offset_x_ = 0.0f;
				offset_y_ = 0.0f;
			}
			~RenderParam()
			{
				if (camera_){
					delete camera_;
					camera_ = NULL;
				}
			}
			virtual int GetWidth()
			{
				return width_;
			}
			virtual void SetWidth(int w)
			{
				width_ = w;
			}
			virtual int GetHeight()
			{
				return height_;
			}
			virtual void SetHeight(int h)
			{
				height_ =h;
			}
			//virtual Camera* GetCamera()
			//{
			//	return camera_;
			//}
			//virtual void SetCamera(Camera* camera)
			//{
			//	camera_ = camera;
			//}


			/// 设置窗宽窗位
			void SetWindowWidthLevel(int width, int level)
			{
				window_width_ = width;
				window_level_ = level;
			}
			/// 获取窗宽窗位
			void GetWindowWidthLevel(int& width, int& level)
			{
				width = window_width_;
				level = window_level_;
			}
			virtual void SetScale(float scale)
			{
				scale_factor_ = scale;
			}
			virtual float GetScale()
			{
				return scale_factor_;
			}
			virtual void SetOffset(float dx, float dy)
			{
				offset_x_ = dx;
				offset_y_ = dy;
			}
			virtual void GetOffset(float &dx, float &dy)
			{
				dx = offset_x_;
				dy = offset_y_;
			}
			virtual void SetBlendMode(BlendMode mode)
			{
				blend_mode_ = mode;
			}
			virtual BlendMode GetBlendMode()
			{
				return blend_mode_;
			}
			virtual void SetInterpolationMode(DW::IMAGE::InterpolationMode mode)
			{
				interpolation_mode_ = mode;
			}
			virtual DW::IMAGE::InterpolationMode GetInterpolationMode()
			{
				return interpolation_mode_;
			}

		protected:
			BYTE render_type_;
			/// 窗宽
			int window_width_;
			/// 窗位
			int window_level_;
			BlendMode blend_mode_;
			DW::IMAGE::InterpolationMode interpolation_mode_;
			int width_;
			int height_;
			Camera* camera_;
			//Light* light_;
			/// 放缩系数
			float scale_factor_;
			/// 偏移量
			float offset_x_;
			float offset_y_;
		};
	}
}