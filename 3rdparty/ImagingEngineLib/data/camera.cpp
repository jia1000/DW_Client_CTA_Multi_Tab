#pragma once

#include "camera.h"

using namespace DW::IMAGE;
using namespace std;


Camera::Camera()
{
	//vtk_camera_ = vtkCamera::New();
	//vtk_camera_ = vtkSmartPointer<vtkCamera>::New();
}
Camera::~Camera()
{
	vtk_camera_->Delete();
}

void Camera::Rotate(float angle)
{
	if (vtk_camera_){

		vtk_camera_->Roll(angle);
	}
}

void Camera::RotateWXYZ(float angle, float x, float y, float z)
{
	if (vtk_camera_){

		//vtk_camera_->Azimuth(angle);
	}
}

void Camera::Rotate(float x, float y, float angle)
{
	if (vtk_camera_){
		//vtk_camera_->Elevation(-90 + angle);
	}
}

void Camera::Zoom(float factor)
{
	if (vtk_camera_){
		vtk_camera_->Zoom(factor);
	}
}

void Camera::SetVtkCamera(vtkSmartPointer<vtkCamera> camera) 
{
	vtk_camera_ = camera;
}
			
vtkSmartPointer<vtkCamera> Camera::GetVtkCamera() 
{
	return vtk_camera_;
}

void Camera::DeepCopy(Camera* source)
{
	this->vtk_camera_->DeepCopy(source->GetVtkCamera());
	//...
}

void Camera::SetCameraViewPlane(OrientationType ori)
{
	// Adjust camera settings According to view
	vtk_camera_->SetFocalPoint(0.0, 0.0, 0.0);
	switch (ori)
	{
	case OrientationType::AXIAL:
		vtk_camera_->SetViewUp(0.0, -1.0, 0.0);
		vtk_camera_->SetPosition(0.0, 0.0, -1.0);
		break;

	case OrientationType::SAGITTAL:
		vtk_camera_->SetViewUp(0.0, 0.0, 1.0);
		vtk_camera_->SetPosition(1.0, 0.0, 0.0);
		break;

	case OrientationType::CORONAL:
		vtk_camera_->SetViewUp(0.0, 0.0, 1.0);
		vtk_camera_->SetPosition(0.0, -1.0, 0.0);
		break;
	}
}