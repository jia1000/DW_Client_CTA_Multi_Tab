/*=========================================================================

  Program:   ImagingEngine
  Module:    dcm_reader.h
  author: 	 zhangjian
  Brief:	 定义dicom图像加载接口

=========================================================================*/
#pragma once

#include "data_definition.h"
#include "vol_data.h"

using namespace std;

namespace DW {
	namespace IO{

		///
		// define a interface to load dicom files
		///
		class IDicomReader
		{
		public:
			virtual bool LoadDirectory(const char* dir) = 0;
			virtual bool LoadFiles(vector<const char*> files) = 0;
			virtual bool LoadVolumeMask(const char* file) = 0;
			virtual VolData* GetData() = 0;
			virtual void Close() = 0;
		};
		
	}
}