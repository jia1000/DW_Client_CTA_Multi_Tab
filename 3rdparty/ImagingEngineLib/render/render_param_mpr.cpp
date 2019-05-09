/*=========================================================================

  Program:   ImagingEngine
  Module:    render_param_mpr.cpp
  author: 	 zhangjian
  Brief:	 

=========================================================================*/
#include "render/render_param_mpr.h"

using namespace DW::IMAGE;
using namespace DW::Render;

MPRRenderParam::MPRRenderParam()
{
	// size
	width_ = 512;
	height_ = 512;
	scale_factor_ = 1.0f;
	center_.x = 255;
	center_.y = 255;
	center_.z = 142;
	row_vector[0] = 1.0f;
	row_vector[1] = 0.0f;
	row_vector[2] = 0.0f;
	column_vector[0] = 0.0f;
	column_vector[1] = 1.0f;
	column_vector[2] = 0.0f;
	// camera
	camera_ = new Camera();	
	// thickness : 0.0 means not in Slab mode
	thickness_ = 0.0f;
	blend_mode_ = BlendMode::AverageIntensity;
	interpolation_mode_ = InterpolationMode::Cubic;
}

MPRRenderParam::~MPRRenderParam()
{

}
