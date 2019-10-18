#include "stdafx.h"
#include "widgets_mpr_vtk2.h"

#include "main/vtk_3d/cross_view_vtk_actor.h"
#include "main/vtk_3d/cross_view_vtk_interactor_style.h"

//double leftViewStation[4] = { 0.0, 0.5, 0.5, 1.0 };
////double rightViewStation[4] = { 0.5, 0.0, 1.0, 1.0 };
//double rightViewStation[4] = { 0.5, 0.5, 1.0, 1.0 };
//double leftdownViewStation[4] = { 0.0, 0.0, 0.5, 0.5 };
//double rightdownViewStation[4] = { 0.5, 0.0, 1.0, 0.5 };

WidgetsMprVtk2::WidgetsMprVtk2(HWND parent, std::vector<CButtonUI*> vVtkShowBtns)
    : WidgetsMprVtk(parent, vVtkShowBtns[0])
{
    m_parentWnd = parent;
    m_vVtkShowBtns.clear();
    for (int i = 0; i < vVtkShowBtns.size(); ++i) {
        m_vVtkShowBtns.push_back(vVtkShowBtns[i]);
    }
}


WidgetsMprVtk2::~WidgetsMprVtk2(void)
{
}

void WidgetsMprVtk2::ShowWidgets_Test()
{
    // Create the RenderWindow, Renderer and both Actors
	CreateRendererAndRenderWindowAndInteractor();
    
    StartWidgetsRender();
}

void WidgetsMprVtk2::CreateRendererAndRenderWindowAndInteractor()
{
    for (int i = 0; i < m_vVtkShowBtns.size(); ++i) {
        vtkSmartPointer< vtkRenderWindow > renderWindow = vtkSmartPointer< vtkRenderWindow >::New();
        std::string annotation = "AnnotationWidget";
        std::string s_index = to_string(i + 1);
        annotation += s_index;
        renderWindow->SetWindowName(annotation.c_str());
        m_renderWindows.push_back(renderWindow);
    }
}

void WidgetsMprVtk2::SetOneRendererSlice(int delta_slice, int eventStation[3])
{
    int* winSize = m_renderWindow->GetSize();

    if (eventStation[0] < winSize[0] / 2 && eventStation[1] < winSize[1] / 2) {
        sagittal_normal->SetSlice(delta_slice);
    }
    else if (eventStation[0] > winSize[0] / 2 && eventStation[1] > winSize[1] / 2) {
        coronal_normal->SetSlice(delta_slice);
    }
    else if (eventStation[0] < winSize[0] / 2 && eventStation[1] > winSize[1] / 2) {
        axial_normal->SetSlice(delta_slice);
    }
    else {
        axial_normal->SetSlice(delta_slice);
    }
}

void WidgetsMprVtk2::ResizeAndPosition()
{
	if (m_parentWnd) {
        for (int i = 0; i < m_vVtkShowBtns.size(); ++i) {
            CButtonUI* pVtkShowBtn = m_vVtkShowBtns[i];
            RECT rc;
			if (pVtkShowBtn) {
				rc = pVtkShowBtn->GetClientPos();
				pVtkShowBtn->SetText(_T("Please wait..."));
			}
	
			m_renderWindows[i]->SetParentId(m_parentWnd);
			int title_height = 0;//32;
			int button_test_height = 0;//30;
			RECT rect;
			::GetClientRect(m_parentWnd, &rect);
			rect = rc;
            m_renderWindows[i]->SetSize(rect.right - rect.left, rect.bottom - rect.top - title_height - button_test_height);
			m_renderWindows[i]->SetPosition(rect.left, rect.top + title_height + button_test_height);
		}
	}
}

void WidgetsMprVtk2::StartWidgetsRender()
{
    m_v16 = vtkSmartPointer<vtkDICOMImageReader>::New();
    m_v16->SetDirectoryName("..\\bin\\release\\Skin\\data\\slices1");
    m_v16->Update();

    int* data_extent;
    data_extent = m_v16->GetDataExtent();

    for (int Index = 0; Index < 6; Index++) {
        m_data_extent[Index] = data_extent[Index];
    }

    m_cur_hue_coronal_min = 0.6;
    m_cur_hue_coronal_max = 0.6;
    
    ResizeAndPosition();

    // 横状面  -- CT的切层 
    SetAxialRendererNormal(m_v16);

    // 冠状面  -- 人的正面
    SetCoronalRendererNormal(m_v16);

    // 矢状面  -- 人的侧面
    SetSagittalRendererNormal(m_v16);

    // mpr
    SetMprRendererNormal(m_v16);

    for (auto render_window : m_renderWindows) {
        render_window->Render();
    }
}

