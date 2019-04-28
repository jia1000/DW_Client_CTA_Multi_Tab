
#include "renderer_raycasting_cpu.h"
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
//#include <vtkTestUtilities.h>
//#include <vtkRegressionTestImage.h>
#include <vtkTimerLog.h>
#include <vtkVolumeProperty.h>
#include <vtkDICOMImageReader.h>
#include <vtkCallbackCommand.h>

#include "render/render_param_vr.h"

using namespace DW::Render;

static int bmp_counter = 1;

//#define USE_INTERACTOR

RayCastingRenderer::RayCastingRenderer()
{
	is_off_screen_rendering_ = true;
	render_window_ = vtkSmartPointer<vtkRenderWindow>::New();
	is_first_render_ = true;
}

RayCastingRenderer::~RayCastingRenderer()
{
	
}

void RayCastingRenderer::SetRenderParam(RenderParam* param)
{
	render_param_ = param;
}

ShowBuffer* RayCastingRenderer::GetShowBuffer()
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
	}
	return show_buffer_;
}

void RayCastingRenderer::SetData(VolData* data)
{
	volume_data_ = data;
}

void RayCastingRenderer::Render()
{
	if (volume_data_ == NULL) return;

	DoRender(volume_data_->GetVtkData());
}

//此版本从dr.wise客户端修改而来
void RayCastingRenderer::DoRender(vtkSmartPointer<vtkImageData> imagedata)
{
	if (render_param_ == NULL) return;
	VRRenderParam* param_imp = (VRRenderParam *)render_param_;
	// 深拷贝参数？
	camera_ = param_imp->GetCamera();
	
	vtkSmartPointer<vtkRenderWindow> renWin = render_window_;	
	// 从render_param_取出渲染参数
	if (is_first_render_){
		is_first_render_ = false;
		vtkSmartPointer<vtkSmartVolumeMapper> data_mapper = vtkSmartPointer<vtkSmartVolumeMapper>::New();
		data_mapper->SetBlendModeToComposite();
#if VTK_MAJOR_VERSION > 5
		data_mapper->SetInputData(imagedata);
		data_mapper->SetRequestedRenderModeToRayCast();
		//data_mapper->SetAutoAdjustSampleDistances(1);
		//data_mapper->SetSampleDistance(1);
		//data_mapper->SetImageSampleDistance(1);
#else
		data_mapper->SetInputConnection(imagedata->GetProducerPort());
		data_mapper->SetRequestedRenderMode(vtkSmartVolumeMapper::RayCastAndTextureRenderMode);
		//data_mapper->SetRequestedRenderMode(vtkSmartVolumeMapper::RayCastRenderMode);
#endif
		// get render parameters
		int width = param_imp->GetWidth();
		int height = param_imp->GetHeight();
		float ambient = param_imp->GetAmbient();
		float diffuse = param_imp->GetDiffuse();
		float specular = param_imp->GetSpecular();
		float specular_power = param_imp->GetSpecularPower();
		vtkSmartPointer<vtkPiecewiseFunction> gradientOpacity = param_imp->GetGradientFunction();
		vtkSmartPointer<vtkPiecewiseFunction> scalarOpacity = param_imp->GetOpacityFunction();
		vtkSmartPointer<vtkColorTransferFunction> color = param_imp->GetColorFunction();

		vtkSmartPointer<vtkVolumeProperty> volume_property = vtkSmartPointer<vtkVolumeProperty>::New();
		volume_property->ShadeOn();
		volume_property->SetInterpolationType(VTK_LINEAR_INTERPOLATION);
	
		// set light parameters
		volume_property->SetAmbient(ambient);
		volume_property->SetDiffuse(diffuse);
		volume_property->SetSpecular(specular);
		volume_property->SetSpecularPower(specular_power);
		// gradient
		volume_property->SetGradientOpacity(gradientOpacity);
		// opacity
		volume_property->SetScalarOpacity(scalarOpacity);
		// color transfer
		volume_property->SetColor(color);
		// volume 
		vtkSmartPointer<vtkVolume> volume = vtkSmartPointer<vtkVolume>::New();
		volume->SetMapper(data_mapper);
		volume->SetProperty(volume_property);
		// renderer
		vtk_renderer_ = vtkSmartPointer<vtkRenderer>::New();
		renWin->AddRenderer(vtk_renderer_);		
		// 是否离屏渲染===
		if (is_off_screen_rendering_){
			renWin->SetOffScreenRendering(1);
		}
		// add view prop
		vtk_renderer_->AddViewProp(volume);		
		// set window size
		renWin->SetSize(width, height);
	}

#if USE_INTERACTOR
	//vtkSmartPointer<vtkRenderWindowInteractor>  vtk_interactor;
	//vtk_interactor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
 //   vtk_interactor->SetRenderWindow(renWin);
#endif
	
	// set camera to renderer
	vtk_renderer_->SetActiveCamera(camera_->GetCamera());
	vtk_renderer_->ResetCamera();
		
	renWin->Render();

#if USE_INTERACTOR
	//vtk_interactor->Start();
#endif

	// get screenshot from render window  
	vtkSmartPointer<vtkWindowToImageFilter> windowToImageFilter = vtkSmartPointer<vtkWindowToImageFilter>::New();
	windowToImageFilter->SetInput(renWin);
	windowToImageFilter->SetMagnification(1); //set the resolution of the output image (3 times the current resolution of vtk render window)
	windowToImageFilter->SetInputBufferTypeToRGBA(); //also record the alpha (transparency) channel
	windowToImageFilter->ReadFrontBufferOff(); // read from the back buffer
	windowToImageFilter->Update();
	vtk_image_data_ = windowToImageFilter->GetOutput();
}

