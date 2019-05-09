/*=========================================================================

  Program:   ImagingEngine
  Module:    pixel_data_output.h
  author: 	 zhangjian
  Brief:	 pixeldata数据格式转化
			 从字节流数据转换为vtkImageData格式

=========================================================================*/
#pragma once

#include "vtk_include.h"
#include "data_definition.h"
#include "data/pixel_data.h"

namespace DW {
	namespace IMAGE{

		class IPixelDataOutput
		{
		public:
			virtual void* GetData() = 0;

		protected:
			void SetPixelDataSource(IPixelData* data)
			{
				pixel_data_ = data;
			}
		protected:
			IPixelData* pixel_data_;
		};

		class VtkPixelDataOutput : public IPixelDataOutput
		{
		public:
			VtkPixelDataOutput(IPixelData* data)
			{
				pixel_data_ = data;
			}
			void* GetData() override
			{
				if (pixel_data_->IsModified() == false && vtk_series_data_)
					return vtk_series_data_;

				VtkPixelData* vtk_pixel = reinterpret_cast<VtkPixelData*>(pixel_data_);
				if (vtk_pixel){
					vtk_series_data_ = vtk_pixel->GetPixelData();
				}
				else{
					RawPixelData* raw_pixel = reinterpret_cast<RawPixelData*>(pixel_data_);
					if (raw_pixel){
						UNITDATA3D* pixel_data = raw_pixel->GetPixelData();
						// 转换到vtkImageData
						if (vtk_series_data_){
							vtk_series_data_->Delete();
							vtk_series_data_ = NULL;
						}
						vtk_series_data_ = vtkSmartPointer<vtkImageData>::New();
						int dim[3] = { 0 };
						double origin[3] = { 0.0 }, spacing[3] = { 0.0 };
						raw_pixel->GetDimensions(dim);
						raw_pixel->GetOrigin(origin);
						raw_pixel->GetSpacing(spacing);
						BYTE bits = raw_pixel->GetBitsPerPixel();
						vtk_series_data_->SetOrigin( origin );
						vtk_series_data_->SetSpacing ( spacing );
						vtk_series_data_->SetDimensions( dim[0], dim[1], dim[2] );
						if (bits == 8){
							vtk_series_data_->SetScalarTypeToUnsignedChar();
							vtk_series_data_->SetNumberOfScalarComponents(1);
							vtk_series_data_->AllocateScalars();

							memcpy((unsigned char *)vtk_series_data_->GetScalarPointer(), reinterpret_cast<unsigned short *>(raw_pixel->GetPixelData()), dim[0]*dim[1]);
							vtk_series_data_->Modified();
						}
						else if (bits == 16){
							vtk_series_data_->SetScalarTypeToUnsignedShort();
							vtk_series_data_->SetNumberOfScalarComponents(1);
							vtk_series_data_->AllocateScalars();

							memcpy((unsigned short *)vtk_series_data_->GetScalarPointer(), reinterpret_cast<unsigned short *>(raw_pixel->GetPixelData()), dim[0]*dim[1]);
							vtk_series_data_->Modified();
						}
						else if (bits == 32){
							vtk_series_data_->SetScalarTypeToUnsignedInt();
							vtk_series_data_->SetNumberOfScalarComponents(1);
							vtk_series_data_->AllocateScalars();

							memcpy((unsigned int *)vtk_series_data_->GetScalarPointer(), reinterpret_cast<unsigned short *>(raw_pixel->GetPixelData()), dim[0]*dim[1]);
							vtk_series_data_->Modified();
						}

					}
				}
				return vtk_series_data_;
			}

		protected:
			vtkImageData* vtk_series_data_;
		};

	}
}