void WidgetsMprVtk2::SetAxialRendererNormal(vtkSmartPointer<vtkDICOMImageReader> v16)
{
    m_renderer = vtkSmartPointer<vtkRenderer>::New();
    m_renderer->SetBackground(0.1, 0.2, 0.4);

    vtkSmartPointer<vtkImageMapToColors> axialColors = vtkSmartPointer<vtkImageMapToColors>::New();
    axialColors->SetInputConnection(v16->GetOutputPort());

    axial_normal = vtkSmartPointer<CrossViewVtkActorAxial>::New();
    axial_normal->SetRenerWindow(m_renderWindows[0]);
    axial_normal->SetInputConnection(v16);
    axial_normal->GetMapper()->SetInputConnection(axialColors->GetOutputPort());
    
    m_renderer->AddActor(axial_normal);
    vtkCamera* cam1 = m_renderer->GetActiveCamera();
    cam1->SetFocalPoint(0, 0, 0);
    cam1->SetPosition(0, 0, 1);
    cam1->SetViewUp(0, 1, 0);
    m_renderer->ResetCamera();
    m_renderer->DrawOn();
    //m_renderer->SetViewport(leftViewStation);

    m_renderWindows[0]->AddRenderer(m_renderer);

    vtkSmartPointer<CrossViewVtkInteractorStyle> style = vtkSmartPointer<CrossViewVtkInteractorStyle>::New();
    style->SetImageViewer(this);
    style->SetImageActor(axial_normal);
    m_interactor1 = vtkSmartPointer< vtkRenderWindowInteractor >::New();
    m_interactor1->SetInteractorStyle(style);

    m_interactor1->SetRenderWindow(m_renderWindows[0]);

}

void WidgetsMprVtk2::SetCoronalRendererNormal(vtkSmartPointer<vtkDICOMImageReader> v16)
{
    m_renderer2 = vtkSmartPointer<vtkRenderer>::New();
    m_renderer2->SetBackground(0.4, 0.2, 0.1);

    vtkSmartPointer<vtkImageMapToColors> coronalColors = vtkSmartPointer<vtkImageMapToColors>::New();
    coronalColors->SetInputConnection(v16->GetOutputPort());

    coronal_normal = vtkSmartPointer<CrossViewVtkActorCoronal>::New();
    coronal_normal->SetRenerWindow(m_renderWindows[1]);
    coronal_normal->SetInputConnection(v16);
    coronal_normal->GetMapper()->SetInputConnection(coronalColors->GetOutputPort());
    
    m_renderer2->AddActor(coronal_normal);
    vtkCamera* cam2 = m_renderer2->GetActiveCamera();
    cam2->SetFocalPoint(0, 0, 0);
    cam2->SetPosition(0, -1, 0);
    cam2->SetViewUp(0, 0, -1);
    m_renderer2->ResetCamera();
    m_renderer2->DrawOn();
    //m_renderer2->SetViewport(rightViewStation);

    m_renderWindows[1]->AddRenderer(m_renderer2);

    vtkSmartPointer<CrossViewVtkInteractorStyle> style = vtkSmartPointer<CrossViewVtkInteractorStyle>::New();
    style->SetImageViewer(this);
    style->SetImageActor(coronal_normal);
    m_interactor2 = vtkSmartPointer< vtkRenderWindowInteractor >::New();
    m_interactor2->SetInteractorStyle(style);
                
    m_interactor2->SetRenderWindow(m_renderWindows[1]);
}

void WidgetsMprVtk2::SetSagittalRendererNormal(vtkSmartPointer<vtkDICOMImageReader> v16)
{
    m_renderer3 = vtkSmartPointer<vtkRenderer>::New();
    m_renderer3->SetBackground(0.4, 0.2, 0.1);

    vtkSmartPointer<vtkImageMapToColors> sagittalColors = vtkSmartPointer<vtkImageMapToColors>::New();
    sagittalColors->SetInputConnection(v16->GetOutputPort());

    sagittal_normal = vtkSmartPointer<CrossViewVtkActorSagittal>::New();    
    sagittal_normal->SetRenerWindow(m_renderWindows[2]);
    sagittal_normal->SetInputConnection(v16);
    sagittal_normal->GetMapper()->SetInputConnection(sagittalColors->GetOutputPort());

    m_renderer3->AddActor(sagittal_normal);
    vtkCamera* cam3 = m_renderer3->GetActiveCamera();
    cam3->SetFocalPoint(0, 0, 0);
    cam3->SetPosition(-1, 0, 0);
    cam3->SetViewUp(0, 0, 1);
    m_renderer3->ResetCamera();
    m_renderer3->DrawOn();
    //m_renderer3->SetViewport(leftdownViewStation);

    m_renderWindows[2]->AddRenderer(m_renderer3);

    vtkSmartPointer<CrossViewVtkInteractorStyle> style = vtkSmartPointer<CrossViewVtkInteractorStyle>::New();
    style->SetImageViewer(this);
    style->SetImageActor(sagittal_normal);
    m_interactor3 = vtkSmartPointer< vtkRenderWindowInteractor >::New();
    m_interactor3->SetInteractorStyle(style);
    
    m_interactor3->SetRenderWindow(m_renderWindows[2]);

}

