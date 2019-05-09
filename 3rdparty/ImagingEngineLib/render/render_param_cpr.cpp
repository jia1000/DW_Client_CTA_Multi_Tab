/*=========================================================================

  Program:   ImagingEngine
  Module:    render_param_cpr.cpp
  author: 	 zhangjian
  Brief:	 

=========================================================================*/
#include "render/render_param_cpr.h"

using namespace DW::IMAGE;
using namespace DW::Render;

CPRRenderParam::CPRRenderParam()
{
	// size
	width_ = 512;
	height_ = 512;
	scale_factor_ = 1.0f;
	// camera
	camera_ = new Camera();
	// angle
	rotation_angle_ = 0.0f;
	// thickness : 0.0 means not in Slab mode
	thickness_ = 0.0f;
	blend_mode_ = BlendMode::AverageIntensity;
	interpolation_mode_ = InterpolationMode::Cubic;
}

CPRRenderParam::~CPRRenderParam()
{

}