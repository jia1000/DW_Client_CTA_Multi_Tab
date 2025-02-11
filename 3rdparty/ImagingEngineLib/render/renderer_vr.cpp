/*=========================================================================

  Program:   ImagingEngine
  Module:    renderer_vr.cpp
  author: 	 zhangjian
  Brief:	 

=========================================================================*/
#include "render/renderer_vr.h"
#include "render/render_param_vr.h"

#include <vtkWindowToImageFilter.h>
#include <vtkBMPWriter.h>
#include <vtkNew.h>
#include <vtkCamera.h>
#include <vtkColorTransferFunction.h>
#include <vtkDataArray.h>
#include <vtkGPUVolumeRayCastMapper.h>
#include <vtkImageData.h>
#include <vtkImageReader.h>
#include <vtkImageShiftScale.h>
#include <vtkNew.h>
#include <vtkOutlineFilter.h>
#include <vtkPiecewiseFunction.h>
#include <vtkPointData.h>
#include <vtkPolyDataMapper.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkSmartPointer.h>
#include <vtkStructuredPointsReader.h>
#include <vtkTimerLog.h>
#include <vtkVolumeProperty.h>
#include <vtkDICOMImageReader.h>
#include <vtkCallbackCommand.h>
#include "tools/vtk_image_data_creator.h"
#include "tools/timer.h"
#include "tools/logger.h"

using namespace DW::Render;

VolumeRenderer::VolumeRenderer()
{
	camera_ = new Camera();
	render_mode_ = RenderMode::RAYCASTING;
	show_buffer_ = new ShowBuffer();
	is_off_screen_rendering_ = true;
	is_first_render_ = true;
	/// initialize vtk objects
	vtk_render_window_ = vtkSmartPointer<vtkRenderWindow>::New();
	vtk_renderer_ = vtkSmartPointer<vtkRenderer>::New();
	vtk_volume_mapper_ = vtkSmartPointer<vtkSmartVolumeMapper>::New();
#if VTK_MAJOR_VERSION > 5
	//// Workaround for vtkSmartVolumeMapper bug (https://gitlab.kitware.com/vtk/vtk/issues/17323)
	vtk_volume_mapper_->InteractiveAdjustSampleDistancesOff(); 
#endif
	vtk_volume_property_ = vtkSmartPointer<vtkVolumeProperty>::New();
	vtk_volume_property_->SetInterpolationTypeToLinear();
	vtk_volume_ = vtkVolume::New();
	vtk_volume_->SetProperty(vtk_volume_property_);
	vtk_volume_->SetMapper(vtk_volume_mapper_);

	/// We Create an TODO default transfer function We have only to have something by default
	/// Opacity
	transfer_function_.SetScalarOpacity(-800.0, 0.0);
	transfer_function_.SetScalarOpacity(-750.0, 0.0);
	transfer_function_.SetScalarOpacity(-350.0, 0.0);
	transfer_function_.SetScalarOpacity(-300.0, 0.0);
	transfer_function_.SetScalarOpacity(-200.0, 0.0);
	transfer_function_.SetScalarOpacity(0.0, 0.0);
	transfer_function_.SetScalarOpacity(10.0, 0.05);
	transfer_function_.SetScalarOpacity(500.0, 0.65);
	transfer_function_.SetScalarOpacity(1200.0, 0.9);
	transfer_function_.SetScalarOpacity(2750.0, 1.0);
	/// Colors
	transfer_function_.SetColor(190.0, 0.4f, 0.2f, 0.0f);
	transfer_function_.SetColor(2000, 1.0f, 1.0f, 1.0f);
	/// Gradient
	transfer_function_.SetGradientOpacity(10, 0.0);
	transfer_function_.SetGradientOpacity(90, 0.5);
	transfer_function_.SetGradientOpacity(100, 1.0);

	/// Set lighting parameters
	vtk_volume_property_->SetAmbient(0.2);
	vtk_volume_property_->SetDiffuse(0.7);
	vtk_volume_property_->SetSpecular(0.3);
	vtk_volume_property_->SetSpecularPower(8.0);
	/// Set shade on
	vtk_volume_property_->ShadeOn();
	/// Set blend mode
	vtk_volume_mapper_->SetBlendModeToComposite();
	/// Set requested render mode
#if VTK_MAJOR_VERSION > 5
	vtk_volume_mapper_->SetRequestedRenderModeToRayCast();
#else
	vtk_volume_mapper_->SetRequestedRenderMode(vtkSmartVolumeMapper::RayCastRenderMode);
#endif
	/// Set transfer function
	vtk_volume_property_->SetColor(transfer_function_.vtkColorTransferFunction());
	vtk_volume_property_->SetScalarOpacity(transfer_function_.vtkScalarOpacityTransferFunction());
	vtk_volume_property_->SetGradientOpacity(transfer_function_.vtkGradientOpacityTransferFunction());

	vtk_renderer_->AddViewProp(vtk_volume_);
	vtk_render_window_->AddRenderer(vtk_renderer_);
	vtk_renderer_->ResetCamera();

	camera_->SetVtkCamera(vtk_renderer_->GetActiveCamera());

	SetOffScreenRendering(true);
}