//此版本从RenderingVolumeFromDICOM示例修改而来
void RayCastingRenderer::DoRender2(vtkSmartPointer<vtkImageData> imagedata)
{
	vtkSmartPointer<vtkRenderWindow> renderWindow = render_window_;
	vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
	vtkSmartPointer<vtkInteractorStyleTrackballCamera> interactorStyle = vtkSmartPointer<vtkInteractorStyleTrackballCamera>::New();
	vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
	vtkSmartPointer<vtkSmartVolumeMapper> volumeMapper = vtkSmartPointer<vtkSmartVolumeMapper>::New();
	vtkSmartPointer<vtkVolumeProperty> volumeProperty = vtkSmartPointer<vtkVolumeProperty>::New();
	vtkSmartPointer<vtkPiecewiseFunction> gradientOpacity = vtkSmartPointer<vtkPiecewiseFunction>::New();
	vtkSmartPointer<vtkPiecewiseFunction> scalarOpacity = vtkSmartPointer<vtkPiecewiseFunction>::New();
	vtkSmartPointer<vtkColorTransferFunction> color = vtkSmartPointer<vtkColorTransferFunction>::New();
	vtkSmartPointer<vtkVolume> volume = vtkSmartPointer<vtkVolume>::New();

	renderer->SetBackground(0.1, 0.2, 0.3);
	renderWindow->AddRenderer(renderer);

	// 是否离屏渲染===
	if (is_off_screen_rendering_){
		renderWindow->SetOffScreenRendering(1);
	}
	renderWindow->SetSize(800, 800);

	renderWindowInteractor->SetInteractorStyle(interactorStyle);
	renderWindowInteractor->SetRenderWindow(renderWindow);

	volumeMapper->SetBlendModeToComposite();
	//no interface in vtk 5.10.1
	//volumeMapper->SetRequestedRenderModeToGPU();
#if VTK_MAJOR_VERSION > 5
	volumeMapper->SetRequestedRenderModeToGPU();
	volumeMapper->SetInputData(imagedata);
#else
	volumeMapper->SetInput(imagedata);
#endif
	//volumeMapper->SetInputData(imageData);

	volumeProperty->ShadeOn();
	volumeProperty->SetInterpolationTypeToLinear();

	volumeProperty->SetAmbient(0.2);
	volumeProperty->SetDiffuse(0.7);
	volumeProperty->SetSpecular(0.3);
	volumeProperty->SetSpecularPower(8.0);

	gradientOpacity->AddPoint(10, 0.0);
	gradientOpacity->AddPoint(90, 0.5);
	gradientOpacity->AddPoint(100, 1.0);
	volumeProperty->SetGradientOpacity(gradientOpacity);

	scalarOpacity->AddPoint(-800.0, 0.0);
	scalarOpacity->AddPoint(-750.0, 0.0);
	scalarOpacity->AddPoint(-350.0, 0.0);
	scalarOpacity->AddPoint(-300.0, 0.0);
	scalarOpacity->AddPoint(-200.0, 0.0);
	scalarOpacity->AddPoint(0.0, 0.0);
	scalarOpacity->AddPoint(10.0, 0.05);
	scalarOpacity->AddPoint(500.0, 0.65);
	scalarOpacity->AddPoint(1200.0, 0.9);
	scalarOpacity->AddPoint(2750.0, 1.0);
	volumeProperty->SetScalarOpacity(scalarOpacity);

	color->AddRGBPoint(190.0, 0.4, 0.2, 0.00);
	color->AddRGBPoint(2000, 1.0, 1.0, 1.0);
	volumeProperty->SetColor(color);

	volume->SetMapper(volumeMapper);
	volume->SetProperty(volumeProperty);
	renderer->AddVolume(volume);
	renderer->ResetCamera();

	renderWindow->Render();
	renderWindowInteractor->Start();


	//// Screenshot  
	//vtkSmartPointer<vtkWindowToImageFilter> windowToImageFilter = vtkSmartPointer<vtkWindowToImageFilter>::New();
	//windowToImageFilter->SetInput(renderWindow);
	//windowToImageFilter->SetMagnification(3); //set the resolution of the output image (3 times the current resolution of vtk render window)
	//windowToImageFilter->SetInputBufferTypeToRGBA(); //also record the alpha (transparency) channel
	//windowToImageFilter->ReadFrontBufferOff(); // read from the back buffer
	//windowToImageFilter->Update();
	//vtk_image_data_ = windowToImageFilter->GetOutput();
}

void RayCastingRenderer::SetOffScreenRendering(bool flag)
{
	is_off_screen_rendering_ = flag;
}

bool RayCastingRenderer::PickByImagePos(const Vector2i& in_point, Vector3d& out_point)
{
	return false;
}

bool RayCastingRenderer::PickByImagePos(const int& x, const int& y, Vector3d& out_point)
{
	return false;
}

bool RayCastingRenderer::PickBy3DPoint(const Vector3d& in_point, int& x, int& y)
{
	return false;
}

float RayCastingRenderer::GetVoxelVolume()
{
	return 0.0;
}

float RayCastingRenderer::GetVoxelVolume(BoundingBox*)
{
	return 0.0;
}

float RayCastingRenderer::CalculateLength(Vector3d&, Vector3d&)
{
	return 0.0;
}

void RayCastingRenderer::GetOrientationTags(Vector3d&, Vector3d&, Vector3d&, Vector3d&)
{

}