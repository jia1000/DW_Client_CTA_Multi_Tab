/*=========================================================================

  Program:   ImagingEngine
  Module:    renderer_cpr.h
  author: 	 zhangjian
  Brief:	 CPRͼ�����Ⱦ��

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
			// get handler
			IThreedPickHandler* GetThreedPickHandler() { return this; }
			IThreedMeasureHandler* GetThreedMeasureHandler() { return this; }
			IThreedOrientationHandler* GetThreedOrientationHandler() { return this; }

			// IThreedPickHandler implementation
			bool PickByImagePos(const Vector2i& in_pnt, Vector3d& out_point) override;
			bool PickByImagePos(const int& x, const int& y, Vector3d& out_point) override;
			bool PickBy3DPoint(const Vector3d&, int&, int&) override;
			// IThreedMeasureHandler implementation
			float CalculateLength(Vector3d&, Vector3d&) override;
			// IThreedOrientationHandler implementation
			void GetOrientationTags(Vector3d&, Vector3d&, Vector3d&, Vector3d&) override;
			// ������ת��
			void SetRotationAxis(float x, float y, float z);
			// ��ȡ��ת��
			void GetRotationAxis(float& x, float& y, float& z);
			// ��ȡ��ת���ĵ�
			void GetRotationCenter(float& x, float& y, float& z);

			// methods for testing
			void SetOffScreenRendering(bool);
		protected:
			virtual void DoRender();
			// ��������CPRͼ��Ŀ���
			virtual void CalculateImageSize(VolCurve* curve,
				float direction[3],
				float thickness,
				float& left_distance,
				float& right_distance);
			// �����Χ�н���ķ�Χ
			virtual void GetBoundingBoxBorder(float cross_pnt[3],
				float direction[3],
				float origin[3],
				float curr_proj_point[3],
				float& left_distance, 
				float& right_distance);
			// ����curveɨ�裬���������и������ݵĿռ�����
			virtual vtkSmartPointer<vtkPolyData> SweepLine(VolCurve* curve,
				float direction[3],
				float distance,
				unsigned int cols);
			vtkSmartPointer<vtkPolyData> SweepLine2 (vtkPolyData* line,
				float direction[3],
				float distance,
				unsigned int cols);
			// ����ֱ�����Χ�еĽ���
			int Compute3DCrossPoint(const float SrcPnt[],
				float Dir[],
				BoundingBox* box,
				float* cross_point_one, 
				float* cross_point_two);
			// �����Բ�ֵ����line of interest�ϵ��ֵ
			void TrilinearInterpolation(short& ctv, 
				float ix, 
				float iy, 
				float iz, 
				VolData* data);
			// ������ת�����ת���ĵ�
			void UpdateRotation();

		protected:
			bool is_off_screen_rendering_;
			// �����ڴ�pixeldataӦ�ô�����λ���������
			vtkSmartPointer<vtkImageData> output_vtk_image_data_;
			// �Ѿ���ʹ����
			vtkSmartPointer<vtkRenderWindow> render_window_;
			vtkSmartPointer<vtkRenderer> vtk_renderer_;
			// ��һ����������ͼ�����ұ߽�ľ���
			float origin_padding_left_;
			float origin_padding_right_;
			// ��ת��
			Vector3f rotation_axis_;
			// ͼ�����ĵ�(Ҳ����ת���ĵ�)����ͼ�񼸺����ĵ�
			// ���㷽����������ͼ���Ͼ��η�Χ�����ĵ�
			Point3f rotation_center_;
		};
	}
}