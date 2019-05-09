/*=========================================================================

  Program:   ImagingEngine
  Module:    rotate_cpr.cpp
  author: 	 zhangjian
  Brief:	 

=========================================================================*/
#include "actor/rotate_cpr.h"
#include "tools/logger.h"
#include "render/render_param_cpr.h"
#include "tools/math.h"

using namespace DW::IMAGE;
using namespace DW::ACTOR;
using namespace std;

CPRRotateActor::CPRRotateActor()
{

}
CPRRotateActor::~CPRRotateActor()
{

}

// 绕着指定轴旋转
void CPRRotateActor::Execute(ActorArgs* args)
{
	CPRRotateArgs* cpr_rotate =  dynamic_cast<CPRRotateArgs*>(args);
	if (imaging_== nullptr || cpr_rotate == nullptr) return;
	float angle = cpr_rotate->GetAngle();

	imaging_->Rotate(angle);
	//imaging_->Zoom(0.5);
	CGLogger::Info("CPRRotateActor execute. Angle=" + to_string(angle));
}