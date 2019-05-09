/*=========================================================================

  Program:   ImagingEngine
  Module:    render_param_mpr.h
  author: 	 zhangjian
  Brief:	 MPR图像成像参数

=========================================================================*/
#pragma once

#include "data_definition.h"
#include "render_param.h"
#include "vtk_include.h"
#include "data/curve_line.h"

using namespace DW::IMAGE;

namespace DW {
	namespace Render{

		///
		// MPR图像成像参数
		///
		class MPRRenderParam : public RenderParam
		{
		public:
			MPRRenderParam();
			~MPRRenderParam();
			// 返回扫描轨迹的方向
			void GetPlaneVector(Vector3f& row, Vector3f& col)
			{
				row = row_vector;
				col = column_vector;
			}
			// 设置扫描轨迹的方向
			void SetPlaneVector(Vector3f row, Vector3f col)
			{
				row_vector = row;
				column_vector = col;
			}
			// 返回层厚，0表示默认层厚
			float GetThickness()
			{
				return thickness_;
			}
			// 设置图像中心点
			void SetImageCenter(float x, float y, float z)
			{
				center_.x = x;
				center_.y = y;
				center_.z = z;
			}
			// 获取图像中心点
			void GetImageCenter(Point3f& center)
			{
				center.x = center_.x;
				center.y = center_.y;
				center.z = center_.z;
			}
			// 设置层厚，0表示默认层厚
			void SetThickness(float thickness)
			{
				thickness_ = thickness;
			}
			// 设置旋转轴
			void SetRotationAxis(float x, float y, float z)
			{
				rotation_axis_[0] = x;
				rotation_axis_[1] = y;
				rotation_axis_[2] = z;
			}
			// 获取旋转轴
			void GetRotationAxis(Vector3f& axis)
			{
				axis = rotation_axis_;
			}
			// 设置旋转中心
			void SetRotationCenter(float x, float y, float z)
			{
				rotation_center_.x = x;
				rotation_center_.y = y;
				rotation_center_.z = z;
			}
			// 获取旋转中心
			void GetRotationCenter(float& x, float& y, float& z)
			{
				x = rotation_center_.x;
				y = rotation_center_.y;
				z = rotation_center_.z;
			}

		private:
			// mpr平面行向量
			Vector3f row_vector;
			// mpr平面列向量
			Vector3f column_vector;
			// 图像中心点，由上层指定，默认几何平面中点
			Point3f center_;
			// 层厚
			float thickness_;
			// 旋转轴
			Vector3f rotation_axis_;
			// 旋转中心
			Point3f rotation_center_;
		};

	}
}