
#include "renderer_raycasting_gpu.h"
#include <vtkWindowToImageFilter.h>

using namespace DW::Render;

RayCastingRendererGPU::RayCastingRendererGPU()
{

}

RayCastingRendererGPU::~RayCastingRendererGPU()
{
	
}

//此版本从dr.wise客户端修改而来
void RayCastingRendererGPU::DoRender(vtkSmartPointer<vtkImageData> imagedata)
{
	vtkSmartPointer<vtkSmartVolumeMapper> data_mapper = vtkSmartPointer<vtkSmartVolumeMapper>::New();
    data_mapper->SetBlendModeToMaximumIntensity();
	data_mapper->SetRequestedRenderMode(vtkSmartVolumeMapper::RayCastRenderMode);
#if VTK_MAJOR_VERSION > 5
	data_mapper->SetInputData(imagedata);
#else
	data_mapper->SetInputConnection(imagedata->GetProducerPort());
#endif
	
    vtkSmartPointer<vtkVolumeProperty> volume_property = vtkSmartPointer<vtkVolumeProperty>::New();
    volume_property->ShadeOn();
    volume_property->SetInterpolationType(VTK_LINEAR_INTERPOLATION);
	
	volume_property->SetAmbient(0.2);
	volume_property->SetDiffuse(0.7);
	volume_property->SetSpecular(0.3);
	volume_property->SetSpecularPower(8.0);

	vtkSmartPointer<vtkPiecewiseFunction> gradientOpacity = vtkSmartPointer<vtkPiecewiseFunction>::New();
	gradientOpacity->AddPoint(10, 0.0);
	gradientOpacity->AddPoint(90, 0.5);
	gradientOpacity->AddPoint(100, 1.0);
	volume_property->SetGradientOpacity(gradientOpacity);

    vtkSmartPointer<vtkPiecewiseFunction> scalarOpacity = vtkSmartPointer<vtkPiecewiseFunction>::New();
    //scalarOpacity->AddSegment( level - 0.5 * window, 0.0, level + 0.5 * window, 1.0 );
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
    volume_property->SetScalarOpacity(scalarOpacity); // composite first.

    vtkSmartPointer<vtkColorTransferFunction> color = vtkSmartPointer<vtkColorTransferFunction>::New();
    //color->AddRGBSegment(0.0, 1.0, 1.0, 1.0, 255.0, 1.0, 1.0, 1.0 );
	color->AddRGBPoint(190.0, 0.4, 0.2, 0.00);
	color->AddRGBPoint(2000, 1.0, 1.0, 1.0);
    volume_property->SetColor(color);


    vtkSmartPointer<vtkVolume> volume = vtkSmartPointer<vtkVolume>::New();
    volume->SetMapper(data_mapper);
    volume->SetProperty(volume_property);
    
	vtkSmartPointer<vtkRenderWindowInteractor>  vtk_interactor;
    vtkSmartPointer<vtkRenderer> m_3d_render;
	vtk_interactor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
    vtk_interactor->SetRenderWindow(vtkSmartPointer<vtkRenderWindow>::New());

    vtkSmartPointer<vtkRenderWindow> renWin = render_window_;
	
	// 是否离屏渲染===
	if (is_off_screen_rendering_){
		renWin->SetOffScreenRendering(1);
	}

    m_3d_render = vtkSmartPointer<vtkRenderer>::New();
    renWin->AddRenderer(m_3d_render);
    m_3d_render->AddViewProp(volume);
    m_3d_render->ResetCamera();

	renWin->Render();
	vtk_interactor->Start();

	// Screenshot  
	vtkSmartPointer<vtkWindowToImageFilter> windowToImageFilter = vtkSmartPointer<vtkWindowToImageFilter>::New();
	windowToImageFilter->SetInput(renWin);
	windowToImageFilter->SetMagnification(1); //set the resolution of the output image (3 times the current resolution of vtk render window)
	windowToImageFilter->SetInputBufferTypeToRGBA(); //also record the alpha (transparency) channel
	windowToImageFilter->ReadFrontBufferOff(); // read from the back buffer
	windowToImageFilter->Update();
	vtk_image_data_ = windowToImageFilter->GetOutput();
}
