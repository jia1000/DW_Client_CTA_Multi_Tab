#pragma once
#include "global_include.h"

class CrossViewVtkActor;

class WidgetsMprVtk
{
public:
    WidgetsMprVtk(HWND parent, CButtonUI* pVtkShowBtn);
    ~WidgetsMprVtk(void);

    void ShowWidgets_Test();

	virtual void CreateRendererAndRenderWindowAndInteractor();
    virtual void StartWidgetsRender();

	void ResizeAndPosition();

    void SetSagittalActorNormal(vtkSmartPointer<vtkDICOMImageReader> v16);
    void SetAxialActorNormal(vtkSmartPointer<vtkDICOMImageReader> v16);
    void SetCoronalActorNormal(vtkSmartPointer<vtkDICOMImageReader> v16);


    void SetSkinActor(vtkSmartPointer<vtkDICOMImageReader> v16);
    void SetBoneActor(vtkSmartPointer<vtkDICOMImageReader> v16);
    void SetOutlineActor(vtkSmartPointer<vtkDICOMImageReader> v16);

    void SetSagittalActor(vtkSmartPointer<vtkDICOMImageReader> v16, vtkSmartPointer<vtkLookupTable> lut);
    void SetAxialActor(vtkSmartPointer<vtkDICOMImageReader> v16, vtkSmartPointer<vtkLookupTable> lut);
    void SetCoronalActor(vtkSmartPointer<vtkDICOMImageReader> v16, vtkSmartPointer<vtkLookupTable> lut);

protected:
    vtkSmartPointer<vtkRenderWindow>            m_renderWindow;
    
    vtkSmartPointer<vtkRenderer>                m_renderer;
    vtkSmartPointer<vtkRenderer>                m_renderer2;
    vtkSmartPointer<vtkRenderer>                m_renderer3;
    vtkSmartPointer<vtkRenderer>                m_renderer4;

    vtkSmartPointer<vtkRenderWindowInteractor>  m_interactor;

    vtkSmartPointer<vtkDICOMImageReader> m_v16;
    
    vtkSmartPointer<vtkTextWidget> textWidget;

    vtkSmartPointer<vtkLookupTable> m_bwLut;
    vtkSmartPointer<vtkLookupTable> m_hueLut;
    vtkSmartPointer<vtkLookupTable> m_satLut;
public:
    vtkSmartPointer<CrossViewVtkActor> sagittal_normal;
    vtkSmartPointer<CrossViewVtkActor> axial_normal;
    vtkSmartPointer<CrossViewVtkActor> coronal_normal;
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
	CButtonUI* m_pVtkShowBtn;
};