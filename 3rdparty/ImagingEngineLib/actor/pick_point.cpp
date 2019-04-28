/*=========================================================================

  Program:   ImagingEngine
  Module:    pick_point.cpp
  author: 	 zhangjian
  Brief:	 

=========================================================================*/
#include "actor/pick_point.h"

using namespace DW::ACTOR;

PickPointActor::PickPointActor()
{

}

PickPointActor::~PickPointActor()
{

}

void PickPointActor::Execute(ActorArgs* args)
{
	IThreedRenderer* renderer = imaging_->GetRenderer();
	PickPointArgs* pick_args = dynamic_cast<PickPointArgs*>(args);
	if (renderer){
		IThreedPickHandler* pick = renderer->GetThreedPickHandler();
		if (NULL != pick)
		{
			Vector3d vec3;
			// 从二维图像点获得三维数据场中的点
			pick->PickByImagePos((int)pick_args->GetImageX(), (int)pick_args->GetImageY(), vec3);
		}
	}
}