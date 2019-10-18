#pragma once
#include "global_include.h"

class CrossViewVtkActorBase;
class CrossViewVtkActorSagittal;
class CrossViewVtkActorAxial;
class CrossViewVtkActorCoronal;

class WidgetsMprVtk2
{
public:
    WidgetsMprVtk2(HWND parent, std::vector<CButtonUI*> vVtkShowBtns);
    ~WidgetsMprVtk2(void);

    void ShowWidgets_Test();

	virtual void CreateRendererAndRenderWindowAndInteractor();
    virtual void StartWidgetsRender();

	void ResizeAndPosition();

    void SetAxialRendererNormal(vtkSmartPointer<vtkDICOMImageReader> v16);
    void SetCoronalRendererNormal(vtkSmartPointer<vtkDICOMImageReader> v16);
    void SetSagittalRendererNormal(vtkSmartPointer<vtkDICOMImageReader> v16);
    void SetMprRendererNormal(vtkSmartPointer<vtkDICOMImageReader> v16);

            
    void SetSkinActor(vtkSmartPointer<vtkDICOMImageReader> v16);
    void SetBoneActor(vtkSmartPointer<vtkDICOMImageReader> v16);
    void SetOutlineActor(vtkSmartPointer<vtkDICOMImageReader> v16);

    void SetSagittalActor(vtkSmartPointer<vtkDICOMImageReader> v16, vtkSmartPointer<vtkLookupTable> lut);
    void SetAxialActor(vtkSmartPointer<vtkDICOMImageReader> v16, vtkSmartPointer<vtkLookupTable> lut);
    void SetCoronalActor(vtkSmartPointer<vtkDICOMImageReader> v16, vtkSmartPointer<vtkLookupTable> lut);

public:
    std::vector<vtkSmartPointer<vtkRenderWindow>>     m_renderWindows;
        
protected:
    vtkSmartPointer<vtkRenderWindowInteractor>  m_interactor1;
    vtkSmartPointer<vtkRenderWindowInteractor>  m_interactor2;
    vtkSmartPointer<vtkRenderWindowInteractor>  m_interactor3;
    vtkSmartPointer<vtkRenderWindowInteractor>  m_interactor4;


    vtkSmartPointer<vtkDICOMImageReader> m_v16;
    
    vtkSmartPointer<vtkTextWidget> textWidget;

    vtkSmartPointer<vtkLookupTable> m_bwLut;
    vtkSmartPointer<vtkLookupTable> m_hueLut;
    vtkSmartPointer<vtkLookupTable> m_satLut;
public:
    vtkSmartPointer<CrossViewVtkActorSagittal> sagittal_normal;
    vtkSmartPointer<CrossViewVtkActorAxial> axial_normal;
    vtkSmartPointer<CrossViewVtkActorCoronal> coronal_normal;
protected:
    vtkSmartPointer<vtkActor> skin;
    vtkSmartPointer<vtkActor> bone;
    vtkSmartPointer<vtkActor> outline;

    vtkSmartPointer<vtkImageActor> sagittal;
    vtkSmartPointer<vtkImageActor> axial;
    vtkSmartPointer<vtkImageActor> coronal;

    int m_data_extent[6];
    int m_cur_sagitta_normal;
    int m_cur_cornal_normal;
    int m_cur_axial_normal;

    int m_cur_sagitta;
    int m_cur_cornal;
    int m_cur_axial;

    double m_cur_hue_coronal_min;
    double m_cur_hue_coronal_max;

	HWND m_parentWnd;
	RECT m_rc;
	//CButtonUI* m_pVtkShowBtn;

    std::vector<CButtonUI*> m_vVtkShowBtns;
};