/*=========================================================================

  Program:   ImagingEngine
  Module:    pixel_data.cpp
  author: 	 zhangjian
  Brief:	 

=========================================================================*/
#include "data/pixel_data.h"
#include "tools/vtk_image_data_creator.h"

using namespace DW::IMAGE;

// Macro for a single case of the SWITCH_SCALAR_TYPE macro.
#define SWITCH_SCALAR_TYPE_CASE(vtkTypeName, typeName, code) case VTK_ ## vtkTypeName: \
{                                                                                  \
	typedef typeName ScalarType;                                                   \
	code;                                                                          \
}                                                                                  \
	break;

// This macro implements a switch to handle the different scalar types of pixel data.
// For each supported scalar type, the type is typedefed as ScalarType and code is run.
// For the default case, meaning an unsupported scalar type, defaultCode is run.
#define SWITCH_SCALAR_TYPE(code, defaultCode) switch (scalar_type_)  \
{                                                                    \
	SWITCH_SCALAR_TYPE_CASE(CHAR, char, code)                        \
	SWITCH_SCALAR_TYPE_CASE(SIGNED_CHAR, signed char, code)          \
	SWITCH_SCALAR_TYPE_CASE(UNSIGNED_CHAR, unsigned char, code)      \
	SWITCH_SCALAR_TYPE_CASE(SHORT, short, code)                      \
	SWITCH_SCALAR_TYPE_CASE(UNSIGNED_SHORT, unsigned short, code)    \
	SWITCH_SCALAR_TYPE_CASE(INT, int, code)                          \
	SWITCH_SCALAR_TYPE_CASE(UNSIGNED_INT, unsigned int, code)        \
	SWITCH_SCALAR_TYPE_CASE(LONG, long, code)                        \
	SWITCH_SCALAR_TYPE_CASE(UNSIGNED_LONG, unsigned long, code)      \
	SWITCH_SCALAR_TYPE_CASE(FLOAT, float, code)                      \
	SWITCH_SCALAR_TYPE_CASE(DOUBLE, double, code)                    \
}


RawPixelData::RawPixelData(UNITDATA3D* data)
{
	series_data_ = data;
}

RawPixelData::~RawPixelData()
{
	if (series_data_){
		delete series_data_;
		series_data_ = NULL;
	}
}

UNITDATA3D* RawPixelData::GetPixelData()
{
	return series_data_;
}

vtkImageData* RawPixelData::GetVtkImageData()
{
	UNITDATA3D* pixel_data = this->GetPixelData();
	// 转换到vtkImageData
	if (vtk_series_data_){
		vtk_series_data_->Delete();
		vtk_series_data_ = NULL;
	}
	int dim[3] = { 0 };
	double origin[3] = { 0.0 }, spacing[3] = { 0.0 };
	this->GetDimensions(dim);
	this->GetOrigin(origin);
	this->GetSpacing(spacing);
	BYTE bits = this->GetBitsPerPixel();

	unsigned char* ptr = this->GetPixelData();
	VtkImageDataCreator imageDataCreator;
	imageDataCreator.SetOrigin(origin);
	imageDataCreator.SetSpacing(spacing);
	imageDataCreator.SetDimensions(dim);
	imageDataCreator.SetNumberOfComponents(bits / 8);
	vtk_series_data_ = imageDataCreator.Create(ptr);
	
	return vtk_series_data_;
}

void *RawPixelData::GetDataPointer(int x, int y, int z)
{
	if (this->Dimensions &&
		x < this->Dimensions[0] &&
		y < this->Dimensions[1] &&
		z < this->Dimensions[2])
	{
		return series_data_ + (x + y * Dimensions[0] + z * Dimensions[0] * Dimensions[1]) * (bits_per_pixel_ / 8);
	}
	return NULL;
}

VtkPixelData::VtkPixelData(vtkImageData* data)
	: data_pointer_(data->GetScalarPointer())
	, scalar_type_(data->GetScalarType())
{
	vtk_series_data_ = data;
}

VtkPixelData::~VtkPixelData()
{
	if (vtk_series_data_){
		vtk_series_data_->Delete();
		vtk_series_data_ = NULL;
	}
}

vtkImageData* VtkPixelData::GetVtkImageData()
{
	return vtk_series_data_;
}

void *VtkPixelData::GetDataPointer(int x, int y, int z)
{
	// 使用data_pointer_可以降低50%以上的时间消耗
	if (data_pointer_){
		int pos = z * (Dimensions[0] * Dimensions[1]) + (Dimensions[1] - 1 - y) * Dimensions[0] + x;
		return static_cast<char*>(data_pointer_) + pos * ScalarSize();
	}

	if (this->Dimensions &&
		x < this->Dimensions[0] &&
		y < this->Dimensions[1] &&
		z < this->Dimensions[2])
	{
		return vtk_series_data_->GetScalarPointer(x, Dimensions[1] - 1 - y, z);
	}
	return NULL;
}

size_t VtkPixelData::ScalarSize() const
{
	SWITCH_SCALAR_TYPE(return sizeof(ScalarType),
		return 0)
}