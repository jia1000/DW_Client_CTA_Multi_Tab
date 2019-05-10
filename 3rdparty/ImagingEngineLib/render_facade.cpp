/*=========================================================================

  Program:   ImagingEngine
  Module:    render_facade.cpp
  author: 	 zhangjian
  Brief:	 

=========================================================================*/
#include "render_facade.h"
#include "render/render_param.h"
#include "render/render_param_mpr.h"
#include "render/render_param_cpr.h"
#include "render/render_param_vr.h"
#include "curve_source.h"
#include "data_source.h"

using namespace DW;
using namespace DW::IMAGE;
using namespace DW::Render;


RenderFacade *RenderFacade::instance_ = 0;
RenderFacade *RenderFacade::Get() 
{
	if (instance_ == nullptr)
		instance_ = new RenderFacade;
	return instance_;
}

IThreedImaging *RenderFacade::GetControl(string control_id)
{
	return RenderSource::Get()->GetRenderControl(control_id);
}

HBITMAP RenderFacade::GetImageBuffer(string control_id)
{
	IThreedImaging *imaging = RenderSource::Get()->GetRenderControl(control_id);
	if (imaging == NULL) return NULL;

	return imaging->GetImageBuffer();
}

void RenderFacade::Zoom(string control_id, float scale)
{
	IThreedImaging *imaging = GetControl(control_id);
	if (imaging == NULL) return;

	imaging->Zoom(scale);
}

void RenderFacade::ZoomToSize(string control_id, int width, int height)
{
	IThreedImaging *imaging = GetControl(control_id);
	if (imaging == NULL) return;

	//TODO update width and height...or redefine the interface...
	imaging->ZoomToFitWindow();
}

void RenderFacade::Move(string control_id, float dx, float dy)
{
	IThreedImaging *imaging = GetControl(control_id);
	if (imaging == NULL) return;

	imaging->Move(dx, dy);
}

void RenderFacade::Move(string control_id, float move_vector[3])
{
	IThreedImaging *imaging = GetControl(control_id);
	if (imaging == NULL) return;

	imaging->Move(move_vector);
}

void RenderFacade::Roll(string control_id, float angle)
{
	IThreedImaging *imaging = GetControl(control_id);
	if (imaging == NULL) return;

	imaging->Rotate(angle);
}

void RenderFacade::Rotate(string control_id, float angle, float axis[3])
{
	IThreedImaging *imaging = GetControl(control_id);
	if (imaging == NULL) return;

	Vector3f axis_vec(axis[0], axis[1], axis[2]);
	imaging->Rotate3D(axis_vec, angle);
}

void RenderFacade::SetOrientation(string control_id, OrientationType ori)
{
	IThreedImaging *imaging = GetControl(control_id);
	if (imaging == NULL) return;

	RenderParam *param = imaging->GetRenderer()->GetRenderParam();
	if (param == NULL) return;

	RenderMode mode = imaging->GetRenderMode();
	switch (mode)
	{
	case DW::IMAGE::RenderMode::SMART_RAYCASTING:
	case DW::IMAGE::RenderMode::RAYCASTING_GPU:
	case DW::IMAGE::RenderMode::RAYCASTING:
	case DW::IMAGE::RenderMode::TEXTURE_3D_GPU:
	case DW::IMAGE::RenderMode::ISO_SURFACE:
	case DW::IMAGE::RenderMode::SSD:
		{
			Camera *camera = imaging->GetRenderer()->GetCamera();
			if (camera){
				camera->SetCameraViewPlane(ori);
			}
		}
		break;
	case DW::IMAGE::RenderMode::MPR:
	case DW::IMAGE::RenderMode::CROSS_SECTIONAL_MPR:
		{
			MPRRenderParam *param_mpr = dynamic_cast<MPRRenderParam *>(imaging->GetRenderer()->GetRenderParam());
			if (param_mpr){
				Vector3f row_vec, col_vec;
				if (ori == OrientationType::AXIAL){
					row_vec[0] = 1.0f;
					row_vec[1] = 0.0f;
					row_vec[2] = 0.0f;
					col_vec[0] = 0.0f;
					col_vec[1] = 1.0f;
					col_vec[2] = 0.0f;
				}
				else if (ori == OrientationType::SAGITTAL){
					row_vec[0] = 0.0f;
					row_vec[1] = 1.0f;
					row_vec[2] = 0.0f;
					col_vec[0] = 0.0f;
					col_vec[1] = 0.0f;
					col_vec[2] = -1.0f;
				}
				else if (ori == OrientationType::CORONAL){
					row_vec[0] = 1.0f;
					row_vec[1] = 0.0f;
					row_vec[2] = 0.0f;
					col_vec[0] = 0.0f;
					col_vec[1] = 0.0f;
					col_vec[2] = -1.0f;
				}
				else{
					row_vec[0] = 1.0f;
					row_vec[1] = 0.0f;
					row_vec[2] = 0.0f;
					col_vec[0] = 0.0f;
					col_vec[1] = 1.0f;
					col_vec[2] = 0.0f;
				}
				param_mpr->SetPlaneVector(row_vec, col_vec);
				param_mpr->SetImageCenter(255,255,0);
			}
		}
		break;
	case DW::IMAGE::RenderMode::STRAIGHTENED_CPR:
	case DW::IMAGE::RenderMode::STRETECHED_CPR:
		{
			CPRRenderParam *param_cpr = dynamic_cast<CPRRenderParam *>(imaging->GetRenderer()->GetRenderParam());
			if (param_cpr){
				Vector3f vec_of_interest;
				if (ori == OrientationType::AXIAL){
					vec_of_interest[0] = 0.0f;
					vec_of_interest[1] = 0.0f;
					vec_of_interest[2] = -1.0f;
				}
				else if (ori == OrientationType::SAGITTAL){
					vec_of_interest[0] = 0.0f;
					vec_of_interest[1] = 1.0f;
					vec_of_interest[2] = 0.0f;
				}
				else if (ori == OrientationType::CORONAL){
					vec_of_interest[0] = 1.0f;
					vec_of_interest[1] = 0.0f;
					vec_of_interest[2] = 0.0f;
				}
				else{
					vec_of_interest[0] = 0.0f;
					vec_of_interest[1] = 0.0f;
					vec_of_interest[2] = -1.0f;
				}

				param_cpr->SetDirection(vec_of_interest[0], vec_of_interest[1], vec_of_interest[2]);
			}
		}
		break;
	default:
		break;
	}
}

