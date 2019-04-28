/*=========================================================================

  Program:   ImagingEngine
  Module:    light.h
  author: 	 zhangjian
  Brief:	 µ∆π‚

=========================================================================*/
#pragma once

#include <vtkLight.h>
#include <vtkSmartPointer.h>
#include "data_definition.h"

using namespace DW::IMAGE;
using namespace std;

namespace DW {
	namespace IMAGE{
		
		///
		// µ∆π‚¿‡
		///
		class Light
		{
		public:
			Light();
			~Light();
			vtkSmartPointer<vtkLight> GetLight();
			void SetLight(vtkSmartPointer<vtkLight> light);
		protected:
			vtkSmartPointer<vtkLight> vtk_light_;
		private:

		};
	}
}