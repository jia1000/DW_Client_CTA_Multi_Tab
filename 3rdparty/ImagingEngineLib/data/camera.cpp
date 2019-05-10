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

void Camera::RotateX(float angle)
{
	if (vtk_camera_){
		// Rotate the camera about the cross product of the negative of the
		// direction of projection and the view up vector, using the focal point
		// as the center of rotation.  The result is a vertical rotation of the
		// scene.
		vtk_camera_->Elevation(angle);
	}
}

void Camera::RotateY(float angle)
{
	if (vtk_camera_){
		// Rotate the camera about the view up vector centered at the focal point.
		// Note that the view up vector is whatever was set via SetViewUp, and is
		// not necessarily perpendicular to the direction of projection.  The
		// result is a horizontal rotation of the camera.
		vtk_camera_->Azimuth(angle);
	}
}

void Camera::RotateZ(float angle)
{
	// No longer used cause of opencv processing
	if (vtk_camera_){
		// Rotate the camera about the direction of projection.  This will
		// spin the camera about its axis.
		vtk_camera_->Roll(angle);
	}
}

void Camera::RotateWXYZ(float angle, float x, float y, float z)
{
	if (vtk_camera_){

		
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