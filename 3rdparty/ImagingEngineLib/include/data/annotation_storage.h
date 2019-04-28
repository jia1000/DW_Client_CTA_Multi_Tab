/*=========================================================================

  Program:   ImagingEngine
  Module:    annotation_storage.h
  author: 	 zhangjian
  Brief:	 

=========================================================================*/
#pragma once

#include "data_definition.h"
#include "vector3.h"
#include "imaging_3d.h"
#include "render/pick_3d.h"

using namespace std;

namespace DW {
	namespace IMAGE {

		class AnnotationManager
		{
		public:
			~AnnotationManager() { instance = nullptr; }

			static AnnotationManager* Get() 
			{
				if (instance == nullptr)
					instance = new AnnotationManager;
				return instance;
			}

			void AddAnnotation(ThreedAnnotation* anno) { annotation_cache_.push_back(anno); }

		private:
			AnnotationManager();

		private:
			static AnnotationManager* instance;
			vector<ThreedAnnotation*> annotation_cache_;
		};

		class ThreedAnnotation
		{
		public:
			void AddPoint(Vector3d pnt) { points_.push_back(pnt); }
		private:
			vector<Vector3d> points_;

		};
	}
}