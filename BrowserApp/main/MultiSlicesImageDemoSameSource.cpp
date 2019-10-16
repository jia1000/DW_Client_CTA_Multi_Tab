#include "stdafx.h"
#include "MultiSlicesImageDemoSameSource.h"

//////////////////////////////////////////////////////////////////////////

#if 1
myInteractorStyle* myInteractorStyle::New()
{
	return new myInteractorStyle;
}

void    myInteractorStyle::SetLeftRender(vtkRenderer *render)
{
	this->m_leftRender = render;
}

void    myInteractorStyle::SetRightRender(vtkRenderer *render)
{
	this->m_rightRender = render;
}

void    myInteractorStyle::OnMouseMove()
{
	vtkInteractorStyleTrackballCamera::OnMouseMove();
	this->CameraSynchronous();
}

void myInteractorStyle::OnLeftButtonDown()
{
	vtkInteractorStyleTrackballCamera::OnLeftButtonDown();
	this->CameraSynchronous();
}

void myInteractorStyle::OnLeftButtonUp()
{
	vtkInteractorStyleTrackballCamera::OnLeftButtonUp();
	this->CameraSynchronous();
}

void myInteractorStyle::OnMiddleButtonDown()
{
	vtkInteractorStyleTrackballCamera::OnMiddleButtonDown();
	this->CameraSynchronous();
}

void myInteractorStyle::OnMiddleButtonUp()
{
	vtkInteractorStyleTrackballCamera::OnMiddleButtonUp();
	this->CameraSynchronous();
}

void myInteractorStyle::OnRightButtonDown()
{
	vtkInteractorStyleTrackballCamera::OnRightButtonDown();
	this->CameraSynchronous();
}

void myInteractorStyle::OnRightButtonUp()
{
	vtkInteractorStyleTrackballCamera::OnRightButtonUp();
	this->CameraSynchronous();
}

void myInteractorStyle::OnMouseWheelForward()
{
	vtkInteractorStyleTrackballCamera::OnMouseWheelForward();
	this->CameraSynchronous();
}

void myInteractorStyle::OnMouseWheelBackward()
{
	vtkInteractorStyleTrackballCamera::OnMouseWheelBackward();
	this->CameraSynchronous();
}

void myInteractorStyle:: Rotate()
{
	vtkInteractorStyleTrackballCamera::Rotate();
	this->CameraSynchronous();
}

void myInteractorStyle::Spin()
{
	vtkInteractorStyleTrackballCamera::Spin();
	this->CameraSynchronous();
}

void myInteractorStyle::Pan()
{
	vtkInteractorStyleTrackballCamera::Pan();
	this->CameraSynchronous();
}

void myInteractorStyle::Dolly()
{
	vtkInteractorStyleTrackballCamera::Dolly();
	this->CameraSynchronous();
}

void    myInteractorStyle::CameraSynchronous()
{
	vtkCamera *leftCamera = m_leftRender->GetActiveCamera();
	vtkCamera *rightCamera = m_rightRender->GetActiveCamera();

	//获取窗口大小
	int *winSize = this->GetInteractor()->GetRenderWindow()->GetSize();

	//获取事件窗口位置
	int eventStation[3];
	this->GetInteractor()->GetEventPosition(eventStation);

	if (eventStation[0] < winSize[0] / 2)
	{
		rightCamera->SetPosition(leftCamera->GetPosition());
		rightCamera->SetFocalPoint(leftCamera->GetFocalPoint());
		rightCamera->SetViewUp(leftCamera->GetViewUp());
		m_rightRender->ResetCameraClippingRange();

	}
	else
	{
		leftCamera->SetPosition(rightCamera->GetPosition());
		leftCamera->SetViewUp(rightCamera->GetViewUp());
		leftCamera->SetFocalPoint(rightCamera->GetFocalPoint());
		m_leftRender->ResetCameraClippingRange();

	}

	this->GetInteractor()->GetRenderWindow()->Render();
}
#endif
//////////////////////////////////////////////////////////////////////////

// 创建一个序列的三维重建
MultiSlicesImageDemoSameSource::MultiSlicesImageDemoSameSource(HWND parent, CButtonUI* pVtkShowBtn, bool is_multi_actor) : CWidgetsDemoBase(parent, pVtkShowBtn)
{
	
}


MultiSlicesImageDemoSameSource::~MultiSlicesImageDemoSameSource(void)
{
	
}

