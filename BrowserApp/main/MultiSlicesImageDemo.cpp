#include "stdafx.h"
#include "MultiSlicesImageDemo.h"

// 创建一个序列的三维重建
MultiSlicesImageDemo::MultiSlicesImageDemo(HWND parent, CButtonUI* pVtkShowBtn, bool is_multi_actor) : CWidgetsDemoBase(parent, pVtkShowBtn)
{
	m_is_show_Sagitta	= true;
	m_is_show_coronal	= true;
	m_is_show_axial		= true;
	m_is_show_bone		= true;
	m_is_show_skin		= true;
	m_is_show_outline	= true;

	m_use_multi_actor   = is_multi_actor;
}


MultiSlicesImageDemo::~MultiSlicesImageDemo(void)
{
	//m_interactor->
	m_renderWindow->RemoveRenderer(m_renderer);
}

void MultiSlicesImageDemo::ShowWidgets_Move_Sagitta()
{
	if (m_cur_sagitta >= m_data_extent[1]) {
		m_cur_sagitta = 0;
	}
	m_cur_sagitta++;
	// 设置显示3D切层的位置
	sagittal->SetDisplayExtent(m_cur_sagitta, m_cur_sagitta, m_data_extent[2],m_data_extent[3], m_data_extent[4],m_data_extent[5]);
	m_renderWindow->Render();

}
void MultiSlicesImageDemo::ShowWidgets_Move_Cornal()
{
	if (m_cur_cornal >= m_data_extent[3]) {
		m_cur_cornal = 0;
	}
	m_cur_cornal++;
	// 设置显示3D切层的位置
	coronal->SetDisplayExtent(m_data_extent[0],m_data_extent[1], m_cur_cornal, m_cur_cornal, m_data_extent[4],m_data_extent[5]);
	m_renderWindow->Render();

}
void MultiSlicesImageDemo::ShowWidgets_Move_Axial()
{
	if (m_cur_axial >= m_data_extent[5]) {
		m_cur_axial = 0;
	}
	m_cur_axial++;

	// 设置显示3D切层的位置
	axial->SetDisplayExtent(m_data_extent[0],m_data_extent[1], m_data_extent[2],m_data_extent[3], m_cur_axial, m_cur_axial);

	m_renderWindow->Render();
}
void MultiSlicesImageDemo::ShowWidgets_Hue_coronal()
{
	m_cur_hue_coronal_min += 0.1;
	m_cur_hue_coronal_max += 0.1;
	if (m_cur_hue_coronal_min >= 1.0) {
		m_cur_hue_coronal_min = 0.0;
	}
	if (m_cur_hue_coronal_max >= 1.0) {
		m_cur_hue_coronal_max = 0.0;
	}
	m_satLut->SetHueRange (m_cur_hue_coronal_min, m_cur_hue_coronal_max);		// 设置色度/色调范围
	m_renderWindow->Render();
}
void MultiSlicesImageDemo::ShowWidgets_show_Hide_Sagitta()
{
	m_is_show_Sagitta = !m_is_show_Sagitta;
	if (m_is_show_Sagitta) {
		sagittal->VisibilityOn();
	} 
	else {
		sagittal->VisibilityOff();
	}
	m_renderWindow->Render();
}
void MultiSlicesImageDemo::ShowWidgets_show_Hide_coronal()
{
	m_is_show_coronal = !m_is_show_coronal;
	if (m_is_show_coronal) {
		coronal->VisibilityOn();
	} 
	else {
		coronal->VisibilityOff();
	}
	m_renderWindow->Render();
}
void MultiSlicesImageDemo::ShowWidgets_show_Hide_Axial()
{
	m_is_show_axial = !m_is_show_axial;
	if (m_is_show_axial) {
		axial->VisibilityOn();
	} 
	else {
		axial->VisibilityOff();
	}
	m_renderWindow->Render();
}
void MultiSlicesImageDemo::ShowWidgets_show_Hide_bone()
{
	m_is_show_bone = !m_is_show_bone;
	if (m_is_show_bone) {
		bone->VisibilityOn();
	} 
	else {
		bone->VisibilityOff();
	}
	m_renderWindow->Render();
}
void MultiSlicesImageDemo::ShowWidgets_show_Hide_skin()
{
	m_is_show_skin = !m_is_show_skin;
	if (m_is_show_skin) {
		skin->VisibilityOn();
	} 
	else {
		skin->VisibilityOff();
	}
	m_renderWindow->Render();
}
void MultiSlicesImageDemo::ShowWidgets_show_Hide_outline()
{
	m_is_show_outline = !m_is_show_outline;
	if (m_is_show_outline) {
		outline->VisibilityOn();
	} 
	else {
		outline->VisibilityOff();
	}
	m_renderWindow->Render();

}
void MultiSlicesImageDemo::ShowWidgets_Mip_Cornal()
{
	if (m_cur_cornal >= m_data_extent[3]) {
		m_cur_cornal = 0;
	}
	m_cur_cornal++;
	// 设置显示3D切层的位置
	int coronal_mip_max = m_cur_cornal + 5;
	if (coronal_mip_max >= m_data_extent[3]) {
		coronal_mip_max = m_data_extent[3];
	}
	// 打算显示多层，但是这个办法不行。
	coronal->SetDisplayExtent(m_data_extent[0],m_data_extent[1], m_cur_cornal, coronal_mip_max, m_data_extent[4],m_data_extent[5]);
	m_renderWindow->Render();
}
#define TEST_CTA_RAW  1  // 用于测CTA项目重建崩溃时，验证vtk的PloyData的正确性