VolumeRenderer::~VolumeRenderer()
{

}

void VolumeRenderer::SetRenderParam(RenderParam* param)
{
	render_param_ = param;

	//render_param_->GetCamera()->SetVtkCamera(vtk_renderer_->GetActiveCamera());

	///// Set default display position: Coronal
	//vtk_renderer_->GetActiveCamera()->SetViewUp(0, -1, 0);
	//vtk_renderer_->GetActiveCamera()->Elevation(-90);
}

ShowBuffer* VolumeRenderer::GetShowBuffer()
{
	// 在VR renderer类内部转换数据类型
	if (vtk_image_data_ && show_buffer_){

		//原来的方法，用来对比生成的图像
		int width = vtk_image_data_->GetDimensions()[0];
		int height = vtk_image_data_->GetDimensions()[1];

		// 从8位转换到32位
		int slice = -1;

		UNITDATASHOW* data_ptr = reinterpret_cast<UNITDATASHOW*>( vtk_image_data_->GetScalarPointer() );
		int number_of_components = vtk_image_data_->GetNumberOfScalarComponents();

		show_buffer_->SetBufferData(data_ptr, width, height, number_of_components * 8);

		//show_buffer_->DumpBufferToFile("D:\\VR_raycasting.bmp");
	}
	return show_buffer_;
}

void VolumeRenderer::SetData(VolData* data)
{
	if (volume_data_ == data)
		return;

	Timer::begin("VolumeRenderer::SetData");

	volume_data_ = data;
	// Workaround for vtkSmartVolumeMapper bug (https://gitlab.kitware.com/vtk/vtk/issues/17328)
	volume_data_->GetPixelData()->GetVtkImageData()->Modified();
	vtkImageData *image_data = volume_data_->GetPixelData()->GetVtkImageData();
	// 在此处将mask转换为vtkImageData格式
	UNITMARK3D *mark = volume_data_->GetMark();

	// 输入到Mapper里的vtkImageData
	vtkImageData *mapper_input_data = vtkImageData::New();
	if (mark){

		if (vtk_mask_filter_){
			vtk_mask_filter_->Delete();
		}
		vtk_mask_filter_ = vtkSmartPointer<vtkImageMask>::New();

		int dims[3];
		volume_data_->GetPixelData()->GetDimensions(dims);
		double spacings[3];
		volume_data_->GetPixelData()->GetSpacing(spacings);

		//////////////////////////////////////////////////////////////////////////
		/// Using Creator to generate vtkImageData
		//double origins[3] = {0.0, 0.0, 0.0};
		//VtkImageDataCreator imageDataCreator;
		//imageDataCreator.SetOrigin(origins);
		//imageDataCreator.SetSpacing(spacings);
		//imageDataCreator.SetDimensions(dims);
		//imageDataCreator.SetNumberOfComponents(1);
		//input_mark_data_ = imageDataCreator.Create(mark);
		//
		//vtk_mask_filter_->SetInputData(0, image_data);
		//vtk_mask_filter_->SetInputData(1, input_mark_data_);
		//vtk_mask_filter_->SetMaskedOutputValue(-1024, -1024, -1024);
		//vtk_mask_filter_->Update();
		//
		////// Input image data is useless now
		////image_data->Delete();
		////image_data = NULL;
		//
		//mapper_input_data->DeepCopy(vtk_mask_filter_->GetOutput());
		//
		//vtk_mask_filter_->Delete();
		/// END
		//////////////////////////////////////////////////////////////////////////

		//////////////////////////////////////////////////////////////////////////
		// Set volume mask data
		vtkSmartPointer<vtkImageImport> image_import = vtkSmartPointer<vtkImageImport>::New();
		image_import->SetDataSpacing(spacings);
		image_import->SetDataOrigin(0, 0, 0);
		image_import->SetWholeExtent(0, dims[0] - 1, 0, dims[1] - 1, 0, dims[2] - 1);
		image_import->SetDataExtentToWholeExtent();
		image_import->SetDataScalarTypeToUnsignedChar();
		image_import->SetNumberOfScalarComponents(1);
		image_import->SetImportVoidPointer(mark, 1);
		image_import->Update();
		input_mark_data_ = image_import->GetOutput();

		int extent[6];
		input_mark_data_->GetExtent(extent);
		
		vtk_mask_filter_->SetInputData(0, image_data);
		vtk_mask_filter_->SetInputData(1, input_mark_data_);
		vtk_mask_filter_->SetMaskedOutputValue(-1024, -1024, -1024);
		vtk_mask_filter_->Update();

		mapper_input_data->ShallowCopy(vtk_mask_filter_->GetOutput());
		//////////////////////////////////////////////////////////////////////////
	}
	else{
		mapper_input_data->ShallowCopy(image_data);
	}
	

#if VTK_MAJOR_VERSION > 5
	vtk_volume_mapper_->SetInputData(mapper_input_data);
	/// force the mapper to compute a sample distance based on data spacing
	vtk_volume_mapper_->SetSampleDistance(-1.0);
#else
	vtk_volume_mapper_->SetInput(volume_data_->GetPixelData()->GetVtkImageData());
#endif
	is_first_render_ = true;

	Timer::end("VolumeRenderer::SetData");
	CGLogger::Info(Timer::summery());
}