void MultiSlicesImageDemoSameSource::CreateRendererAndRenderWindowAndInteractor()
{
	m_renderer = vtkSmartPointer<vtkRenderer>::New();
	m_renderer->SetBackground(0.1, 0.2, 0.4);

	m_renderer2 = vtkSmartPointer<vtkRenderer>::New();
	m_renderer2->SetBackground(0.4, 0.2, 0.1);

	m_renderer3 = vtkSmartPointer<vtkRenderer>::New();
	m_renderer3->SetBackground(0.4, 0.2, 0.1);

	m_renderer4 = vtkSmartPointer<vtkRenderer>::New();
	m_renderer4->SetBackground(0.1, 0.2, 0.4);

	m_renderWindow = vtkSmartPointer< vtkRenderWindow >::New();
	m_renderWindow->AddRenderer(m_renderer);
	m_renderWindow->AddRenderer(m_renderer2);
	m_renderWindow->AddRenderer(m_renderer3);
	m_renderWindow->AddRenderer(m_renderer4);
	m_renderWindow->SetWindowName("AnnotationWidget");

	m_interactor = vtkSmartPointer< vtkRenderWindowInteractor >::New();
	vtkSmartPointer<myInteractorStyle> style = vtkSmartPointer<myInteractorStyle>::New();
	style->SetLeftRender(m_renderer);
	style->SetRightRender(m_renderer2);
	m_interactor->SetInteractorStyle(style);

	m_interactor->SetRenderWindow(m_renderWindow);
}

void MultiSlicesImageDemoSameSource::StartWidgetsRender()
{
	double leftViewStation[4] = { 0.0, 0.5, 0.5, 1.0 };
	//double rightViewStation[4] = { 0.5, 0.0, 1.0, 1.0 };
	double rightViewStation[4] = { 0.5, 0.5, 1.0, 1.0 };
	double leftdownViewStation[4] = { 0.0, 0.0, 0.5, 0.5 };
	double rightdownViewStation[4] = { 0.5, 0.0, 1.0, 0.5 };


	m_v16 = vtkSmartPointer<vtkDICOMImageReader>::New();
	m_v16->SetDirectoryName("..\\bin\\release\\Skin\\data\\slices1");
	m_v16->Update();

	int *data_extent;
	data_extent = m_v16->GetDataExtent();

	for (int Index = 0; Index < 6 ; Index++) {
		m_data_extent[Index] = data_extent[Index];
	}

	m_bwLut = vtkSmartPointer<vtkLookupTable>::New();
	m_bwLut->SetTableRange (0, 2000);	
	m_bwLut->SetSaturationRange (0, 0);	// 设置色彩饱和度
	m_bwLut->SetHueRange (0, 0);		// 设置色度/色调范围
	m_bwLut->SetValueRange (0, 1);
	m_bwLut->Build(); 

	// 用这个验证函数的使用
	m_hueLut = vtkSmartPointer<vtkLookupTable>::New();
	m_hueLut->SetTableRange (0, 2000);
	m_hueLut->SetHueRange (0, 01);		// 设置色度/色调范围
	m_hueLut->SetSaturationRange (0, 0);// 设置色彩饱和度
	m_hueLut->SetValueRange (0, 1);
	m_hueLut->Build();

	m_cur_hue_coronal_min = 0.6;
	m_cur_hue_coronal_max = 0.6;

	m_satLut = vtkSmartPointer<vtkLookupTable>::New();
	m_satLut->SetTableRange (0, 2000);
	m_satLut->SetHueRange (0, 0);		// 设置色度/色调范围
	m_satLut->SetSaturationRange (0, 0);// 设置色彩饱和度
	m_satLut->SetValueRange (0, 1);
	m_satLut->Build();

	SetSkinActor(m_v16);
	SetBoneActor(m_v16);
	SetOutlineActor(m_v16);
	// 这是3个轴面的slice显示
	SetSagittalActor(m_v16, m_bwLut);
	SetAxialActor(m_v16, m_hueLut);
	SetCoronalActor(m_v16, m_satLut);

	m_renderer->AddActor(sagittal); 
	vtkCamera* cam1 = m_renderer->GetActiveCamera();
	cam1->SetFocalPoint(0,0,0);
	cam1->SetPosition(0, -1, 0);
	cam1->SetViewUp(0,0,1);
	//m_renderer->ResetCamera();
	m_renderer->DrawOn();
	m_renderer->SetViewport(leftViewStation);

	ResizeAndPosition();

	m_renderer2->AddActor(axial); 
	vtkCamera* cam2 = m_renderer2->GetActiveCamera();
	cam2->SetFocalPoint(0,0,0);
	cam2->SetPosition(0,0,1);
	cam2->SetViewUp(0,1,0);//m_renderer2->ResetCamera();
	m_renderer2->DrawOn();
	m_renderer2->SetViewport(rightViewStation);

	m_renderer3->AddActor(coronal);  
	vtkCamera* cam3 = m_renderer3->GetActiveCamera();
	cam3->SetFocalPoint(0,0,0);
	cam3->SetPosition(1,0,0);
	cam3->SetViewUp(0,0,1);
	//m_renderer3->ResetCamera();
	m_renderer3->DrawOn();
	m_renderer3->SetViewport(leftdownViewStation);

	m_renderer4->AddActor(skin);  
	m_renderer4->AddActor(bone);
	m_renderer4->AddActor(outline);
	//m_renderer4->AddActor(sagittal);
	//m_renderer4->AddActor(axial);
	//m_renderer4->AddActor(coronal);

	//m_renderer4->ResetCamera();
	m_renderer4->DrawOn();
	m_renderer4->SetViewport(rightdownViewStation);

	m_renderWindow->Render();
}

