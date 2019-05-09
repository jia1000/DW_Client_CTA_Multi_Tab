/*=========================================================================

  Program:   ImagingEngine
  Module:    render_param_vr.h
  author: 	 zhangjian
  Brief:	 VR图像成像参数，包含了VR协议的参数

=========================================================================*/
#pragma once

#include "data_definition.h"
#include "render_param.h"
#include "vtk_include.h"
#include "data/render_mode.h"

using namespace DW::IMAGE;

namespace DW {
	namespace Render{

		class VRRenderParam : public RenderParam
		{
		public:
			VRRenderParam();
			~VRRenderParam();

			/// Set the desired update rate
			void SetDesiredUpdateRate(float rate);
			/// Get the desired update rate
			float GetDesiredUpdateRate();

			/// Set volume reconstruction method
			void SetRenderingMode(RenderMode mode);
			/// Get volume reconstruction method
			RenderMode GetRenderMode();

		private:
			/// The desired update rate which tells how quickly they need to render
			/// When camera is still, the desired update rate should also be updated to a small value
			float desired_update_rate_;
			/// Render mode: what kind of mapper is used.
			RenderMode rendering_mode_;
		};
	}
}