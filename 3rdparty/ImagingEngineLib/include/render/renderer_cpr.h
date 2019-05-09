/*=========================================================================

  Program:   ImagingEngine
  Module:    renderer_cpr.h
  author: 	 zhangjian
  Brief:	 CPR图像的渲染类

=========================================================================*/
#pragma once

#include "vtk_include.h"
#include "data_definition.h"
#include "renderer_3d.h"
#include "pick_3d.h"
#include "measure_3d.h"
#include "orientation_3d.h"
#include "data/curve_line.h"

using namespace DW::IMAGE;

namespace DW {
	namespace Render{

		class CPRRenderer : public IThreedRenderer
			, public IThreedPickHandler
			, public IThreedMeasureHandler
			, public IThreedOrientationHandler
		{
		public:
			CPRRenderer();
			virtual ~CPRRenderer();
			void Render() override;
			ShowBuffer* GetShowBuffer() override;
			void SetData(VolData* data) override;
			/// get handler
			IThreedPickHandler* GetThreedPickHandler() { return this; }
			IThreedMeasureHandler* GetThreedMeasureHandler() { return this; }
			IThreedOrientationHandler* GetThreedOrientationHandler() { return this; }

			/// IThreedPickHandler implementation
			bool PickByImagePos(const Vector2i& in_pnt, Vector3d& out_point) override;
			bool PickByImagePos(const int& x, const int& y, Vector3d& out_point) override;
			bool PickBy3DPoint(const Vector3d&, int&, int&) override;
			/// IThreedMeasureHandler implementation
			float CalculateLength(Vector3d&, Vector3d&) override;
			/// IThreedOrientationHandler implementation
			void GetOrientationTags(Vector3d&, Vector3d&, Vector3d&, Vector3d&) override;

			/// 设置旋转轴
			void SetRotationAxis(float x, float y, float z);
			/// 获取旋转轴
			void GetRotationAxis(float& x, float& y, float& z);
			/// 获取旋转中心点
			void GetRotationCenter(float& x, float& y, float& z);
			/// Convert world coordinate to display coordinate
			void ComputeWorldToDisplay(Point3f& world_pos, Point3f& display_pos) override;
			void ComputeWorldToDisplay(double x, double y, double z, double display_point[3]) override;
			/// Convert vtkImageData to showbuffer object
			void BufferTransform() override;

			/// methods for testing
			void SetOffScreenRendering(bool);
		protected:
			virtual void DoRender();
			// 计算生成CPR图像的宽高
			virtual void CalculateImageSize(VolCurve* curve,
				float direction[3],
				float thickness,
				float& left_distance,
				float& right_distance);
			// 计算包围盒交点的范围
			virtual void GetBoundingBoxBorder(float cross_pnt[3],
				float direction[3],
				float origin[3],
				float curr_proj_point[3],
				float& left_distance, 
				float& right_distance);
			// 沿着curve扫描，计算用来切割体数据的空间曲面
			virtual vtkSmartPointer<vtkPolyData> SweepLine(VolCurve* curve,
				float direction[3],
				float distance,
				unsigned int cols);
			vtkSmartPointer<vtkPolyData> SweepLine2 (vtkPolyData* line,
				float direction[3],
				float distance,
				unsigned int cols);
			// 计算直线与包围盒的交点
			int Compute3DCrossPoint(const float SrcPnt[],
				float Dir[],
				BoundingBox* box,
				float* cross_point_one, 
				float* cross_point_two);
			// 三线性插值计算line of interest上点的值
			void TrilinearInterpolation(short& ctv, 
				float ix, 
				float iy, 
				float iz, 
				VolData* data);
			// 更新旋转轴和旋转中心点
			void UpdateRotation();

		protected:
			bool is_off_screen_rendering_;
			// 暂用于从pixeldata应用窗宽窗位后的输出结果
			vtkSmartPointer<vtkImageData> output_vtk_image_data_;
			// 已经不使用了
			vtkSmartPointer<vtkRenderWindow> render_window_;
			vtkSmartPointer<vtkRenderer> vtk_renderer_;
			// 第一个采样点与图像左右边界的距离
			float origin_padding_left_;
			float origin_padding_right_;
			// 旋转轴
			Vector3f rotation_axis_;
			// 图像中心点(也是旋转中心点)，非图像几何中心点
			// 计算方法：曲线在图像上矩形范围的中心点
			Point3f rotation_center_;
			static int tmp_counter_;
			int cpr_file_id;
		};
	}
}