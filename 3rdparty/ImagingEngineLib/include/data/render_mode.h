/*=========================================================================

  Program:   ImagingEngine
  Module:    render_mode.h
  author: 	 zhangjian
  Brief:	 

=========================================================================*/
#pragma once

#include "data_definition.h"
#include "data_convert.h"

using namespace DW::IMAGE;
using namespace std;

namespace DW {
	namespace IMAGE{

		enum RenderMode
		{
			RaycastingGPU,
			Raycasting,
			Texture3DGPU,
			IsoSurface,
			SSD,
			MIP,
			MPR,
			CPR
		};
	}
}