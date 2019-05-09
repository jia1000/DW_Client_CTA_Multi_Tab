/*=========================================================================

  Program:   ImagingEngine
  Module:    renderer_raycasting_gpu.h
  author: 	 zhangjian
  Brief:	 VR图像的raycasting重建方法渲染类，基于GPU

=========================================================================*/
#pragma once

#include "vtk_include.h"
#include "data_definition.h"
#include "renderer_raycasting_cpu.h"
#include "render/renderer_vr.h"

using namespace DW::IMAGE;

namespace DW {
	namespace Render{

		class RayCastingRendererGPU : public VolumeRenderer
		{
		public:
			RayCastingRendererGPU();
			~RayCastingRendererGPU();
		protected:
			void DoRender(vtkSmartPointer<vtkImageData> imagedata) override;
		private:

		};
	}
}