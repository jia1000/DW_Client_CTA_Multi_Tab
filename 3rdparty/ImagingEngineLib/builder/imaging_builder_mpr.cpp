#include "imaging_builder_mpr.h"

#include "imaging_mpr.h"
#include "render/renderer_mpr.h"
#include "render/render_param_mpr.h"
#include "render/renderer_mpr_vtk.h"

using namespace DW::Builder;

MPRImagingBuilder::MPRImagingBuilder()
{
	imaging_ = new MPRImaging("");
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

void MPRImagingBuilder::BuildRenderer(string)
{
	if (imaging_){
		MPRRenderParam *param = new MPRRenderParam();
		param->SetWidth(512);
		param->SetHeight(512);
		param->SetImageCenter(0,0,0);
		param->SetRotationCenter(0,0,0);
		// 设置默认窗口窗位
		if (imaging_->GetData()){
			int width, level;
			imaging_->GetData()->GetDefaultWindowWidthLevel(width, level);
			param->SetWindowWidthLevel(width, level);
		}
		param->SetBlendMode(BlendMode::MaximumIntensity);
		param->SetInterpolationMode(InterpolationMode::Cubic);

		renderer_ = new MPRRenderer();
		renderer_->SetRenderParam(param);
		renderer_->SetData(imaging_->GetData());
		imaging_->SetRenderer(renderer_);
	}
}

IThreedImaging* MPRImagingBuilder::GetImaging()
{
	return imaging_;
}