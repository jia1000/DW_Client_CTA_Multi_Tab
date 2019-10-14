#pragma once
#include "global_include.h"

class CWidgetsDemoBase
{
public:
    CWidgetsDemoBase(HWND parent, CButtonUI* pVtkShowBtn);
    ~CWidgetsDemoBase(void);

    void ShowWidgets_Test();

	virtual void CreateRendererAndRenderWindowAndInteractor();
    virtual vtkSmartPointer<vtkRenderer> CreateRenderer();
    virtual vtkSmartPointer<vtkRenderWindow> CreateRenderWindow(vtkSmartPointer<vtkRenderer> renderer);
    virtual vtkSmartPointer<vtkRenderWindowInteractor> CreateInteractor(vtkSmartPointer<vtkRenderWindow> renderWindow);
    virtual void SetMyInteractorStyle(vtkSmartPointer<vtkRenderWindowInteractor> interactor) { }
    virtual void CreatePipeline(vtkSmartPointer<vtkRenderer> renderer) { }
    virtual void AddMyActor(vtkSmartPointer<vtkRenderWindowInteractor> interactor) { }
    virtual void StartWidgetsRender() { }

	void ResizeAndPosition();

protected:
    vtkSmartPointer<vtkRenderer>                m_renderer;
    vtkSmartPointer<vtkRenderWindow>            m_renderWindow;
    vtkSmartPointer<vtkRenderWindowInteractor>  m_interactor;

	HWND m_parentWnd;
	RECT m_rc;
	CButtonUI* m_pVtkShowBtn;
};

