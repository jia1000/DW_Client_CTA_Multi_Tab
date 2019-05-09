/*=========================================================================

  Program:   ImagingEngine
  Module:    data_format.h
  author: 	 zhangjian
  Brief:	 定义存储pixeldata数据的类
			 RawPixelData: 通过dcmtk读取的pixeldata
			 VtkPixelData: 通过vtk/itk读取的vtkImageData

=========================================================================*/
#pragma once


#include "vtk_include.h"
#include "data_definition.h"
#include "data_convert.h"
#include "data/box3.h"
#include "data/aabb.h"

using namespace DW::IMAGE;
using namespace std;

namespace DW {
	namespace IMAGE{

		class IPixelData
		{
		public:
			virtual vtkImageData* GetVtkImageData() = 0;
			/// 从数据指针中获取指定坐标位置的值
			virtual void *GetDataPointer(int x, int y, int z) = 0;

			bool IsModified()
			{
				return is_modified_;
			}
			/// Set dimensions of the volume data
			void SetDimensions(int x, int y, int z)
			{
				Dimensions[0] = x;
				Dimensions[1] = y;
				Dimensions[2] = z;
			}
			/// Get dimensions of the volume data
			void GetDimensions(int dim[3])
			{
				for (int i=0; i<3; ++i){
					dim[i] = Dimensions[i];
				}
			}
			/// Set origin point of the volume data
			void SetOrigin(double x, double y, double z)
			{
				Origin[0] = x;
				Origin[1] = y;
				Origin[2] = z;
			}
			/// Get origin point of the volume data
			void GetOrigin(double origin[3])
			{
				for (int i=0; i<3; ++i){
					origin[i] = Origin[i];
				}
			}
			/// Set pixel spacing (x,y,z) of the volume data
			/// x and y spacings are stored in PixelSpacing tag [0028,0030]
			/// z spacing (average value) should be calculated from ImagePositionPatient tag [0020,0032] 
			void SetSpacing(double x, double y, double z)
			{
				Spacing[0] = x;
				Spacing[1] = y;
				Spacing[2] = z;
			}
			/// Get pixel spacing (x,y,z) of the volume data
			void GetSpacing(double spacing[3])
			{
				for (int i=0; i<3; ++i){
					spacing[i] = Spacing[i];
				}
			}
			/// Set bits per pixel
			void SetBitsPerPixel(BYTE bits)
			{
				bits_per_pixel_ = bits;
			}
			/// Get bits per pixel
			BYTE GetBitsPerPixel()
			{
				return bits_per_pixel_;
			}

		protected:
			bool is_modified_;
			int Dimensions[3];
			double Origin[3];
			double Spacing[3];
			BYTE bits_per_pixel_;
			vtkSmartPointer<vtkImageData> vtk_series_data_;
		};

		class RawPixelData : public IPixelData
		{
		public:
			RawPixelData(UNITDATA3D*);
			~RawPixelData();
			vtkImageData* GetVtkImageData() override;
			UNITDATA3D* GetPixelData();
			void *GetDataPointer(int x, int y, int z) override;

		private:
			UNITDATA3D* series_data_;			//3d pixel data of the series
		};

		class VtkPixelData : public IPixelData
		{
		public:
			VtkPixelData(vtkImageData*);
			~VtkPixelData();
			vtkImageData* GetVtkImageData() override;
			void *GetDataPointer(int x, int y, int z) override;

		private:
			size_t ScalarSize() const;

		private:
			/// Pointer to the raw pixel data.
			void *data_pointer_;
			/// Scalar type of the pixel data represented as a VTK integer constant.
			const int scalar_type_;

		};

	}
}