void RenderFacade::SetOrientation(string control_id, OrientationType ori, float vec[3])
{
	IThreedImaging *imaging = GetControl(control_id);
	if (imaging == NULL) return;

	//TODO finish set orientation
}

void RenderFacade::SetCPRCurveID(string control_id, string curve_id)
{
	IThreedImaging *imaging = GetControl(control_id);
	if (imaging == NULL) return;

	CPRRenderParam *param_cpr = dynamic_cast<CPRRenderParam *>(imaging->GetRenderer()->GetRenderParam());
	if (param_cpr == NULL) return;

	VolCurve *curve = CurveSource::Get()->GetCurve(current_series_uid_, curve_id);
	if (curve == NULL) return;

	param_cpr->SetCurve(curve);
}

void RenderFacade::SetPositionAlongCurve(string control_id, string curve_id, int point_index)
{
	IThreedImaging *imaging = GetControl(control_id);
	if (imaging == NULL) return;

	MPRRenderParam *param_mpr = dynamic_cast<MPRRenderParam *>(imaging->GetRenderer()->GetRenderParam());
	if (param_mpr == NULL) return;

	VolCurve *curve = CurveSource::Get()->GetCurve(current_series_uid_, curve_id);
	if (curve == NULL) return;

	Vector3f vx, vy;
	float ix, iy, iz;
	// Validate point_index is between min/max value
	int number_of_components = curve->GetNumberOfSamplePoint();
	if (point_index >= number_of_components){
		point_index = number_of_components - 1;
	}
	else if (point_index < 0){
		point_index = 0;
	}
	curve->GetSamplePoint(point_index, ix, iy, iz);
	curve->GetLocalPlaneVector(point_index, vx, vy);

	// Set imaging parameters
	param_mpr->SetImageCenter(ix, iy, iz);
	param_mpr->SetPlaneVector(vx, vy);
}

void RenderFacade::SetDesiredUpdateRate(string control_id, float rate)
{
	IThreedImaging *imaging = GetControl(control_id);
	if (imaging == NULL) return;

	VRRenderParam *param_vr = dynamic_cast<VRRenderParam *>(imaging->GetRenderer()->GetRenderParam());
	if (param_vr == NULL) return;

	param_vr->SetDesiredUpdateRate(rate);
}

void RenderFacade::RenderControl(string control_id)
{
	IThreedImaging *imaging = GetControl(control_id);
	if (imaging == NULL) return;

	imaging->Render();
}

void RenderFacade::ResetControl(string control_id)
{
	IThreedImaging *imaging = GetControl(control_id);
	if (imaging == NULL) return;

	//TODO reset control
}

void RenderFacade::ChangeSeries(string series_uid)
{
	current_series_uid_ = series_uid;
	VolData *data = ImageDataSource::Get()->GetVolData(series_uid);
	if (data){
		vector<IThreedImaging *> imaging_list = RenderSource::Get()->GetRenderControls();
		auto it = imaging_list.begin();
		for (; it!=imaging_list.end(); ++it){
			(*it)->SetData(data);
			RenderParam *param = (*it)->GetRenderer()->GetRenderParam();
			if (param){
				// Set default window width & level
				int width, level;
				data->GetDefaultWindowWidthLevel(width, level);
				param->SetWindowWidthLevel(width, level);
			}
			(*it)->GetRenderer()->SetData(data);
		}
	}
}

void RenderFacade::SetImageSize(string control_id, int width, int height)
{
	IThreedImaging *imaging = GetControl(control_id);
	if (imaging == NULL) return;

	RenderParam *param = imaging->GetRenderer()->GetRenderParam();
	if (param == NULL) return;

	param->SetWidth(width);
	param->SetHeight(height);
}

void RenderFacade::SetWindowWidthLevel(string control_id, int width, int level)
{
	IThreedImaging *imaging = GetControl(control_id);
	if (imaging == NULL) return;

	RenderParam *param = imaging->GetRenderer()->GetRenderParam();
	if (param == NULL) return;

	imaging->WindowWidthLevel(width, level);
}

void RenderFacade::SetBlendMode(string control_id, int mode)
{
	IThreedImaging *imaging = GetControl(control_id);
	if (imaging == NULL) return;

	RenderParam *param = imaging->GetRenderer()->GetRenderParam();
	if (param == NULL) return;

	if (mode == 1){
		param->SetBlendMode(BlendMode::MaximumIntensity);
	}
	else if (mode == 2){
		param->SetBlendMode(BlendMode::MinimumIntensity);
	}
	else{
		param->SetBlendMode(BlendMode::AverageIntensity);
	}
}

void RenderFacade::SetSlabThickness(string control_id, float thickness)
{
	IThreedImaging *imaging = GetControl(control_id);
	if (imaging == NULL) return;


}