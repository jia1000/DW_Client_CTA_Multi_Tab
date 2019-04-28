/*=========================================================================

  Program:   ImagingEngine
  Module:    renderer_raycasting_cpu.h
  author: 	 zhangjian
  Brief:	 VR图像的raycasting重建方法渲染类，是基于cpu计算的，性能低

=========================================================================*/
#pragma once

#include "vtk_include.h"
#include "data_definition.h"
#include "renderer_3d.h"
#include "pick_3d.h"
#include "measure_3d.h"
#include "orientation_3d.h"

using namespace DW::IMAGE;

namespace DW {
	namespace Render{

		class RayCastingRenderer : public IThreedRenderer
			, public IThreedPickHandler
			, public IThreedMeasureHandler
			, public IThreedOrientationHandler
		{
		public:
			RayCastingRenderer();
			virtual ~RayCastingRenderer();
			void SetRenderParam(RenderParam* param) override;
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
			float GetVoxelVolume() override;
			float GetVoxelVolume(BoundingBox*) override;
			float CalculateLength(Vector3d&, Vector3d&) override;
			// IThreedOrientationHandler implementation
			void GetOrientationTags(Vector3d&, Vector3d&, Vector3d&, Vector3d&) override;

			// methods for testing
			void SetOffScreenRendering(bool);
		protected:
			virtual void DoRender(vtkSmartPointer<vtkImageData> imagedata);
			void DoRender2(vtkSmartPointer<vtkImageData> imagedata);
		protected:
			bool is_off_screen_rendering_;
			vtkSmartPointer<vtkImageData> vtk_image_data_;
			vtkSmartPointer<vtkRenderWindow> render_window_;
			vtkSmartPointer<vtkRenderer> vtk_renderer_;
		};
	}
}