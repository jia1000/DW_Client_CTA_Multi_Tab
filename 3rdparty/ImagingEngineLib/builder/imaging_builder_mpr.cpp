#include "imaging_builder_mpr.h"

#include "imaging_vr.h"
#include "renderer_raycasting_cpu.h"

using namespace DW::Builder;

MPRImagingBuilder::MPRImagingBuilder()
{
	imaging_ = new VRImaging();
}

MPRImagingBuilder::~MPRImagingBuilder()
{
	if (imaging_){
		delete imaging_;
	}
}

void MPRImagingBuilder::BuildData(IDicomReader* data)
{
	if (imaging_ && data){
		imaging_->SetData(data->GetData());
	}
}

void MPRImagingBuilder::BuildRenderer()
{
	if (imaging_){
		renderer_ = new RayCastingRenderer();
		renderer_->SetData(imaging_->GetData());
		imaging_->SetRenderer(renderer_);
	}
}

IThreedRenderer* MPRImagingBuilder::GetImaging()
{
	return imaging_;
}