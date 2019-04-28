#pragma once

#include "light.h"

using namespace DW::IMAGE;
using namespace std;


Light::Light()
{

}
Light::~Light()
{

}

void Light::SetLight(vtkSmartPointer<vtkLight> light) 
{
	vtk_light_ = light;
}

vtkSmartPointer<vtkLight> Light::GetLight() 
{
	return vtk_light_;
}