void WidgetsMprVtk2::SetMprRendererNormal(vtkSmartPointer<vtkDICOMImageReader> v16)
{
    m_renderer4 = vtkSmartPointer<vtkRenderer>::New();
    m_renderer4->SetBackground(0.1, 0.2, 0.4);

    m_bwLut = vtkSmartPointer<vtkLookupTable>::New();
    m_bwLut->SetTableRange(0, 2000);
    m_bwLut->SetSaturationRange(0, 0);	// 设置色彩饱和度
    m_bwLut->SetHueRange(0, 0);		// 设置色度/色调范围
    m_bwLut->SetValueRange(0, 1);
    m_bwLut->Build();

    // 用这个验证函数的使用
    m_hueLut = vtkSmartPointer<vtkLookupTable>::New();
    m_hueLut->SetTableRange(0, 2000);
    m_hueLut->SetHueRange(0, 01);		// 设置色度/色调范围
    m_hueLut->SetSaturationRange(0, 0);// 设置色彩饱和度
    m_hueLut->SetValueRange(0, 1);
    m_hueLut->Build();

    m_satLut = vtkSmartPointer<vtkLookupTable>::New();
    m_satLut->SetTableRange(0, 2000);
    m_satLut->SetHueRange(0, 0);		// 设置色度/色调范围
    m_satLut->SetSaturationRange(0, 0);// 设置色彩饱和度
    m_satLut->SetValueRange(0, 1);
    m_satLut->Build();

    SetSkinActor(m_v16);
    SetBoneActor(m_v16);
    SetOutlineActor(m_v16);
    // 这是3个轴面的slice显示
    SetSagittalActor(m_v16, m_bwLut);
    SetAxialActor(m_v16, m_hueLut);
    SetCoronalActor(m_v16, m_satLut);

    // 三维图像
    m_renderer4->AddActor(skin);
    m_renderer4->AddActor(bone);
    m_renderer4->AddActor(outline);
    m_renderer4->AddActor(sagittal);
    m_renderer4->AddActor(axial);
    m_renderer4->AddActor(coronal);
    m_renderer4->DrawOn();
    //m_renderer4->SetViewport(rightdownViewStation);

    m_renderWindows[3]->AddRenderer(m_renderer4);

    vtkSmartPointer<vtkInteractorStyleTrackballCamera> style = vtkSmartPointer<vtkInteractorStyleTrackballCamera>::New();
    m_interactor4 = vtkSmartPointer< vtkRenderWindowInteractor >::New();
    m_interactor4->SetInteractorStyle(style);

    m_interactor4->SetRenderWindow(m_renderWindows[3]);
}

void WidgetsMprVtk2::SetSkinActor(vtkSmartPointer<vtkDICOMImageReader> v16)
{
    // Contour 轮廓、等高线
    vtkSmartPointer<vtkContourFilter> skinExtractor = vtkSmartPointer<vtkContourFilter>::New();
    skinExtractor->SetInputConnection(v16->GetOutputPort());
    skinExtractor->SetValue(0, 500);

    // Poly Data 多边形数据
    vtkSmartPointer<vtkPolyDataNormals> skinNormals = vtkSmartPointer<vtkPolyDataNormals>::New();
    skinNormals->SetInputConnection(skinExtractor->GetOutputPort());
    skinNormals->SetFeatureAngle(60.0);

    // stripper 剥离器
    vtkSmartPointer<vtkStripper> skinStripper = vtkSmartPointer<vtkStripper>::New();
    skinStripper->SetInputConnection(skinNormals->GetOutputPort());
    //skinStripper->Update();

    vtkSmartPointer<vtkPolyDataMapper> skinMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    skinMapper->SetInputConnection(skinStripper->GetOutputPort());
    // scalar标量、梯状、分等级
    skinMapper->ScalarVisibilityOff();

    skin = vtkSmartPointer<vtkActor>::New();
    skin->SetMapper(skinMapper);
    skin->GetProperty()->SetDiffuseColor(1, .49, .25); // diffuse 弥漫  散射
    skin->GetProperty()->SetSpecular(.3);  // Specular 反射
    skin->GetProperty()->SetSpecularPower(20);
}

