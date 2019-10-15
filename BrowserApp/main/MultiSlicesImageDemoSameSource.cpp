#include "stdafx.h"
#include "MultiSlicesImageDemoSameSource.h"

//////////////////////////////////////////////////////////////////////////


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

	m_renderWindow = vtkSmartPointer< vtkRenderWindow >::New();
	m_renderWindow->AddRenderer(m_renderer);
	m_renderWindow->AddRenderer(m_renderer2);
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
	double leftViewStation[4] = { 0.0, 0.0, 0.5, 1.0 };
	double rightViewStation[4] = { 0.5, 0.0, 1.0, 1.0 };

	m_v16 = vtkSmartPointer<vtkDICOMImageReader>::New();
	m_v16->SetDirectoryName("..\\bin\\release\\Skin\\data\\slices1");
	m_v16->Update();

	SetSkinActor(m_v16);
	SetBoneActor(m_v16);


	m_renderer->AddActor(skin);
	m_renderer->ResetCamera();
	m_renderer->DrawOn();
	m_renderer->SetViewport(leftViewStation);



	m_renderer2->AddActor(bone);
	m_renderer2->ResetCamera();
	m_renderer2->DrawOn();
	m_renderer2->SetViewport(rightViewStation);

}

void MultiSlicesImageDemoSameSource::SetMyInteractorStyle(vtkSmartPointer<vtkRenderWindowInteractor> interactor)
{
    vtkSmartPointer<vtkInteractorStyleTrackballCamera> style = vtkSmartPointer<vtkInteractorStyleTrackballCamera>::New();
    interactor->SetInteractorStyle( style );
}

void MultiSlicesImageDemoSameSource::CreatePipeline(vtkSmartPointer<vtkRenderer> renderer)
{
    
}

void MultiSlicesImageDemoSameSource::AddMyActor(vtkSmartPointer<vtkRenderWindowInteractor> interactor)
{

}

void MultiSlicesImageDemoSameSource::SetSkinActor(vtkSmartPointer<vtkDICOMImageReader> v16)
{
	vtkSmartPointer<vtkDICOMImageReader> reader = vtkSmartPointer<vtkDICOMImageReader>::New();

	// Contour 轮廓、等高线
	vtkSmartPointer<vtkContourFilter> skinExtractor =
		vtkSmartPointer<vtkContourFilter>::New();
	skinExtractor->SetInputConnection( reader->GetOutputPort());
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