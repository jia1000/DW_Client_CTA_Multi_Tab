/*=========================================================================

  Program:   ImagingEngine
  Module:    window_width_level.cpp
  author: 	 zhangjian
  Brief:	 

=========================================================================*/
#include "actor/window_width_level.h"
#include "tools/logger.h"

using namespace DW::ACTOR;

WindowWidthLevelActor::WindowWidthLevelActor()
{

}

WindowWidthLevelActor::~WindowWidthLevelActor()
{

}

void WindowWidthLevelActor::Execute(ActorArgs* args)
{
	WindowWidthLevelArgs* window_args =  dynamic_cast<WindowWidthLevelArgs*>(args);
	if (imaging_== nullptr || window_args == nullptr) return;

	int ww = window_args->GetWindowWidth();
	int wl = window_args->GetWindowLevel();

	imaging_->WindowWidthLevel(ww, wl);

	CGLogger::Info("WindowWidthLevelActor execute. width=" + to_string(ww) + " level=" + to_string(wl));
}