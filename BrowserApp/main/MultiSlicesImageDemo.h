#pragma once
#include "WidgetsDemoBase.h"

class MultiSlicesImageDemo :
    public CWidgetsDemoBase
{
public:
    MultiSlicesImageDemo(HWND parent, CButtonUI* pVtkShowBtn, bool is_multi_actor);
    ~MultiSlicesImageDemo(void);

    void SetMyInteractorStyle(vtkSmartPointer<vtkRenderWindowInteractor> interactor);

    virtual void StartWidgetsRender();
    virtual void CreatePipeline(vtkSmartPointer<vtkRenderer> renderer);
    virtual void AddMyActor(vtkSmartPointer<vtkRenderWindowInteractor> interactor);

	void SetSkinActor(vtkSmartPointer<vtkDICOMImageReader> v16);
	void SetBoneActor(vtkSmartPointer<vtkDICOMImageReader> v16);
	void SetOutlineActor(vtkSmartPointer<vtkDICOMImageReader> v16);

	void SetSagittalActor(vtkSmartPointer<vtkDICOMImageReader> v16, vtkSmartPointer<vtkLookupTable> lut);
	void SetAxialActor(vtkSmartPointer<vtkDICOMImageReader> v16, vtkSmartPointer<vtkLookupTable> lut);
	void SetCoronalActor(vtkSmartPointer<vtkDICOMImageReader> v16, vtkSmartPointer<vtkLookupTable> lut);

	void ShowWidgets_Move_Sagitta();
	void ShowWidgets_Move_Cornal();
	void ShowWidgets_Move_Axial();
	void ShowWidgets_Hue_coronal();
	void ShowWidgets_show_Hide_Sagitta();
	void ShowWidgets_show_Hide_coronal();
	void ShowWidgets_show_Hide_Axial();
	void ShowWidgets_show_Hide_bone();
	void ShowWidgets_show_Hide_skin();
	void ShowWidgets_show_Hide_outline();
	void ShowWidgets_Mip_Cornal();
private:
    vtkSmartPointer<vtkTextWidget> textWidget;

	vtkSmartPointer<vtkDICOMImageReader> m_v16;

	//vtkSmartPointer<vtkMetaImageReader> m_v16;
	vtkSmartPointer<vtkLookupTable> m_bwLut;
	vtkSmartPointer<vtkLookupTable> m_hueLut;
	vtkSmartPointer<vtkLookupTable> m_satLut;

	vtkSmartPointer<vtkActor> skin;
	vtkSmartPointer<vtkActor> bone;
	vtkSmartPointer<vtkActor> outline;

	vtkSmartPointer<vtkImageActor> sagittal;
	vtkSmartPointer<vtkImageActor> axial;
	vtkSmartPointer<vtkImageActor> coronal;

	int m_data_extent[6];
	int m_cur_sagitta;
	int m_cur_cornal;
	int m_cur_axial;

	double m_cur_hue_coronal_min;
	double m_cur_hue_coronal_max;

	bool m_is_show_Sagitta;
	bool m_is_show_coronal;
	bool m_is_show_axial;
	bool m_is_show_bone;
	bool m_is_show_skin;
	bool m_is_show_outline;

	bool m_use_multi_actor;// 目前false单actor  true多actor
};

