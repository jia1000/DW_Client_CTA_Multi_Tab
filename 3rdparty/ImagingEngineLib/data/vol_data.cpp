
#include "vol_data.h"

using namespace DW::IMAGE;

VolData::VolData()
{
	bounding_box_ = new BoundingBox();
	slice_width_ = 512;
	slice_height_ = 512;
}

VolData::~VolData()
{

}

UNITDATA3D* VolData::GetData()
{
	return series_data_;
}
vtkImageData* VolData::GetVtkData()
{
	return vtk_series_data;
}
void VolData::SetBitsPerPixel(BYTE bits)
{
	bits_per_pixel_ = bits;
}
void VolData::SetBitsStored(BYTE bits)
{
	bits_stored_ = bits;
}
void VolData::SetData(UNITDATA3D* data)
{
	series_data_ = data;
}
void VolData::SetMark(UNITMARK3D* mark)
{
	series_mark_ = mark;
}
void VolData::SetVtkData(vtkImageData* data)
{
	vtk_series_data = data;
}
void VolData::SetSliceWidth(int width)
{
	slice_width_ = width;
}
void VolData::SetSliceHeight(int height)
{
	slice_height_ = height;
}
void VolData::SetSliceCount(int count)
{
	slice_count_ = count;
}
void VolData::SetSliceFOVWidth(int fov_w)
{
	slice_fov_width_ = fov_w;
}
void VolData::SetSliceFOVHeight(int fov_h)
{
	slice_fov_height_ = fov_h;
}
void VolData::SetStudyInstanceUID(string uid)
{
	study_instance_uid_ = uid;
}
void VolData::SetSeriesInstanceUID(string uid)
{
	series_instance_uid_ = uid;
}
void VolData::SetBoundingBox(float xmin,float ymin,float zmin,float xmax,float ymax,float zmax)
{
	bounding_box_->Update(xmin, ymin, zmin, xmax, ymax, zmax);
}
void *VolData::GetDataPointer(int x, int y, int z)
{
	if (vtk_series_data){
		return vtk_series_data->GetScalarPointer(x, y, z);
	}
	return series_data_ + (x + y * slice_width_ + z * slice_width_ * slice_height_) * (bits_per_pixel_ / 8);
}
void *VolData::GetDataPointer()
{
	if (vtk_series_data){
		return vtk_series_data->GetScalarPointer();
	}
	return series_data_;
}
void VolData::Destroy() {
	if (series_data_){
		delete series_data_;
		series_data_ = NULL;
	}
	if (series_mark_){
		delete series_mark_;
		series_mark_ = NULL;
	}
	if (vtk_series_data){
		vtk_series_data->Delete();
	}
	if (bounding_box_){
		delete bounding_box_;
		bounding_box_ = NULL;
	}
	delete this;
}