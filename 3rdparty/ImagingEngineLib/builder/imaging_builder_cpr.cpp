#include "builder/imaging_builder_cpr.h"

#include "render/imaging_cpr.h"
#include "render/render_param_cpr.h"
#include "render/renderer_cpr.h"
#include "render/renderer_straightened_cpr.h"
#include "render/renderer_stretched_cpr.h"

#include "io/txt_reader.h"
#include "tools/string_util.h"

using namespace DW::Builder;

CPRImagingBuilder::CPRImagingBuilder()
{
	imaging_ = new CPRImaging("");
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

void CPRImagingBuilder::BuildRenderer(string str)
{
	if (imaging_ && curve_path_.empty() == false){
		CPRRenderParam *param = new CPRRenderParam();
		param->SetWidth(512);
		param->SetHeight(512);
		param->SetCurve(new VolCurve());
		vector<string> curve_data = ReadTxt(curve_path_.c_str());
		auto it = curve_data.begin();
		while (it != curve_data.end()){
			vector<string> arr_data = Split(*it, ",");
			if (arr_data.size() >= 3){
				int x = atoi(arr_data[0].c_str());
				int y = atoi(arr_data[1].c_str());
				int z = atoi(arr_data[2].c_str()) - 1;
				param->GetCurve()->AddControlPoint(x, y, z);
			}
			++it;
		}
		param->GetCurve()->Update();
		param->SetDirection(1.0f, 0.0f, 0.0f);
		// 设置默认窗口窗位
		if (imaging_->GetData()){
			int width, level;
			imaging_->GetData()->GetDefaultWindowWidthLevel(width, level);
			param->SetWindowWidthLevel(width, level);
		}
		render_param_ = param;
		if (str == "Straightened"){
			renderer_ = new StraightededCPRRenderer();
		}
		else{
			renderer_ = new StretchedCPRRenderer();
		}
		renderer_->SetRenderParam(render_param_);
		renderer_->SetData(imaging_->GetData());
		imaging_->SetRenderer(renderer_);
	}
}

IThreedImaging* CPRImagingBuilder::GetImaging()
{
	return imaging_;
}