void MultiSlicesImageDemo::StartWidgetsRender()
{
	//m_v16 = vtkSmartPointer<vtkMetaImageReader>::New();
	//m_v16->SetDataDimensions(64,64);
	//m_v16->SetImageRange(1, 93);
	//m_v16->SetDataByteOrderToLittleEndian();
	//m_v16->SetFilePrefix ("F:\\dev_study\\duilib_study\\Duilib_Demo\\Duilib_Demo\\data\\FullHead.mhd");
	//m_v16->SetDataSpacing (3.2, 3.2, 1.5);
	//m_v16->Update();

	//m_v16 = vtkSmartPointer<vtkMetaImageReader>::New();
	//
	//if (m_use_multi_actor) {
	//	m_v16->SetFileName("F:\\dev_study\\duilib_study\\Duilib_Demo\\Duilib_Demo\\data\\FullHead.mhd");
	//} else {
	//	m_v16->SetFileName("F:\\dev_study\\duilib_study\\Duilib_Demo\\Duilib_Demo\\data\\FullHead.mhd");
	//}
	//m_v16->Update();
			
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
	if (m_use_multi_actor) {
		SetBoneActor(m_v16);
		SetOutlineActor(m_v16);
		// 这是3个轴面的slice显示
		SetSagittalActor(m_v16, m_bwLut);
		SetAxialActor(m_v16, m_hueLut);
		SetCoronalActor(m_v16, m_satLut);
	} else {
	}

	vtkSmartPointer<vtkCamera> aCamera =
		vtkSmartPointer<vtkCamera>::New();
	aCamera->SetViewUp (0, 0, -1);
	aCamera->SetPosition (0, 1, 0);
	aCamera->SetFocalPoint (0, 0, 0);
	aCamera->ComputeViewPlaneNormal();
	aCamera->Azimuth(30.0);
	aCamera->Elevation(30.0);

	if (m_use_multi_actor) {
		m_renderer->AddActor(outline);
		m_renderer->AddActor(sagittal);
		m_renderer->AddActor(axial);
		m_renderer->AddActor(coronal);	
		m_renderer->AddActor(bone);
	} else {
	}

	
	m_renderer->AddActor(skin);
	// Turn off bone for this example.
	//bone->VisibilityOff();

	// Set skin to semi-transparent.
	skin->GetProperty()->SetOpacity(0.5);

	// An initial camera view is created.  The Dolly() method moves
	// the camera towards the FocalPoint, thereby enlarging the image.
	m_renderer->SetActiveCamera(aCamera);

	//////////////////////////////////////////////////////////////////////////
    //renderWindow->SetSize(600, 600);
	ResizeAndPosition();

	m_renderer->ResetCamera ();
	aCamera->Dolly(1.5);

	m_renderer->ResetCameraClippingRange ();

    m_interactor->Initialize();
    m_renderWindow->Render();
    m_interactor->Start();
}

void MultiSlicesImageDemo::SetMyInteractorStyle(vtkSmartPointer<vtkRenderWindowInteractor> interactor)
{
    vtkSmartPointer<vtkInteractorStyleTrackballCamera> style = vtkSmartPointer<vtkInteractorStyleTrackballCamera>::New();
    interactor->SetInteractorStyle( style );
}