void VolumeRenderer::SetVolumeTransformation()
{
	//Image *imageReference = getMainInput()->getImage(0);
	//ImagePlane currentPlane;
	//currentPlane.fillFromImage(imageReference);
	//std::array<double, 3> currentPlaneRowVector = Vector3(currentPlane.getImageOrientation().getRowVector());
	//std::array<double, 3> currentPlaneColumnVector = Vector3(currentPlane.getImageOrientation().getColumnVector());
	//double stackDirection[3];
	//getMainInput()->getStackDirection(stackDirection);

	//DEBUG_LOG(QString("currentPlaneRowVector: %1 %2 %3").arg(currentPlaneRowVector[0]).arg(currentPlaneRowVector[1]).arg(currentPlaneRowVector[2]));
	//DEBUG_LOG(QString("currentPlaneColumnVector: %1 %2 %3").arg(currentPlaneColumnVector[0]).arg(currentPlaneColumnVector[1]).arg(currentPlaneColumnVector[2]));
	//DEBUG_LOG(QString("stackDirection: %1 %2 %3").arg(stackDirection[0]).arg(stackDirection[1]).arg(stackDirection[2]));

	//vtkMatrix4x4 *projectionMatrix = vtkMatrix4x4::New();
	//projectionMatrix->Identity();

	//if ((   currentPlaneRowVector[0] == 0.0 &&    currentPlaneRowVector[1] == 0.0 &&    currentPlaneRowVector[2] == 0.0) ||
	//	(currentPlaneColumnVector[0] == 0.0 && currentPlaneColumnVector[1] == 0.0 && currentPlaneColumnVector[2] == 0.0) ||
	//	(          stackDirection[0] == 0.0 &&           stackDirection[1] == 0.0 &&           stackDirection[2] == 0.0))
	//{
	//	DEBUG_LOG("One of the vectors is null: setting an identity projection matrix.");
	//}
	//else
	//{
	//	for (int row = 0; row < 3; row++)
	//	{
	//		projectionMatrix->SetElement(row, 0, currentPlaneRowVector[row]);
	//		projectionMatrix->SetElement(row, 1, currentPlaneColumnVector[row]);
	//		projectionMatrix->SetElement(row, 2, stackDirection[row]);
	//	}
	//}

	//m_vtkVolume->SetUserMatrix(projectionMatrix);
	//m_isosurfaceActor->SetUserMatrix(projectionMatrix);

	//projectionMatrix->Delete();
}

void VolumeRenderer::Render()
{
	if (volume_data_ == NULL) return;

	Timer::begin("VolumeRenderer::Render");

	DoRender(volume_data_->GetPixelData()->GetVtkImageData());

	Timer::end("VolumeRenderer::Render");
	CGLogger::Info(Timer::summery());
}

