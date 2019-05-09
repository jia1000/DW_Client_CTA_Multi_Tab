/*=========================================================================

  Program:   ImagingEngine
  Module:    render_param_cpr.h
  author: 	 zhangjian
  Brief:	 CPR图像成像参数

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
		// CPU图像显示方式参照以下论文
		// Kanitsar et al. "CPR - Curved Planar Reformation", Proc. IEEE  Visualization, 2002, 37-44
		///
		enum CPRRenderMode
		{
			STRETCHED_CPR_AUTO,
			STRETCHED_CPR_HOR,
			STRETCHED_CPR_VER,
			STRAIGHTENED_CPR_AUTO,
			STRAIGHTENED_CPR_HOR,
			STRAIGHTENED_CPR_VER,
		};

		///
		// CPR图像成像参数
		// 部分参照侯贺硕士论文《心脏冠状动脉曲面重建技术研究》
		// Kanitsar et al. "CPR - Curved Planar Reformation", Proc. IEEE  Visualization, 2002, 37-44
		///
		class CPRRenderParam : public RenderParam
		{
		public:
			CPRRenderParam();
			~CPRRenderParam();
			// 返回curve
			VolCurve* GetCurve()
			{
				return curve_;
			}
			// 设置curve
			void SetCurve(VolCurve* curve)
			{
				curve_ = curve;
			}
			// 返回rendre mode
			CPRRenderMode GetRenderMode()
			{
				return render_mode_;
			}
			// 设置rendre mode
			void SetRenderMode(CPRRenderMode mode)
			{
				render_mode_ = mode;
			}
			// 返回扫描轨迹的方向
			void GetDirection(float& x, float& y, float& z)
			{
				x = vector_of_interest_[0];
				y = vector_of_interest_[1];
				z = vector_of_interest_[2];
			}
			// 设置扫描轨迹的方向
			void SetDirection(const float& x, const float& y, const float& z)
			{
				vector_of_interest_[0] = x;
				vector_of_interest_[1] = y;
				vector_of_interest_[2] = z;
			}
			// 返回层厚，0表示默认层厚
			float GetThickness()
			{
				return thickness_;
			}
			// 设置层厚，0表示默认层厚
			void SetThickness(float thickness)
			{
				thickness_ = thickness;
			}
			// 设置旋转轴
			void SetAxis(float x, float y, float z)
			{
				rotation_axis_[0] = x;
				rotation_axis_[1] = y;
				rotation_axis_[2] = z;
			}
			// 获取旋转轴
			void GetAxis(float& x, float& y, float& z)
			{
				x = rotation_axis_[0];
				y = rotation_axis_[1];
				z = rotation_axis_[2];
			}
			// 设置旋转中心
			void SetCenter(float x, float y, float z)
			{
				rotation_center_.x = x;
				rotation_center_.y = y;
				rotation_center_.z = z;
			}
			// 获取旋转中心
			void GetCenter(float& x, float& y, float& z)
			{
				x = rotation_center_.x;
				y = rotation_center_.y;
				z = rotation_center_.z;
			}
			// 设置旋转角度
			void SetAngle(float angle)
			{
				rotation_angle_ = angle;
			}
			// 设置旋转角度
			float GetAngle()
			{
				return rotation_angle_;
			}
		private:
			// 扫描轨迹(切割线)的方向，用于控制stretched cpr旋转
			Vector3f vector_of_interest_;
			// 旋转的角度，用于控制straightened cpr旋转
			float rotation_angle_;
			// 成像方式：全景/拉直/行填充/列填充
			CPRRenderMode render_mode_;
			// 三维曲线
			VolCurve* curve_;
			// 层厚
			float thickness_;
			// 旋转轴
			Vector3f rotation_axis_;
			// 旋转中心
			Point3f rotation_center_;
		};

	}
}