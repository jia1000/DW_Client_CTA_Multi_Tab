/*=========================================================================

  Program:   ImagingEngine
  Module:    rotate_vr.cpp
  author: 	 zhangjian
  Brief:	 

=========================================================================*/
#pragma once

#include "actor/rotate_vr.h"
#include "tools/logger.h"

using namespace DW::ACTOR;
using namespace std;

VRRotateActor::VRRotateActor()
{

}
VRRotateActor::~VRRotateActor()
{

}

// 绕着指定轴旋转
void VRRotateActor::Execute(ActorArgs* args)
{
	VRRotateArgs* vr_rotate =  dynamic_cast<VRRotateArgs*>(args);
	if (vr_rotate == nullptr) return;

	if (imaging_){
		IThreedRenderer* renderer = imaging_->GetRenderer();
		if (renderer){
			Camera* camera = renderer->GetCamera();
			if (camera){
				camera->Rotate(vr_rotate->GetAngle());

				CGLogger::Info("VRRotateActor execute. Angle=" + to_string(vr_rotate->GetAngle()));
			}
		}
	}
}
// 绕着指定轴旋转
void VRRotateActor::Rotate(Vector3d axis, float angle)
{
	if (imaging_){
		IThreedRenderer* renderer = imaging_->GetRenderer();
		if (renderer){
			Camera* camera = renderer->GetCamera();
			if (camera){
				camera->Rotate(axis, angle);
			}
		}
	}
}

// 绕着平面轴旋转
void VRRotateActor::Rotate(float x, float y, float angle)
{
	if (imaging_){
		IThreedRenderer* renderer = imaging_->GetRenderer();
		if (renderer){
			Camera* camera = renderer->GetCamera();
			if (camera){
				camera->Rotate(x, y, angle);
			}
		}
	}
}
