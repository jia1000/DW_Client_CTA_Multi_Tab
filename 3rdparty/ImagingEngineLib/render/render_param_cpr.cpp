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
	// camera
	camera_ = new Camera();	
	camera_->GetCamera()->SetViewUp(0,0,1);
	camera_->GetCamera()->SetPosition(0,0,0);
	camera_->GetCamera()->SetFocalPoint(0,1,0);
	// angle
	rotation_angle_ = 0.0f;
}

CPRRenderParam::~CPRRenderParam()
{

}