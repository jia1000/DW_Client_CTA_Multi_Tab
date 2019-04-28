/*=========================================================================

  Program:   ImagingEngine
  Module:    render_manager.h
  author: 	 zhangjian
  Brief:	 

=========================================================================*/
#pragma once

#include "data/data_definition.h"
#include "render/imaging_3d.h"
#include "builder/imaging_builder_3d.h"
#include "data_source.h"
#include "imaging_source.h"

using namespace DW::IMAGE;
using namespace DW::Builder;

namespace DW {
	
	class RenderManager
	{
	public:
		~RenderManager() { instance_ = nullptr; }
	
		static RenderManager* Get() 
		{
			if (instance_ == nullptr)
				instance_ = new RenderManager;
			return instance_;
		}


	private:
		static RenderManager* instance_;
		map<string, IThreedImagingBuilder *> builder_set_;
		
	};

}