void MultiSlicesImageDemoSameSource::SetMyInteractorStyle(vtkSmartPointer<vtkRenderWindowInteractor> interactor)
{
    
}

void MultiSlicesImageDemoSameSource::CreatePipeline(vtkSmartPointer<vtkRenderer> renderer)
{
    
}

void MultiSlicesImageDemoSameSource::AddMyActor(vtkSmartPointer<vtkRenderWindowInteractor> interactor)
{

}

void MultiSlicesImageDemoSameSource::SetSkinActor(vtkSmartPointer<vtkDICOMImageReader> v16)
{
	//vtkSmartPointer<vtkDICOMImageReader> reader = vtkSmartPointer<vtkDICOMImageReader>::New();

	// Contour 轮廓、等高线
	vtkSmartPointer<vtkContourFilter> skinExtractor =
		vtkSmartPointer<vtkContourFilter>::New();
	skinExtractor->SetInputConnection( v16->GetOutputPort());
	skinExtractor->SetValue(0, 500);
	//skinExtractor->Update();

	// Poly Data 多边形数据
	vtkSmartPointer<vtkPolyDataNormals> skinNormals =
		vtkSmartPointer<vtkPolyDataNormals>::New();
	skinNormals->SetInputConnection(skinExtractor->GetOutputPort());
	skinNormals->SetFeatureAngle(60.0);
	//skinNormals->Update();

	// stripper 剥离器
	vtkSmartPointer<vtkStripper> skinStripper =
		vtkSmartPointer<vtkStripper>::New();
	skinStripper->SetInputConnection(skinNormals->GetOutputPort());
	//skinStripper->Update();

	vtkSmartPointer<vtkPolyDataMapper> skinMapper =
		vtkSmartPointer<vtkPolyDataMapper>::New();
	skinMapper->SetInputConnection(skinStripper->GetOutputPort());
	// scalar标量、梯状、分等级
	skinMapper->ScalarVisibilityOff();

	skin = vtkSmartPointer<vtkActor>::New();
	skin->SetMapper(skinMapper);
	skin->GetProperty()->SetDiffuseColor(1, .49, .25); // diffuse 弥漫  散射
	skin->GetProperty()->SetSpecular(.3);  // Specular 反射
	skin->GetProperty()->SetSpecularPower(20);
}

void MultiSlicesImageDemoSameSource::SetBoneActor(vtkSmartPointer<vtkDICOMImageReader> v16)
{
	vtkSmartPointer<vtkContourFilter> boneExtractor =
		vtkSmartPointer<vtkContourFilter>::New();
	boneExtractor->SetInputConnection(v16->GetOutputPort());
	boneExtractor->SetValue(0, 1150);

	vtkSmartPointer<vtkPolyDataNormals> boneNormals =
		vtkSmartPointer<vtkPolyDataNormals>::New();
	boneNormals->SetInputConnection(boneExtractor->GetOutputPort());
	boneNormals->SetFeatureAngle(60.0);

	vtkSmartPointer<vtkStripper> boneStripper =
		vtkSmartPointer<vtkStripper>::New();
	boneStripper->SetInputConnection(boneNormals->GetOutputPort());

	vtkSmartPointer<vtkPolyDataMapper> boneMapper =
		vtkSmartPointer<vtkPolyDataMapper>::New();
	boneMapper->SetInputConnection(boneStripper->GetOutputPort());
	boneMapper->ScalarVisibilityOff();

	bone = vtkSmartPointer<vtkActor>::New();
	bone->SetMapper(boneMapper);
	bone->GetProperty()->SetDiffuseColor(1, 1, .9412);
}

