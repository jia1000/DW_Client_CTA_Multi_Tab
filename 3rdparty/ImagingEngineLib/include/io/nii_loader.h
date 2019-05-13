/*=========================================================================

  Program:   ImagingEngine
  Module:    nii_loader.h
  author: 	 zhangjian
  Brief:	 

=========================================================================*/
#pragma once

#include "vtk_include.h"
#include "data_definition.h"
#include "vol_data.h"
#include "io/dcm_reader.h"

using namespace std;

namespace DW {
	namespace IO {

		class NiiImageLoader : public IDicomReader
		{
		public:
			NiiImageLoader();
			virtual ~NiiImageLoader();

			/// IDicomReader implementation
			bool LoadDirectory(const char* dir) override;
			bool LoadFiles(vector<const char*> files) override;
			bool LoadVolumeMask(const char* file) override;
			VolData* GetData() override;
			void Close() override;

		protected: 
			DataConvertUtil* data_convret_;
			VolData* volume_data_;
			vtkSmartPointer<vtkImageData> vtk_image_data_;
			vtkSmartPointer<vtkDICOMImageReader> vtk_image_reader_;

		};

		class NiiImageLoader2 : NiiImageLoader
		{
		public:
			NiiImageLoader2();
			~NiiImageLoader2();

			bool LoadFiles(vector<const char*> files) override;
			
		};

	}
}
