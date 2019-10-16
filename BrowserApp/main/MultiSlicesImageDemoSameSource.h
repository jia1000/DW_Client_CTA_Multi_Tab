#pragma once
#include "WidgetsDemoBase.h"

//////////////////////////////////////////////////////////////////////////
class myInteractorStyle :public vtkInteractorStyleTrackballCamera
{
public:

	static myInteractorStyle* New();

	void    SetLeftRender(vtkRenderer *render);

	void    SetRightRender(vtkRenderer *render);

	void    OnMouseMove();

	void OnLeftButtonDown();

	void OnLeftButtonUp();

	void OnMiddleButtonDown();

	void OnMiddleButtonUp();

	void OnRightButtonDown();

	void OnRightButtonUp();

	void OnMouseWheelForward();

	void OnMouseWheelBackward();

	void Rotate();

	void Spin();

	void Pan();

	void Dolly();

private:
	void    CameraSynchronous();

private:
	vtkRenderer     *m_leftRender;
	vtkRenderer     *m_rightRender;

};
//////////////////////////////////////////////////////////////////////////

class MultiSlicesImageDemoSameSource :
    public CWidgetsDemoBase
{
public:
    MultiSlicesImageDemoSameSource(HWND parent, CButtonUI* pVtkShowBtn, bool is_multi_actor);
    ~MultiSlicesImageDemoSameSource(void);

	virtual void CreateRendererAndRenderWindowAndInteractor();

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

private:
	vtkSmartPointer<vtkRenderer>                m_renderer2;
	vtkSmartPointer<vtkRenderer>                m_renderer3;
	vtkSmartPointer<vtkRenderer>                m_renderer4;

	vtkSmartPointer<vtkTextWidget> textWidget;
	vtkSmartPointer<vtkDICOMImageReader> m_v16;

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
};