void WidgetsMprVtk2::SetBoneActor(vtkSmartPointer<vtkDICOMImageReader> v16)
{
    vtkSmartPointer<vtkContourFilter> boneExtractor = vtkSmartPointer<vtkContourFilter>::New();
    boneExtractor->SetInputConnection(v16->GetOutputPort());
    boneExtractor->SetValue(0, 1150);

    vtkSmartPointer<vtkPolyDataNormals> boneNormals = vtkSmartPointer<vtkPolyDataNormals>::New();
    boneNormals->SetInputConnection(boneExtractor->GetOutputPort());
    boneNormals->SetFeatureAngle(60.0);

    vtkSmartPointer<vtkStripper> boneStripper = vtkSmartPointer<vtkStripper>::New();
    boneStripper->SetInputConnection(boneNormals->GetOutputPort());

    vtkSmartPointer<vtkPolyDataMapper> boneMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    boneMapper->SetInputConnection(boneStripper->GetOutputPort());
    boneMapper->ScalarVisibilityOff();

    bone = vtkSmartPointer<vtkActor>::New();
    bone->SetMapper(boneMapper);
    bone->GetProperty()->SetDiffuseColor(1, 1, .9412);
}

void WidgetsMprVtk2::SetOutlineActor(vtkSmartPointer<vtkDICOMImageReader> v16)
{
    vtkSmartPointer<vtkOutlineFilter> outlineData = vtkSmartPointer<vtkOutlineFilter>::New();
    outlineData->SetInputConnection(v16->GetOutputPort());

    vtkSmartPointer<vtkPolyDataMapper> mapOutline = vtkSmartPointer<vtkPolyDataMapper>::New();
    mapOutline->SetInputConnection(outlineData->GetOutputPort());

    outline = vtkSmartPointer<vtkActor>::New();
    outline->SetMapper(mapOutline);
    outline->GetProperty()->SetColor(0, 0, 0);
}

void WidgetsMprVtk2::SetSagittalActor(vtkSmartPointer<vtkDICOMImageReader> v16, vtkSmartPointer<vtkLookupTable> lut)
{
    vtkSmartPointer<vtkImageMapToColors> sagittalColors = vtkSmartPointer<vtkImageMapToColors>::New();
    sagittalColors->SetInputConnection(v16->GetOutputPort());
    sagittalColors->SetLookupTable(lut);

    sagittal = vtkSmartPointer<vtkImageActor>::New();
    sagittal->GetMapper()->SetInputConnection(sagittalColors->GetOutputPort());
    int adv = m_data_extent[1] - m_data_extent[0] + 1;
    m_cur_sagitta = adv / 2;
    // 设置显示3D切层的位置
    sagittal->SetDisplayExtent(m_cur_sagitta, m_cur_sagitta, m_data_extent[2], m_data_extent[3], m_data_extent[4], m_data_extent[5]);
}

void WidgetsMprVtk2::SetAxialActor(vtkSmartPointer<vtkDICOMImageReader> v16, vtkSmartPointer<vtkLookupTable> lut)
{
    vtkSmartPointer<vtkImageMapToColors> axialColors = vtkSmartPointer<vtkImageMapToColors>::New();
    axialColors->SetInputConnection(v16->GetOutputPort());
    axialColors->SetLookupTable(lut);

    axial = vtkSmartPointer<vtkImageActor>::New();
    axial->GetMapper()->SetInputConnection(axialColors->GetOutputPort());
    int adv = m_data_extent[5] - m_data_extent[4] + 1;
    m_cur_axial = adv / 2;
    // 设置显示3D切层的位置
    axial->SetDisplayExtent(m_data_extent[0], m_data_extent[1], m_data_extent[2], m_data_extent[3], m_cur_axial, m_cur_axial);
}

void WidgetsMprVtk2::SetCoronalActor(vtkSmartPointer<vtkDICOMImageReader> v16, vtkSmartPointer<vtkLookupTable> lut)
{
    vtkSmartPointer<vtkImageMapToColors> coronalColors = vtkSmartPointer<vtkImageMapToColors>::New();
    coronalColors->SetInputConnection(v16->GetOutputPort());
    coronalColors->SetLookupTable(lut);

    coronal = vtkSmartPointer<vtkImageActor>::New();
    coronal->GetMapper()->SetInputConnection(coronalColors->GetOutputPort());
    int adv = m_data_extent[3] - m_data_extent[2] + 1;
    m_cur_cornal = adv / 2;
    // 设置显示3D切层的位置
    coronal->SetDisplayExtent(m_data_extent[0], m_data_extent[1], m_cur_cornal, m_cur_cornal, m_data_extent[4], m_data_extent[5]);
}