void MultiSlicesImageDemoSameSource::SetOutlineActor(vtkSmartPointer<vtkDICOMImageReader> v16)
{
	vtkSmartPointer<vtkOutlineFilter> outlineData =
		vtkSmartPointer<vtkOutlineFilter>::New();
	outlineData->SetInputConnection(v16->GetOutputPort());
	//outlineData->Update();

	vtkSmartPointer<vtkPolyDataMapper> mapOutline =
		vtkSmartPointer<vtkPolyDataMapper>::New();
	mapOutline->SetInputConnection(outlineData->GetOutputPort());

	outline = vtkSmartPointer<vtkActor>::New();
	outline->SetMapper(mapOutline);
	outline->GetProperty()->SetColor(0,0,0);
}

void MultiSlicesImageDemoSameSource::SetSagittalActor(vtkSmartPointer<vtkDICOMImageReader> v16, vtkSmartPointer<vtkLookupTable> lut)
{
	vtkSmartPointer<vtkImageMapToColors> sagittalColors = vtkSmartPointer<vtkImageMapToColors>::New();
	sagittalColors->SetInputConnection(v16->GetOutputPort());
	sagittalColors->SetLookupTable(lut);
	//sagittalColors->Update();

	sagittal = vtkSmartPointer<vtkImageActor>::New();
	sagittal->GetMapper()->SetInputConnection(sagittalColors->GetOutputPort());
	//sagittal->SetDisplayExtent(32,32, 0,63, 0,92);
	int adv = m_data_extent[1] - m_data_extent[0] + 1;
	m_cur_sagitta = adv / 2;
	// 设置显示3D切层的位置
	sagittal->SetDisplayExtent(m_cur_sagitta, m_cur_sagitta, m_data_extent[2],m_data_extent[3], m_data_extent[4],m_data_extent[5]);

}

void MultiSlicesImageDemoSameSource::SetAxialActor(vtkSmartPointer<vtkDICOMImageReader> v16, vtkSmartPointer<vtkLookupTable> lut)
{
	vtkSmartPointer<vtkImageMapToColors> axialColors = vtkSmartPointer<vtkImageMapToColors>::New();
	axialColors->SetInputConnection(v16->GetOutputPort());
	axialColors->SetLookupTable(lut);
	//axialColors->Update();

	axial =	vtkSmartPointer<vtkImageActor>::New();
	axial->GetMapper()->SetInputConnection(axialColors->GetOutputPort());
	//axial->SetDisplayExtent(0,63, 0,63, 46,46);
	int adv = m_data_extent[5] - m_data_extent[4] + 1;
	m_cur_axial = adv / 2;
	// 设置显示3D切层的位置
	axial->SetDisplayExtent(m_data_extent[0],m_data_extent[1], m_data_extent[2],m_data_extent[3], m_cur_axial, m_cur_axial);

}

void MultiSlicesImageDemoSameSource::SetCoronalActor(vtkSmartPointer<vtkDICOMImageReader> v16, vtkSmartPointer<vtkLookupTable> lut)
{
	vtkSmartPointer<vtkImageMapToColors> coronalColors =	vtkSmartPointer<vtkImageMapToColors>::New();
	coronalColors->SetInputConnection(v16->GetOutputPort());
	coronalColors->SetLookupTable(lut);
	//coronalColors->Update();

	coronal = vtkSmartPointer<vtkImageActor>::New();
	coronal->GetMapper()->SetInputConnection(coronalColors->GetOutputPort());
	//coronal->SetDisplayExtent(0,63, 32,32, 0,92);
	int adv = m_data_extent[3] - m_data_extent[2] + 1;
	m_cur_cornal = adv / 2;
	// 设置显示3D切层的位置
	coronal->SetDisplayExtent(m_data_extent[0],m_data_extent[1], m_cur_cornal, m_cur_cornal, m_data_extent[4],m_data_extent[5]);
}