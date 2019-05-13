
#include "vol_data.h"
#include "tools/vtk_image_data_creator.h"

using namespace DW::IMAGE;

VolData::VolData()
{
	bounding_box_ = new BoundingBox();
	slice_width_ = 512;
	slice_height_ = 512;
}

VolData::~VolData()
{
	Destroy();
}

IPixelData* VolData::GetPixelData()
{
	return pixel_data_;
}

void VolData::SetPixelData(IPixelData* data)
{
	pixel_data_ = data;
}

void VolData::SetBitsPerPixel(BYTE bits)
{
	bits_per_pixel_ = bits;
}
void VolData::SetBitsStored(BYTE bits)
{
	bits_stored_ = bits;
}

void VolData::SetMark(UNITMARK3D* mark)
{
	series_mark_ = mark;

	//if (series_mark_){
	//	int dims[3];
	//	pixel_data_->GetDimensions(dims);
	//	double spacings[3];
	//	pixel_data_->GetSpacing(spacings);

	//	// Set volume mask data
	//	//vtkSmartPointer<vtkImageImport> image_import = vtkSmartPointer<vtkImageImport>::New();
	//	//image_import->SetDataSpacing(spacings);
	//	//image_import->SetDataOrigin(0, 0, 0);
	//	//image_import->SetWholeExtent(0, dims[0], 0, dims[1], 0, dims[2]);
	//	//image_import->SetDataExtentToWholeExtent();
	//	//image_import->SetDataScalarTypeToUnsignedChar();
	//	//image_import->SetNumberOfScalarComponents(1);
	//	//image_import->SetImportVoidPointer(mark);
	//	//image_import->Update();
	//	//vtk_series_mark_ = image_import->GetOutput();


	//	double origins[3] = {0.0, 0.0, 0.0};
	//	VtkImageDataCreator imageDataCreator;
	//	imageDataCreator.SetOrigin(origins);
	//	imageDataCreator.SetSpacing(spacings);
	//	imageDataCreator.SetDimensions(dims);
	//	imageDataCreator.SetNumberOfComponents(1);
	//	vtk_series_mark_ = imageDataCreator.Create(mark);

	//	delete series_mark_;
	//	series_mark_ = NULL;

	//}
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
	if (pixel_data_){
		return pixel_data_->GetDataPointer(x, y, z);
	}
	return NULL;
}

void VolData::Destroy() {
	if (pixel_data_){
		delete pixel_data_;
		pixel_data_ = NULL;
	}
	if (series_mark_){
		delete series_mark_;
		series_mark_ = NULL;
	}
	if (bounding_box_){
		delete bounding_box_;
		bounding_box_ = NULL;
	}
	delete this;
}

void VolData::GetDefaultWindowWidthLevel(int &width, int &level)
{
	width = window_width_;
	level = window_level_;
}

void VolData::SetDefaultWindowWidthLevel(int width, int level)
{
	window_width_ = width;
	window_level_ = level;
}