void MultiSlicesImageDemo::CreatePipeline(vtkSmartPointer<vtkRenderer> renderer)
{
    
}

void MultiSlicesImageDemo::AddMyActor(vtkSmartPointer<vtkRenderWindowInteractor> interactor)
{

}

void MultiSlicesImageDemo::SetSkinActor(vtkSmartPointer<vtkDICOMImageReader> v16)
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

void MultiSlicesImageDemo::SetBoneActor(vtkSmartPointer<vtkDICOMImageReader> v16)
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

void MultiSlicesImageDemo::SetOutlineActor(vtkSmartPointer<vtkDICOMImageReader> v16)
{
	vtkSmartPointer<vtkOutlineFilter> outlineData =
		vtkSmartPointer<vtkOutlineFilter>::New();
	outlineData->SetInputConnection(v16->GetOutputPort());
	outlineData->Update();

	vtkSmartPointer<vtkPolyDataMapper> mapOutline =
		vtkSmartPointer<vtkPolyDataMapper>::New();
	mapOutline->SetInputConnection(outlineData->GetOutputPort());

	outline = vtkSmartPointer<vtkActor>::New();
	outline->SetMapper(mapOutline);
	outline->GetProperty()->SetColor(0,0,0);
}

void MultiSlicesImageDemo::SetSagittalActor(vtkSmartPointer<vtkDICOMImageReader> v16, vtkSmartPointer<vtkLookupTable> lut)
{
	vtkSmartPointer<vtkImageMapToColors> sagittalColors = vtkSmartPointer<vtkImageMapToColors>::New();
	sagittalColors->SetInputConnection(v16->GetOutputPort());
	sagittalColors->SetLookupTable(lut);
	sagittalColors->Update();

	sagittal = vtkSmartPointer<vtkImageActor>::New();
	sagittal->GetMapper()->SetInputConnection(sagittalColors->GetOutputPort());
	//sagittal->SetDisplayExtent(32,32, 0,63, 0,92);
	int adv = m_data_extent[1] - m_data_extent[0] + 1;
	m_cur_sagitta = adv / 2;
	// 设置显示3D切层的位置
	sagittal->SetDisplayExtent(m_cur_sagitta, m_cur_sagitta, m_data_extent[2],m_data_extent[3], m_data_extent[4],m_data_extent[5]);

}

void MultiSlicesImageDemo::SetAxialActor(vtkSmartPointer<vtkDICOMImageReader> v16, vtkSmartPointer<vtkLookupTable> lut)
{
	vtkSmartPointer<vtkImageMapToColors> axialColors = vtkSmartPointer<vtkImageMapToColors>::New();
	axialColors->SetInputConnection(v16->GetOutputPort());
	axialColors->SetLookupTable(lut);
	axialColors->Update();

	axial =	vtkSmartPointer<vtkImageActor>::New();
	axial->GetMapper()->SetInputConnection(axialColors->GetOutputPort());
	//axial->SetDisplayExtent(0,63, 0,63, 46,46);
	int adv = m_data_extent[5] - m_data_extent[4] + 1;
	m_cur_axial = adv / 2;
	// 设置显示3D切层的位置
	axial->SetDisplayExtent(m_data_extent[0],m_data_extent[1], m_data_extent[2],m_data_extent[3], m_cur_axial, m_cur_axial);
	
}

void MultiSlicesImageDemo::SetCoronalActor(vtkSmartPointer<vtkDICOMImageReader> v16, vtkSmartPointer<vtkLookupTable> lut)
{
	vtkSmartPointer<vtkImageMapToColors> coronalColors =	vtkSmartPointer<vtkImageMapToColors>::New();
	coronalColors->SetInputConnection(v16->GetOutputPort());
	coronalColors->SetLookupTable(lut);
	coronalColors->Update();

	coronal = vtkSmartPointer<vtkImageActor>::New();
	coronal->GetMapper()->SetInputConnection(coronalColors->GetOutputPort());
	//coronal->SetDisplayExtent(0,63, 32,32, 0,92);
	int adv = m_data_extent[3] - m_data_extent[2] + 1;
	m_cur_cornal = adv / 2;
	// 设置显示3D切层的位置
	coronal->SetDisplayExtent(m_data_extent[0],m_data_extent[1], m_cur_cornal, m_cur_cornal, m_data_extent[4],m_data_extent[5]);
}