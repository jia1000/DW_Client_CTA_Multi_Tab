/*=========================================================================

  Program:   ImagingEngine
  Module:    renderer_vr.h
  author: 	 zhangjian
  Brief:	 

=========================================================================*/
#pragma once

#include "vtk_include.h"
#include "data_definition.h"
#include "renderer_3d.h"
#include "pick_3d.h"
#include "measure_3d.h"
#include "orientation_3d.h"
#include "data/pixel_data.h"
#include "render/transfer_function.h"

using namespace DW::IMAGE;

namespace DW {
	namespace Render{

		class VolumeRenderer : public IThreedRenderer
			, public IThreedPickHandler
			, public IThreedMeasureHandler
			, public IThreedOrientationHandler
		{
		public:
			VolumeRenderer();
			virtual ~VolumeRenderer();

			void SetRenderParam(RenderParam* param) override;
			void Render() override;
			ShowBuffer* GetShowBuffer() override;
			void SetData(VolData* data) override;
			/// Convert world coordinate to display coordinate
			void ComputeWorldToDisplay(Point3f& world_pos, Point3f& display_pos) override;
			void ComputeWorldToDisplay(double x, double y, double z, double display_point[3]) override;

			/// Shading On/Off
			void SetShading(bool);
			/// Lighting Parameters.
			void SetAmbient(double);
			void SetDiffuse(double);
			void SetSpecular(double);
			void SetSpecularPower(double);

			/// Determines The bounding box that defines the volume cutting plans
			void SetClippingPlanes(vtkPlanes *clipping);
			/// Get cutting plans that have been defined on the volume
			vtkPlanes* GetClippingPlanes() const;
			/// Get/Set the current transfer function
			void SetTransferFunction(const TransferFunction &transferFunction);
			/// Computes and sets the transformation matrix for the volume to convert from the DICOM space to the world space.
			void SetVolumeTransformation();

			/// get handler
			IThreedPickHandler* GetThreedPickHandler() { return this; }
			IThreedMeasureHandler* GetThreedMeasureHandler() { return this; }
			IThreedOrientationHandler* GetThreedOrientationHandler() { return this; }

			/// IThreedPickHandler implementation
			bool PickByImagePos(const Vector2i& in_pnt, Vector3d& out_point) override;
			bool PickByImagePos(const int& x, const int& y, Vector3d& out_point) override;
			bool PickBy3DPoint(const Vector3d&, int&, int&) override;
			/// IThreedMeasureHandler implementation
			float GetVoxelVolume() override;
			float GetVoxelVolume(BoundingBox*) override;
			float CalculateLength(Vector3d&, Vector3d&) override;
			/// IThreedOrientationHandler implementation
			void GetOrientationTags(Vector3d&, Vector3d&, Vector3d&, Vector3d&) override;
			
			/// methods for testing
			void SetOffScreenRendering(bool);
		protected:
			virtual void DoRender(vtkSmartPointer<vtkImageData> imagedata);

		protected:
			bool is_off_screen_rendering_;
			/// 输出图像的vtkImageData格式
			vtkSmartPointer<vtkImageData> vtk_image_data_;
			/// 输入MASK的vtkImageData格式
			vtkSmartPointer<vtkImageData> input_mark_data_;
			/// vtkImageMask
			vtkSmartPointer<vtkImageMask> vtk_mask_filter_;
			vtkSmartPointer<vtkRenderWindow> vtk_render_window_;
			vtkSmartPointer<vtkRenderer> vtk_renderer_;
			/// The main mapper for volume rendering.
			vtkSmartPointer<vtkSmartVolumeMapper> vtk_volume_mapper_;
			/// Properties of volume rendering.
			vtkSmartPointer<vtkVolumeProperty> vtk_volume_property_;
			/// The volume actor.
			vtkSmartPointer<vtkVolume> vtk_volume_;
			/// The clipping vtkPlanes
			vtkSmartPointer<vtkPlanes> vtk_clipping_planes_;
			/// Current transfer function.
			TransferFunction transfer_function_;
		};
	}
}