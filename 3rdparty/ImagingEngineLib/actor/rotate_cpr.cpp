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

	IThreedRenderer* renderer = imaging_->GetRenderer();
	if (renderer){
		CPRRenderParam* param = dynamic_cast<CPRRenderParam*>(renderer->GetRenderParam());
		if (param){

			//if (param->GetRenderMode() == CPRRenderMode::STRETCHED_CPR_VER ||
			//	param->GetRenderMode() == CPRRenderMode::STRETCHED_CPR_HOR){
					float axis[3]={0}, direction[3]={0}, new_direction[3]={0};
					param->GetAxis(axis[0], axis[1], axis[2]);
					param->GetDirection(direction[0], direction[1], direction[2]);

					MathTool::rotate_axis(axis[0], axis[1], axis[2], 
						direction[0], direction[1], direction[2], 
						new_direction[0], new_direction[1], new_direction[2],
						angle);

					param->SetDirection(new_direction[0], new_direction[1], new_direction[2]);
			//}
			//else if (param->GetRenderMode() == CPRRenderMode::STRAIGHTENED_CPR_VER ||
			//	param->GetRenderMode() == CPRRenderMode::STRAIGHTENED_CPR_HOR){
					param->SetAngle(angle);
			//}
			
			renderer->Render();
		}		

		CGLogger::Info("CPRRotateActor execute. Angle=" + to_string(angle));
	}
}