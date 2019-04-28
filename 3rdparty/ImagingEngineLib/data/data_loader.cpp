#include "data_loader.h"
#include "vtkSmartPointer.h"
#include "vtkDICOMImageReader.h"
#include "vol_data.h"
#include <vtkStringArray.h>
#include <vtkStdString.h>

using namespace DW::IMAGE;
using namespace DW::IO;

VtkDcmLoader::VtkDcmLoader()
{

}

VtkDcmLoader::~VtkDcmLoader()
{

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
		// 3. Convert to VolData object
		volume_data_ = new VolData();
		volume_data_->SetBitsPerPixel(16);
		volume_data_->SetBitsStored(16);
		volume_data_->SetVtkData(vtk_image_data_);
		volume_data_->SetSliceWidth(512);
		volume_data_->SetSliceHeight(512);
		volume_data_->SetSliceCount(285);
		volume_data_->SetSliceFOVWidth(460.00f);
		volume_data_->SetSliceFOVHeight(460.00f);
		volume_data_->SetStudyInstanceUID("1.2.840.113619.2.278.3.679457.166.1526860577.400");
		volume_data_->SetSeriesInstanceUID("1.2.840.113619.2.278.3.679457.166.1526860577.404.3");
		volume_data_->SetBoundingBox(0,0,0,512,512,285);
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
		sprintf(fileName, "../data/Head/head%03d.jpg", i);		
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
	volume_data_->SetVtkData(vtk_image_data_);
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