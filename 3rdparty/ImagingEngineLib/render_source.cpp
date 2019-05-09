/*=========================================================================

  Program:   ImagingEngine
  Module:    render_source.cpp
  author: 	 zhangjian
  Brief:	 

=========================================================================*/
#include "render_source.h"
#include "tools/string_util.h"
#include "render/render_param.h"
#include "render/render_param_cpr.h"
#include "render/render_param_mpr.h"
#include "render/render_param_vr.h"
#include "render/renderer_3d.h"
#include "render/renderer_cpr.h"
#include "render/renderer_stretched_cpr.h"
#include "render/renderer_straightened_cpr.h"
#include "render/renderer_mpr.h"
#include "render/renderer_vr.h"
#include "render/imaging_3d.h"
#include "render/imaging_cpr.h"
#include "render/imaging_mpr.h"
#include "render/imaging_vr.h"

using namespace DW;
using namespace DW::IMAGE;
using namespace DW::Render;


RenderSource *RenderSource::instance_ = 0;
RenderSource *RenderSource::Get() 
{
	if (instance_ == nullptr)
		instance_ = new RenderSource;
	return instance_;
}

IThreedImaging* RenderSource::GetRenderControl(string control_id)
{
	auto it = imaging_list_.begin();
	for (; it<imaging_list_.end(); ++it){
		if ((*it)->GetImagingID() == control_id){
			return (*it);
		}
	}
	return NULL;
}

IThreedImaging* RenderSource::CreateRenderControl(string control_id, RenderControlType type)
{
	if (control_id.empty()){
		control_id = GenerateGUID();
	}

	IThreedRenderer *renderer = CreateRenderer(type);
	IThreedImaging *imaging = NULL;
	switch (type)
	{
	case DW::IMAGE::RenderControlType::VR:
		{
			imaging = new VRImaging(control_id);
			imaging->SetRenderer(renderer);
		}
		break;
	case DW::IMAGE::RenderControlType::MPR:
	case DW::IMAGE::RenderControlType::CROSS_SECTIONAL_MPR:
		{
			imaging = new MPRImaging(control_id);
			imaging->SetRenderer(renderer);
		}
		break;
	case DW::IMAGE::RenderControlType::STRETECHED_CPR:
	case DW::IMAGE::RenderControlType::STRAIGHTENED_CPR:
		{
			imaging = new CPRImaging(control_id);
			imaging->SetRenderer(renderer);
		}
		break;
	default:
		break;
	}

	if (imaging){
		imaging_list_.push_back(imaging);
	}
	return imaging;
}

IThreedRenderer *RenderSource::CreateRenderer(RenderControlType type)
{
	IThreedRenderer *renderer = NULL;
	// 通过type以及配置来选择采用哪种Renderer
	switch (type)
	{
	case DW::IMAGE::RenderControlType::VR:
		{
			VRRenderParam *param_vr = new VRRenderParam();
			param_vr->SetRenderingMode(RenderMode::RAYCASTING_GPU);
			renderer = new VolumeRenderer();
			renderer->SetRenderParam(param_vr);
		}		
		break;
	case DW::IMAGE::RenderControlType::MPR:
	case DW::IMAGE::RenderControlType::CROSS_SECTIONAL_MPR:
		{
			MPRRenderParam *param_mpr = new MPRRenderParam();
			param_mpr->SetWidth(512);
			param_mpr->SetHeight(512);
			param_mpr->SetImageCenter(0,0,0);
			param_mpr->SetRotationCenter(0,0,0);		
			param_mpr->SetBlendMode(BlendMode::MaximumIntensity);
			param_mpr->SetInterpolationMode(InterpolationMode::Cubic);
			renderer = new MPRRenderer();
			renderer->SetRenderParam(param_mpr);
		}		
		break;
	case DW::IMAGE::RenderControlType::STRETECHED_CPR:
		{
			CPRRenderParam *param = new CPRRenderParam();
			param->SetWidth(512);
			param->SetHeight(512);		
			param->SetDirection(1.0f, 0.0f, 0.0f);		
			renderer = new StretchedCPRRenderer();
			renderer->SetRenderParam(param);
		}		
		break;
	case DW::IMAGE::RenderControlType::STRAIGHTENED_CPR:
		{
			CPRRenderParam *param_cpr = new CPRRenderParam();
			param_cpr->SetWidth(512);
			param_cpr->SetHeight(512);		
			param_cpr->SetDirection(1.0f, 0.0f, 0.0f);		
			renderer = new StraightededCPRRenderer();
			renderer->SetRenderParam(param_cpr);
		}
		break;
	default:
		break;
	}

	return renderer;
}