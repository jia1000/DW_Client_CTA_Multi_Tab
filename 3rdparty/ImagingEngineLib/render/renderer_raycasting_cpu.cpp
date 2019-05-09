
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
	render_mode_ = RenderMode::RAYCASTING;
}

RayCastingRenderer::~RayCastingRenderer()
{
	
}


//此版本从dr.wise客户端修改而来
void RayCastingRenderer::DoRender(vtkSmartPointer<vtkImageData> imagedata)
{
//	if (render_param_ == NULL) return;
//	VRRenderParam* param_imp = (VRRenderParam *)render_param_;
//	// 深拷贝参数？
//	camera_ = param_imp->GetCamera();
//	
//	vtkSmartPointer<vtkRenderWindow> renWin = render_window_;	
//	// 从render_param_取出渲染参数
//	if (is_first_render_){
//		is_first_render_ = false;
//		vtkSmartPointer<vtkSmartVolumeMapper> volume_mapper = vtkSmartPointer<vtkSmartVolumeMapper>::New();
//		volume_mapper->SetBlendModeToComposite();
//#if VTK_MAJOR_VERSION > 5
//		volume_mapper->SetInputData(imagedata);
//		volume_mapper->SetRequestedRenderModeToRayCast();
//#else
//		volume_mapper->SetInputConnection(imagedata->GetProducerPort());
//		volume_mapper->SetRequestedRenderMode(vtkSmartVolumeMapper::RayCastRenderMode);
//#endif
//		// get render parameters
//		int width = param_imp->GetWidth();
//		int height = param_imp->GetHeight();
//		float ambient = param_imp->GetAmbient();
//		float diffuse = param_imp->GetDiffuse();
//		float specular = param_imp->GetSpecular();
//		float specular_power = param_imp->GetSpecularPower();
//		vtkSmartPointer<vtkPiecewiseFunction> gradientOpacity = param_imp->GetGradientFunction();
//		vtkSmartPointer<vtkPiecewiseFunction> scalarOpacity = param_imp->GetOpacityFunction();
//		vtkSmartPointer<vtkColorTransferFunction> color = param_imp->GetColorFunction();
//
//		vtkSmartPointer<vtkVolumeProperty> volume_property = vtkSmartPointer<vtkVolumeProperty>::New();
//		volume_property->ShadeOn();
//		volume_property->SetInterpolationType(VTK_LINEAR_INTERPOLATION);
//	
//		// set light parameters
//		volume_property->SetAmbient(ambient);
//		volume_property->SetDiffuse(diffuse);
//		volume_property->SetSpecular(specular);
//		volume_property->SetSpecularPower(specular_power);
//		// gradient
//		volume_property->SetGradientOpacity(gradientOpacity);
//		// opacity
//		volume_property->SetScalarOpacity(scalarOpacity);
//		// color transfer
//		volume_property->SetColor(color);
//		// volume 
//		vtkSmartPointer<vtkVolume> volume = vtkSmartPointer<vtkVolume>::New();
//		volume->SetMapper(volume_mapper);
//		volume->SetProperty(volume_property);
//		// renderer
//		vtk_renderer_ = vtkSmartPointer<vtkRenderer>::New();
//		renWin->AddRenderer(vtk_renderer_);		
//		// 是否离屏渲染===
//		if (is_off_screen_rendering_){
//			renWin->SetOffScreenRendering(1);
//		}
//		// add view prop
//		vtk_renderer_->AddViewProp(volume);		
//		// set window size
//		renWin->SetSize(width, height);
//	}
//
//#if USE_INTERACTOR
//	//vtkSmartPointer<vtkRenderWindowInteractor>  vtk_interactor;
//	//vtk_interactor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
// //   vtk_interactor->SetRenderWindow(renWin);
//#endif
//	
//	// set camera to renderer
//	vtk_renderer_->SetActiveCamera(camera_->GetCamera());
//	vtk_renderer_->ResetCamera();
//		
//	renWin->Render();
//
//#if USE_INTERACTOR
//	//vtk_interactor->Start();
//#endif
//
//	// get screenshot from render window  
//	vtkSmartPointer<vtkWindowToImageFilter> windowToImageFilter = vtkSmartPointer<vtkWindowToImageFilter>::New();
//	windowToImageFilter->SetInput(renWin);
//	windowToImageFilter->SetMagnification(1); //set the resolution of the output image (3 times the current resolution of vtk render window)
//	windowToImageFilter->SetInputBufferTypeToRGBA(); //also record the alpha (transparency) channel
//	windowToImageFilter->ReadFrontBufferOff(); // read from the back buffer
//	windowToImageFilter->Update();
//	vtk_image_data_ = windowToImageFilter->GetOutput();
}

//此版本从RenderingVolumeFromDICOM示例修改而来
void RayCastingRenderer::DoRender2(vtkSmartPointer<vtkImageData> imagedata)
{
	
}