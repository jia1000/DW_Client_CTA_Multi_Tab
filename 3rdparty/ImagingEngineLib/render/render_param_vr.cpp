#include "render_param_vr.h"

using namespace DW::IMAGE;
using namespace DW::Render;

VRRenderParam::VRRenderParam()
{
	// size
	width_ = 512;
	height_ = 512;
	scale_factor_ = 1.0f;
	//// camera
	//camera_ = new Camera();
	//// light
	//light_ = new Light();
	// set default desired update rate
	desired_update_rate_ = 0.01;
	// set default render mode
	rendering_mode_ = RenderMode::RAYCASTING;
}

VRRenderParam::~VRRenderParam()
{
	
}

void VRRenderParam::SetDesiredUpdateRate(float rate)
{
	desired_update_rate_ = rate;
}

float VRRenderParam::GetDesiredUpdateRate()
{
	return desired_update_rate_;
}

void VRRenderParam::SetRenderingMode(RenderMode mode)
{
	rendering_mode_ = mode;
}

RenderMode VRRenderParam::GetRenderMode()
{
	return rendering_mode_;
}