//此版本从dr.wise客户端修改而来
void VolumeRenderer::DoRender(vtkSmartPointer<vtkImageData> imagedata)
{
	if (render_param_ == NULL) return;
	VRRenderParam* param_imp = (VRRenderParam *)render_param_;
	///// 深拷贝参数？
	//camera_ = param_imp->GetCamera();
	// get render parameters
	int width = param_imp->GetWidth();
	int height = param_imp->GetHeight();

	vtk_render_window_->SetSize(width, height);
	// set desired update rate
	vtk_render_window_->SetDesiredUpdateRate(param_imp->GetDesiredUpdateRate());

	switch (param_imp->GetRenderMode())
	{
		case RenderMode::SMART_RAYCASTING:
			vtk_volume_mapper_->SetRequestedRenderModeToDefault(); 
			break;
		case RenderMode::RAYCASTING: 
			vtk_volume_mapper_->SetRequestedRenderModeToRayCast(); 
			break;
		case RenderMode::RAYCASTING_GPU: 
#if VTK_MAJOR_VERSION > 5
			vtk_volume_mapper_->SetRequestedRenderModeToGPU(); 
#else
			// vtk 5以下需要DXD库支持GPU
			vtk_volume_mapper_->SetRequestedRenderMode(vtkSmartVolumeMapper::RayCastRenderMode);
#endif
			break;
	}
	/// The first render after data changed
	if (is_first_render_){
		is_first_render_ = false;
		vtk_renderer_->ResetCamera();
	}

	//TODO needs 5 seconds to render mask data for the first time...
	vtk_render_window_->Render();

	/// get screenshot from render window  
	vtkSmartPointer<vtkWindowToImageFilter> windowToImageFilter = vtkSmartPointer<vtkWindowToImageFilter>::New();
	windowToImageFilter->SetInput(vtk_render_window_);
	windowToImageFilter->SetMagnification(1); //set the resolution of the output image (3 times the current resolution of vtk render window)
	windowToImageFilter->SetInputBufferTypeToRGBA(); //also record the alpha (transparency) channel
	windowToImageFilter->ReadFrontBufferOff(); // read from the back buffer
	windowToImageFilter->Update();
	vtk_image_data_ = windowToImageFilter->GetOutput();
}

void VolumeRenderer::SetOffScreenRendering(bool flag)
{
	is_off_screen_rendering_ = flag;
	vtk_render_window_->SetOffScreenRendering(flag);
}

bool VolumeRenderer::PickByImagePos(const Vector2i& in_point, Vector3d& out_point)
{
	return false;
}

bool VolumeRenderer::PickByImagePos(const int& x, const int& y, Vector3d& out_point)
{
	return false;
}

bool VolumeRenderer::PickBy3DPoint(const Vector3d& in_point, int& x, int& y)
{
	return false;
}

float VolumeRenderer::GetVoxelVolume()
{
	return 0.0;
}

float VolumeRenderer::GetVoxelVolume(BoundingBox*)
{
	return 0.0;
}

float VolumeRenderer::CalculateLength(Vector3d&, Vector3d&)
{
	return 0.0;
}

void VolumeRenderer::GetOrientationTags(Vector3d&, Vector3d&, Vector3d&, Vector3d&)
{

}

//------------------Set rendering parameters---------------------
void VolumeRenderer::SetShading(bool on)
{
	if (on)
	{
		vtk_volume_property_->ShadeOn();
	}
	else
	{
		vtk_volume_property_->ShadeOff();
	}
}

void VolumeRenderer::SetAmbient(double ambient)
{
	vtk_volume_property_->SetAmbient(ambient);
}

void VolumeRenderer::SetDiffuse(double diffuse)
{
	vtk_volume_property_->SetDiffuse(diffuse);
}

void VolumeRenderer::SetSpecular(double specular)
{
	vtk_volume_property_->SetSpecular(specular);
}

void VolumeRenderer::SetSpecularPower(double power)
{
	vtk_volume_property_->SetSpecularPower(power);
}

void VolumeRenderer::SetClippingPlanes(vtkPlanes *clipping)
{
	vtk_clipping_planes_ = clipping;
	vtk_clipping_planes_->Register(nullptr);
	vtk_volume_mapper_->SetClippingPlanes(vtk_clipping_planes_);
}

vtkPlanes* VolumeRenderer::GetClippingPlanes() const
{
	return vtk_clipping_planes_;
}

void VolumeRenderer::SetTransferFunction(const TransferFunction &transferFunction)
{
	transfer_function_ = transferFunction;

	vtk_volume_property_->SetScalarOpacity(transfer_function_.vtkScalarOpacityTransferFunction());
	vtk_volume_property_->SetColor(transfer_function_.vtkColorTransferFunction());
}

//------------------------- end ---------------------------------

void VolumeRenderer::ComputeWorldToDisplay(Point3f& world_pos, Point3f& display_pos)
{
	if (vtk_renderer_)
	{
		double display_arr[3] = {0.0};
		vtk_renderer_->SetWorldPoint(world_pos.x, world_pos.y, world_pos.z, 1.0);
		vtk_renderer_->WorldToDisplay();
		vtk_renderer_->GetDisplayPoint(display_arr);

		display_pos.x = display_arr[0];
		display_pos.y = display_arr[1];
		display_pos.z = display_arr[2];
	}
}

void VolumeRenderer::ComputeWorldToDisplay(double x, double y, double z, double display_point[3])
{
	if (vtk_renderer_)
	{
		vtk_renderer_->SetWorldPoint(x, y, z, 1.0);
		vtk_renderer_->WorldToDisplay();
		vtk_renderer_->GetDisplayPoint(display_point);
	}
}