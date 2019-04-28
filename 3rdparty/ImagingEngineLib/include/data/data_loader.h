/*=========================================================================

  Program:   ImagingEngine
  Module:    data_loader.h
  author: 	 zhangjian
  Brief:	 ”√vtkDICOMImageReader¿¥∂¡»°dicomÕºœÒ

=========================================================================*/
#pragma once

#include "vtk_include.h"
#include "data_definition.h"
#include "vol_data.h"
#include "io/dcm_reader.h"

using namespace DW::IMAGE;
using namespace std;

namespace DW {
	namespace IO{

		class VtkDcmLoader : public IDicomReader
		{
		public:
			VtkDcmLoader();
			~VtkDcmLoader();
			bool LoadDirectory(const char* dir) override;
			bool LoadFiles(vector<const char*> files) override;
			VolData* GetData() override;
			void Close() override;
		protected: 
			UNITDATA3D* buffer_data_;			//pixel data for display

		private:
			DataConvertUtil* data_convret_;
			VolData* volume_data_;
			vtkSmartPointer<vtkImageData> vtk_image_data_;
			vtkSmartPointer<vtkDICOMImageReader> vtk_image_reader_;
		};
	}
}