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

using namespace DW::IMAGE;

namespace DW {
	namespace Render{

		class VRRenderParam : public RenderParam
		{
		public:
			VRRenderParam();
			~VRRenderParam();
			float GetAmbient();
			void SetAmbient(float v);
			float GetDiffuse();
			void SetDiffuse(float v);
			float GetSpecular();
			void SetSpecular(float v);
			float GetSpecularPower();
			void SetSpecularPower(float v);
			vtkSmartPointer<vtkColorTransferFunction> GetColorFunction();
			void SetColorFunction(vtkSmartPointer<vtkColorTransferFunction> func);
			vtkSmartPointer<vtkPiecewiseFunction> GetOpacityFunction();
			void SetOpacityFunction(vtkSmartPointer<vtkPiecewiseFunction> func);
			vtkSmartPointer<vtkPiecewiseFunction> GetGradientFunction();
			void SetGradientFunction(vtkSmartPointer<vtkPiecewiseFunction> func);
		private:
			float ambient_;
			float diffuse_;
			float specular_;
			float specular_power_;
			vtkSmartPointer<vtkColorTransferFunction> color_func_;
			vtkSmartPointer<vtkPiecewiseFunction> opacity_func_;
			vtkSmartPointer<vtkPiecewiseFunction> gradient_func_;
		};
	}
}