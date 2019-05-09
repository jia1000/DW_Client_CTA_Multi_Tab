/*=========================================================================

  Program:   ImagingEngine
  Module:    data_source.h
  author: 	 zhangjian
  Brief:	 

=========================================================================*/
#pragma once

#include "data/data_definition.h"
#include "data/vol_data.h"

namespace DW {

	class ImageDataSource
	{
	public:
		~ImageDataSource() { instance_ = nullptr; }

		static ImageDataSource *Get();

		void AddDirectory(const char *directory);
		void AddFiles(vector<const char *> files);
		void AppendFile(const char *file);
		void AddVolData(string series_uid, VolData *data);
		VolData *GetVolData(string series_uid);
		void Destroy(string series_uid);
		void DestroyAll();

	private:
		ImageDataSource() {}

	private:
		static ImageDataSource *instance_;
		map<string, VolData *> volume_data_set_;
	};

}