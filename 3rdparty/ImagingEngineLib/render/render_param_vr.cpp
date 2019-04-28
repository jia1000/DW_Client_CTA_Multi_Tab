#include "render_param_vr.h"

using namespace DW::IMAGE;
using namespace DW::Render;

VRRenderParam::VRRenderParam()
{
	// size
	width_ = 512;
	height_ = 512;
	// lighting parameters
	ambient_ = 0.2;
	diffuse_ = 0.7;
	specular_ = 0.3;
	specular_power_ = 8.0;
	// color transfer
	color_func_ = vtkSmartPointer<vtkColorTransferFunction>::New();
	color_func_->AddRGBPoint(190.0, 0.4, 0.2, 0.00);
	color_func_->AddRGBPoint(2000, 1.0, 1.0, 1.0);
	// opacity
	opacity_func_ = vtkSmartPointer<vtkPiecewiseFunction>::New();
	opacity_func_->AddPoint(-800.0, 0.0);
	opacity_func_->AddPoint(-750.0, 0.0);
	opacity_func_->AddPoint(-350.0, 0.0);
	opacity_func_->AddPoint(-300.0, 0.0);
	opacity_func_->AddPoint(-200.0, 0.0);
	opacity_func_->AddPoint(0.0, 0.0);
	opacity_func_->AddPoint(10.0, 0.05);
	opacity_func_->AddPoint(500.0, 0.65);
	opacity_func_->AddPoint(1200.0, 0.9);
	opacity_func_->AddPoint(2750.0, 1.0);
	// gradient
	gradient_func_ = vtkSmartPointer<vtkPiecewiseFunction>::New();
	gradient_func_->AddPoint(10, 0.0);
	gradient_func_->AddPoint(90, 0.5);
	gradient_func_->AddPoint(100, 1.0);
	// camera
	camera_ = new Camera();	
	camera_->GetCamera()->SetViewUp(0, -1, 0);
	camera_->GetCamera()->Elevation(-90);
	//// light
	//light_ = new Light();
}

VRRenderParam::~VRRenderParam()
{
	color_func_->Delete();
	opacity_func_->Delete();
	gradient_func_->Delete();
}
float VRRenderParam::GetAmbient()
{
	return ambient_;
}
void VRRenderParam::SetAmbient(float v)
{
	ambient_ = v;
}
float VRRenderParam::GetDiffuse()
{
	return diffuse_;
}
void VRRenderParam::SetDiffuse(float v)
{
	diffuse_ = v;
}
float VRRenderParam::GetSpecular()
{
	return specular_;
}
void VRRenderParam::SetSpecular(float v)
{
	specular_ = v;
}
float VRRenderParam::GetSpecularPower()
{
	return specular_power_;
}
void VRRenderParam::SetSpecularPower(float v)
{
	specular_power_ = v;
}
vtkSmartPointer<vtkColorTransferFunction> VRRenderParam::GetColorFunction()
{
	return color_func_;
}
void VRRenderParam::SetColorFunction(vtkSmartPointer<vtkColorTransferFunction> func)
{
	color_func_ = func;
}
vtkSmartPointer<vtkPiecewiseFunction> VRRenderParam::GetOpacityFunction()
{
	return opacity_func_;
}
void VRRenderParam::SetOpacityFunction(vtkSmartPointer<vtkPiecewiseFunction> func)
{
	opacity_func_ = func;
}

vtkSmartPointer<vtkPiecewiseFunction> VRRenderParam::GetGradientFunction()
{
	return gradient_func_;
}

void VRRenderParam::SetGradientFunction(vtkSmartPointer<vtkPiecewiseFunction> func)
{
	gradient_func_ = func;
}
