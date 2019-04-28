#include "builder/imaging_builder_cpr.h"

#include "render/imaging_cpr.h"
#include "render/render_param_cpr.h"
#include "render/renderer_cpr.h"
#include "render/renderer_straightened_cpr.h"
#include "render/renderer_stretched_cpr.h"

using namespace DW::Builder;

CPRImagingBuilder::CPRImagingBuilder()
{
	imaging_ = new CPRImaging();
}

CPRImagingBuilder::~CPRImagingBuilder()
{
	if (imaging_){
		delete imaging_;
	}
}

void CPRImagingBuilder::BuildData(IDicomReader* data)
{
	if (imaging_ && data){
		imaging_->SetData(data->GetData());
	}
}

void CPRImagingBuilder::BuildRenderer()
{
	if (imaging_){
		CPRRenderParam *param = new CPRRenderParam();
		param->SetWidth(512);
		param->SetHeight(512);
		param->SetCurve(new VolCurve());
		param->GetCurve()->AddControlPoint(256,512-231,0);
		param->GetCurve()->AddControlPoint(260,512-229,5);
		param->GetCurve()->AddControlPoint(259,512-232,10);
		param->GetCurve()->AddControlPoint(258,512-240,20);
		param->GetCurve()->AddControlPoint(256,512-243,25);
		param->GetCurve()->AddControlPoint(257,512-244,30);
		param->GetCurve()->AddControlPoint(250,512-253,40);
		param->GetCurve()->AddControlPoint(249,512-258,50);
		param->GetCurve()->AddControlPoint(248,512-261,60);
		param->GetCurve()->AddControlPoint(247,512-269,70);
		param->GetCurve()->AddControlPoint(247,512-274,80);
		param->GetCurve()->AddControlPoint(251,512-276,90);
		param->GetCurve()->AddControlPoint(242,512-277,100);
		param->GetCurve()->AddControlPoint(235,512-279,105);
		param->GetCurve()->AddControlPoint(235,512-277,110);
		param->GetCurve()->AddControlPoint(234,512-279,115);
		param->GetCurve()->AddControlPoint(227,512-281,130);
		param->GetCurve()->Update();
		param->SetDirection(1.0f, 0.0f, 0.0f);
		param->SetWindowWidthLevel(1500, -600);
		render_param_ = param;
		//renderer_ = new StretchedCPRRenderer();
		renderer_ = new StraightededCPRRenderer();
		renderer_->SetRenderParam(render_param_);
		renderer_->SetData(imaging_->GetData());
		imaging_->SetRenderer(renderer_);
	}
}

IThreedImaging* CPRImagingBuilder::GetImaging()
{
	return imaging_;
}