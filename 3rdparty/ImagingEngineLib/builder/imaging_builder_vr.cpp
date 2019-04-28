#include "imaging_builder_vr.h"

#include "imaging_vr.h"
#include "renderer_raycasting_cpu.h"
#include "renderer_raycasting_gpu.h"
#include "render/render_param_vr.h"

using namespace DW::Builder;

VRImagingBuilder::VRImagingBuilder()
{
	imaging_ = new VRImaging();
}

VRImagingBuilder::~VRImagingBuilder()
{
	if (imaging_){
		delete imaging_;
	}
}

void VRImagingBuilder::BuildData(IDicomReader* data)
{
	if (imaging_ && data){
		imaging_->SetData(data->GetData());
	}
}

void VRImagingBuilder::BuildRenderer()
{
	if (imaging_){
		render_param_ = new VRRenderParam();
		renderer_ = new RayCastingRenderer();
		renderer_->SetRenderParam(render_param_);
		renderer_->SetData(imaging_->GetData());
		imaging_->SetRenderer(renderer_);
	}
}

IThreedImaging* VRImagingBuilder::GetImaging()
{
	return imaging_;
}