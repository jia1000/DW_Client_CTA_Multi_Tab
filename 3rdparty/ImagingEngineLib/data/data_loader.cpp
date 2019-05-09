#include "data_loader.h"
#include "vtkSmartPointer.h"
#include "vtkDICOMImageReader.h"
#include "vol_data.h"
#include <vtkStringArray.h>
#include <vtkStdString.h>
#include "data/pixel_data.h"

using namespace DW::IMAGE;
using namespace DW::IO;

VtkDcmLoader::VtkDcmLoader()
{

}

VtkDcmLoader::~VtkDcmLoader()
{
	Close();
	if (vtk_image_reader_){
		vtk_image_reader_->Delete();
	}
}

bool VtkDcmLoader::LoadDirectory(const char* dir)
{
	try
	{
		// initialize dicom image reader
		vtk_image_reader_ = vtkSmartPointer<vtkDICOMImageReader>::New();
		// 1. Read all the DICOM files in the specified directory
		vtk_image_reader_->SetDirectoryName(dir);
		vtk_image_reader_->Update();
		// 2. Set it to vtkImageData object
		//将图像数据缓存到vtk_image_data_指针里
		vtk_image_data_ = vtk_image_reader_->GetOutput();
		double *spacing = vtk_image_reader_->GetPixelSpacing();
		int width = vtk_image_reader_->GetWidth();
		int height = vtk_image_reader_->GetHeight();
		int bits = vtk_image_reader_->GetBitsAllocated();

		// 3. Convert to VolData object
		volume_data_ = new VolData();
		volume_data_->SetBitsPerPixel(16);
		volume_data_->SetBitsStored(16);
		//volume_data_->SetVtkData(vtk_image_data_);
		volume_data_->SetPixelData(new VtkPixelData(vtk_image_data_));
		volume_data_->GetPixelData()->SetBitsPerPixel(16);
		volume_data_->GetPixelData()->SetDimensions(512, 512, 364);
		volume_data_->GetPixelData()->SetOrigin(0, 0, 0);
		volume_data_->GetPixelData()->SetSpacing(0.3671875, 0.3671875, 0.45);
		volume_data_->SetSliceWidth(512);
		volume_data_->SetSliceHeight(512);
		volume_data_->SetSliceCount(364);
		volume_data_->SetSliceFOVWidth(460.00f);
		volume_data_->SetSliceFOVHeight(460.00f);
		volume_data_->SetStudyInstanceUID("1.2.840.113619.2.278.3.679457.166.1526860577.400");
		volume_data_->SetSeriesInstanceUID("1.2.840.113619.2.278.3.679457.166.1526860577.404.3");
		volume_data_->SetBoundingBox(0, 0, 0, 511, 511, 363);
		volume_data_->SetDefaultWindowWidthLevel(413, 82);
		return true;
	}
	catch (...)
	{
		return false;
	}
}
bool VtkDcmLoader::LoadFiles(vector<const char*> files)
{
	// initialize dicom image reader
	vtk_image_reader_ = vtkSmartPointer<vtkDICOMImageReader>::New();

	// 0. Create a vtkStringArray object with file paths
	vtkSmartPointer<vtkStringArray> file_array = vtkSmartPointer<vtkStringArray>::New();	
	char fileName[128];	
	for (int i = 1; i < 100; i++){		
//		sprintf(fileName, "../data/Head/head%03d.jpg", i);		
		vtkStdString fileStr(fileName);
		file_array->InsertNextValue(fileStr);	
	}

	// 1. Read all the DICOM filess specified
	vtk_image_reader_->SetFileNames(file_array);
	vtk_image_reader_->Update();
	// 2. Set it to vtkImageData object
	//将图像数据缓存到vtk_image_data_指针里
	vtk_image_data_ = vtk_image_reader_->GetOutput();
	// 3. Convert to VolData object
	volume_data_ = new VolData();
	volume_data_->SetBitsPerPixel(16);
	volume_data_->SetBitsStored(16);
	//volume_data_->SetVtkData(vtk_image_data_);
	volume_data_->SetPixelData(new VtkPixelData(vtk_image_data_));
	volume_data_->GetPixelData()->SetBitsPerPixel(16);
	volume_data_->GetPixelData()->SetDimensions(512, 512, 285);
	volume_data_->GetPixelData()->SetOrigin(0, 0, 0);
	volume_data_->GetPixelData()->SetSpacing(0.8687, 0.8687, 1.25);
	volume_data_->SetSliceCount(285);
	volume_data_->SetSliceFOVWidth(460.00f);
	volume_data_->SetSliceFOVHeight(460.00f);
	volume_data_->SetStudyInstanceUID("1.2.840.113619.2.278.3.679457.166.1526860577.400");
	volume_data_->SetSeriesInstanceUID("1.2.840.113619.2.278.3.679457.166.1526860577.404.3");
	volume_data_->SetBoundingBox(0,0,0,512,512,285);
	return true;
}
VolData* VtkDcmLoader::GetData()
{
	return volume_data_;
}

void VtkDcmLoader::Close()
{
	if (volume_data_){
		delete volume_data_;
		volume_data_ = NULL;
	}
}