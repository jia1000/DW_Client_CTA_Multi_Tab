/*=========================================================================

  Program:   ImagingEngine
  Module:    actor_3d.h
  author: 	 zhangjian
  Brief:	 三维图像操作命令基类

=========================================================================*/
#pragma once

#include "data_definition.h"
#include "vector3.h"
#include "imaging_3d.h"

using namespace DW::Render;
using namespace std;
//
namespace DW {
	namespace ACTOR{
		
		class ActorArgs;

		///
		// 三维图像操作命令基类
		///
		class IThreeDActor
		{
		public:
			///
			// 执行命令
			///
			virtual void Execute(ActorArgs* args) = 0;
			///
			// 设置IThreedImaging对象
			///
			void SetImaging(IThreedImaging* obj) { imaging_ = obj; }

		protected:
			IThreedImaging* imaging_;
			void* result_;
		};

		///
		// 命令参数基类
		///
		class ActorArgs
		{
		public:
			ActorArgs() {};
			virtual ~ActorArgs() {};

		protected:
			// 是否鼠标移动事件
			bool mouse_moving_;
			// 响应事件的鼠标：左键1/中键2/右键3
			BYTE mouse_;
		};
	}
}