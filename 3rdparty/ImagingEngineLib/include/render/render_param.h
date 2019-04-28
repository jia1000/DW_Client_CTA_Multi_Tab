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

using namespace DW::IMAGE;

namespace DW {
	namespace Render{

		class RenderParam
		{
		public:
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
			virtual Camera* GetCamera()
			{
				return camera_;
			}
			virtual void SetCamera(Camera* camera)
			{
				camera_ = camera;
			}
		protected:
			BYTE render_type_;
			int width_;
			int height_;
			Camera* camera_;
			//Light* light_;
		};
	}
}