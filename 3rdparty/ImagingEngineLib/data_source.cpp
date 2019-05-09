/*=========================================================================

  Program:   ImagingEngine
  Module:    data_source.cpp
  author: 	 zhangjian
  Brief:	 

=========================================================================*/
#include "data_source.h"

using namespace DW;
using namespace DW::IMAGE;


ImageDataSource *ImageDataSource::instance_ = 0;
ImageDataSource *ImageDataSource::Get() 
{
	if (instance_ == nullptr)
		instance_ = new ImageDataSource;
	return instance_;
}

void ImageDataSource::AddDirectory(const char *directory)
{

}
void ImageDataSource::AddFiles(vector<const char *> files)
{

}
void ImageDataSource::AppendFile(const char *file)
{

}
void ImageDataSource::AddVolData(string series_uid, VolData *data)
{
	if (volume_data_set_.count(series_uid) > 0){
		volume_data_set_[series_uid]->Destroy();
		volume_data_set_.erase(series_uid);
	}
	volume_data_set_.insert(pair<string, VolData *>(series_uid, data));
}
VolData* ImageDataSource::GetVolData(string series_uid)
{
	if (volume_data_set_.count(series_uid) > 0){
		return volume_data_set_[series_uid];
	}
	return NULL;
}
void ImageDataSource::Destroy(string series_uid)
{
	if (volume_data_set_.count(series_uid) > 0){
		volume_data_set_[series_uid]->Destroy();
		volume_data_set_.erase(series_uid);
	}
}
void ImageDataSource::DestroyAll()
{
	auto it = volume_data_set_.begin();
	while(it != volume_data_set_.end())
	{
		it->second->Destroy();
		it++;         
	}
	volume_data_set_.clear();
}