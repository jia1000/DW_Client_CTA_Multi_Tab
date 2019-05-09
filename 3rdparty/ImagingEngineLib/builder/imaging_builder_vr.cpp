#include "imaging_builder_vr.h"

#include "imaging_vr.h"
#include "render/renderer_vr.h"
#include "render/render_param_vr.h"

using namespace DW::Builder;

VRImagingBuilder::VRImagingBuilder()
{
	imaging_ = new VRImaging("");
}

VRImagingBuilder::~VRImagingBuilder()
{
	if (imaging_){
		delete imaging_;
		imaging_ = NULL;
	}
}

void VRImagingBuilder::BuildData(IDicomReader* data)
{
	if (imaging_ && data){
		imaging_->SetData(data->GetData());
	}
}

void VRImagingBuilder::BuildRenderer(string str)
{
	if (imaging_){
		render_param_ = new VRRenderParam();
		renderer_ = new VolumeRenderer();
		renderer_->SetRenderParam(render_param_);
		renderer_->SetData(imaging_->GetData());
		imaging_->SetRenderer(renderer_);
	}
}

IThreedImaging* VRImagingBuilder::GetImaging()
{
	return imaging_;
}