/*=========================================================================

  Program:   ImagingEngine
  Module:    vol_data.h
  author: 	 zhangjian
  Brief:	 三维数据类

=========================================================================*/
#pragma once

#include "vtk_include.h"
#include "data_definition.h"
#include "data_convert.h"
#include "data/box3.h"
#include "data/aabb.h"
#include "data/pixel_data.h"

using namespace DW::IMAGE;
using namespace std;

namespace DW {
	namespace IMAGE{
		class BoundingBox;

		class VolData
		{
		public:
			VolData();
			~VolData();
			IPixelData* GetPixelData();
			void SetPixelData(IPixelData*);

			BYTE GetBitsPerPixel() { return bits_per_pixel_; }
			void SetBitsPerPixel(BYTE bits);
			BYTE GetBitsStored() { return bits_stored_; }
			void SetBitsStored(BYTE bits);
			void SetData(UNITDATA3D* data);
			void SetMark(UNITMARK3D* mark);
			UNITMARK3D *GetMark() { return series_mark_; }

			int GetSliceWidth() { return slice_width_; }
			void SetSliceWidth(int width);
			int GetSliceHeight() { return slice_height_; }
			void SetSliceHeight(int height);
			int GetSliceCount() { return slice_count_; }
			void SetSliceCount(int count);
			int GetSliceFOVWidth() { return slice_fov_width_; }
			void SetSliceFOVWidth(int fov_w);
			int GetSliceFOVHeight() { return slice_fov_height_; }
			void SetSliceFOVHeight(int fov_h);
			void SetStudyInstanceUID(string uid);
			void SetSeriesInstanceUID(string uid);
			/// 设置包围盒的顶点
			void SetBoundingBox(float xmin,float ymin,float zmin,float xmax,float ymax,float zmax);
			/// 获取当前包围盒对象
			BoundingBox* GetBoundingBox() { return bounding_box_; }
			/// 从数据指针中获取指定坐标位置的值
			void *GetDataPointer(int x, int y, int z);
			/// 设置获取数据修改标识
			void SetModified(bool flag) {modified_=flag;}
			bool GetModified() {return modified_;}
			void Destroy();
			/// 窗宽窗位
			void GetDefaultWindowWidthLevel(int &width, int &level);
			void SetDefaultWindowWidthLevel(int width, int level);

		protected:
			BYTE bits_per_pixel_;				//bits allocated for display 
			BYTE bits_stored_;					//bits stored, the max ct value is 4095 with bits stored value 12，and is 10000 with value 16
			IPixelData* pixel_data_;			//3d pixel data of the series
			//UNITDATA3D* series_data_;			//3d pixel data of the series
			UNITMARK3D* series_mark_;			//3d mark data of the series
			//vtkImageData* vtk_series_data;	//3d data with vtkImageData format 
			vtkSmartPointer<vtkImageData> vtk_series_mark_;
			int slice_width_;					//value stored in tag Columns(0028,0011)
			int slice_height_;					//value stored in tag Rows(0028,0010)
			int slice_count_;					//slice count in the series
			float slice_fov_width_;				//Reconstruction Field of View (width), pixel space = fov(width) / columns
			float slice_fov_height_;			//Reconstruction Field of View (width), pixel space = fov(height) / rows
			string study_instance_uid_;			//study instance uid, stored in tag (0020,000D)
			string series_instance_uid_;		//series instance uid, stored in tag (0020,000E)
			BoundingBox *bounding_box_;			//
			int volume_id_;						//
			bool modified_;						//
			int window_width_;					// default window width
			int window_level_;					// default window level

		private:


		};

		class BoundingBox : public AABB
		{
		public:
			BoundingBox() {}
			~BoundingBox() {}

			const float& operator[](const uint32_t index) const
			{
				if (index == 0){
					return this->Xmin;
				}
				else if (index == 1){
					return this->Ymin;
				}
				else if (index == 2){
					return this->Zmin;
				}
				else if (index == 3){
					return this->Xmax;
				}
				else if (index == 4){
					return this->Ymax;
				}
				else if (index == 5){
					return this->Zmax;
				}
			}

			void GetCornerPoints(Point3f& top_left, Point3f& bottom_right)
			{
				top_left.x = Xmin;
				top_left.y = Ymin;
				top_left.z = Zmin;
				bottom_right.x = Xmax;
				bottom_right.y = Ymax;
				bottom_right.z = Zmax;
			}

		private:

		};

	}
}