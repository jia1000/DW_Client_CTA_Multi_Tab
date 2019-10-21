#include "stdafx.h"

#include "VtkDialog.h"
#include "main/vtk_3d/vtk_sample/vtkView.h"
#include "main/vtk_3d/vtk_sample/vtkView3D.h"

#include "global_include.h"

//---------------------------------------------------------------------------- 
class vtkResliceCursorCallback : public vtkCommand 
{ 
public: 
	static vtkResliceCursorCallback *New() 
	{ return new vtkResliceCursorCallback; } 

	void Execute( vtkObject *caller, unsigned long /*ev*/, 
		void *callData ) 
	{ 
		/*vtkImagePlaneWidget* ipw = 
			dynamic_cast< vtkImagePlaneWidget* >( caller ); 
		if (ipw) 
		{ 
			double* wl = static_cast<double*>( callData ); 

			if ( ipw == this->IPW[0] ) 
			{ 
				this->IPW[1]->SetWindowLevel(wl[0],wl[1],1); 
				this->IPW[2]->SetWindowLevel(wl[0],wl[1],1); 
			} 
			else if( ipw == this->IPW[1] ) 
			{ 
				this->IPW[0]->SetWindowLevel(wl[0],wl[1],1); 
				this->IPW[2]->SetWindowLevel(wl[0],wl[1],1); 
			} 
			else if (ipw == this->IPW[2]) 
			{ 
				this->IPW[0]->SetWindowLevel(wl[0],wl[1],1); 
				this->IPW[1]->SetWindowLevel(wl[0],wl[1],1); 
			} 
		} */

		vtkResliceCursorWidget *rcw = dynamic_cast< 
			vtkResliceCursorWidget * >(caller); 
		if (rcw) 
		{ 
			for (int i = 0; i < 3; i++) 
			{ 
				vtkResliceCursorWidget *index_widget = view[i]->m_ResliceCursorWidget;
				if (index_widget == rcw) {
					continue;
				}
				vtkPlaneSource *ps = static_cast< vtkPlaneSource * >( 
					view[i]->GetImagePlaneWidget()->GetPolyDataAlgorithm()); 

				ps->SetOrigin(view[i]->GetResliceCursorWidget()->GetResliceCursorRepresentation()->
					GetPlaneSource()->GetOrigin());
				ps->SetPoint1(view[i]->GetResliceCursorWidget()->GetResliceCursorRepresentation()->
					GetPlaneSource()->GetPoint1());
				ps->SetPoint2(view[i]->GetResliceCursorWidget()->GetResliceCursorRepresentation()->
					GetPlaneSource()->GetPoint2());

				view[i]->GetImagePlaneWidget()->UpdatePlacement(); 
				view[i]->Render();
			} 

			view3D->Render();
		} 
		
	} 

	vtkResliceCursorCallback() {} 
	CvtkView *view[3];
	CvtkView3D *view3D;
	//CvtkView *view3D;
}; 
//////////////////////////////////////////////////////////////////////////

VtkDialog::VtkDialog(void)
{
   

}

VtkDialog::~VtkDialog(void)
{
    
	m_AxialView = NULL;
	m_SagittalView = NULL;
	m_CoronalView = NULL;
	m_3DView = NULL;
	//PostQuitMessage(0);	
}

LPCTSTR VtkDialog::GetWindowClassName() const 
{ 
	return _T("VtkDialog"); 
}

void VtkDialog::OnFinalMessage(HWND hWnd)
{
	WindowImplBase::OnFinalMessage(hWnd);
	delete this; 
}

void VtkDialog::InitWindow()
{
	
}

CControlUI* VtkDialog::CreateControl(LPCTSTR pstrClass, CPaintManagerUI *pManager)
{    
	return NULL;
}

CDuiString VtkDialog::GetSkinFile()
{
	return _T("analyze_films\\vtk_dialog.xml");
}

CDuiString VtkDialog::GetSkinFolder()
{
	return CDuiString(CPaintManagerUI::GetInstancePath()) + _T("skin\\");

}

LRESULT VtkDialog::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	//
	__super::OnSize(uMsg, wParam, lParam, bHandled);
	return 0;
}

void    VtkDialog::Notify(TNotifyUI& msg) 
{
	if (msg.sType == _T("click")) {
		LPCTSTR pszCtrlName = msg.pSender->GetName().GetData();
		if (_tcscmp(pszCtrlName, _T("btn_sysmin")) == 0) {
			SendMessage(WM_SYSCOMMAND, SC_MINIMIZE, 0);
		} else if (_tcscmp(pszCtrlName, _T("btn_sysmax")) == 0) {
			SendMessage(WM_SYSCOMMAND, SC_MAXIMIZE, 0);
			// 最大化和还原2个button之间的切换
			CControlUI* pControl = static_cast<CControlUI*>(m_pm.FindControl(_T("btn_sysmax")));
			if( pControl ) pControl->SetVisible(false);
			pControl = static_cast<CControlUI*>(m_pm.FindControl(_T("btn_restore")));
			if( pControl ) pControl->SetVisible(true);
		} else if (_tcscmp(pszCtrlName, _T("btn_restore")) == 0) {
			SendMessage(WM_SYSCOMMAND, SC_RESTORE, 0);
			// 最大化和还原2个button之间的切换
			CControlUI* pControl = static_cast<CControlUI*>(m_pm.FindControl(_T("btn_sysmax")));
			if( pControl ) pControl->SetVisible(true);
			pControl = static_cast<CControlUI*>(m_pm.FindControl(_T("btn_restore")));
			if( pControl ) pControl->SetVisible(false);
		} else if (_tcscmp(pszCtrlName, _T("btn_sysclose")) == 0) {
			Close(IDOK);
        } else if (_tcscmp(pszCtrlName, _T("btn_tool_vtk_mpr")) == 0) {
            SetXXXXXXXXXXXXXXXXX();
        }
	} else if (_tcsicmp(msg.sType, _T("selectchanged")) == 0) {
		//
	} else if (msg.sType == _T("itemclick")){       
		//
	}
}

void VtkDialog::SetXXXXXXXXXXXXXXXXX()
{
	CButtonUI* pVtkShowBtn1 = static_cast<CButtonUI*>(m_pm.FindControl(_T("Button1")));
	CButtonUI* pVtkShowBtn2 = static_cast<CButtonUI*>(m_pm.FindControl(_T("Button2")));
	CButtonUI* pVtkShowBtn3 = static_cast<CButtonUI*>(m_pm.FindControl(_T("Button3")));
	CButtonUI* pVtkShowBtn4 = static_cast<CButtonUI*>(m_pm.FindControl(_T("Button4")));

	m_AxialView		= new CvtkView(this->m_hWnd, pVtkShowBtn1);
	m_CoronalView	= new CvtkView(this->m_hWnd, pVtkShowBtn2);
	m_SagittalView	= new CvtkView(this->m_hWnd, pVtkShowBtn3);
	m_3DView		= new CvtkView3D(this->m_hWnd, pVtkShowBtn4);
	

	m_AxialView->SetSliceDirection(2);
	m_AxialView->GetImagePlaneWidget()->SetInteractor(m_3DView->GetInteractor());
	m_AxialView->GetImagePlaneWidget()->SetDefaultRenderer(m_3DView->GetRenderer());

	m_CoronalView->SetSliceDirection(1);
	m_CoronalView->GetImagePlaneWidget()->SetInteractor(m_3DView->GetInteractor());
	m_CoronalView->GetImagePlaneWidget()->SetDefaultRenderer(m_3DView->GetRenderer());
	m_CoronalView->SetResliceCursor(m_AxialView->GetResliceCursor());

	m_SagittalView->SetSliceDirection(0);
	m_SagittalView->GetImagePlaneWidget()->SetInteractor(m_3DView->GetInteractor());
	m_SagittalView->GetImagePlaneWidget()->SetDefaultRenderer(m_3DView->GetRenderer());
	m_SagittalView->SetResliceCursor(m_AxialView->GetResliceCursor());

	
	// Read file
	vtkSmartPointer<vtkDICOMImageReader> m_v16 = vtkSmartPointer<vtkDICOMImageReader>::New();
	m_v16->SetDirectoryName("..\\bin\\release\\Skin\\data\\slices1");
	m_v16->Update();
	m_CoronalView->SetImageData(m_v16->GetOutput());
	m_SagittalView->SetImageData(m_v16->GetOutput());
	m_AxialView->SetImageData(m_v16->GetOutput());

	vtkSmartPointer<vtkResliceCursorCallback> cbk = 
		vtkSmartPointer<vtkResliceCursorCallback>::New(); 
	cbk->view[0] = m_AxialView;
	m_AxialView->GetResliceCursorWidget()->AddObserver( 
		vtkResliceCursorWidget::ResliceAxesChangedEvent, cbk ); 

	cbk->view[1] = m_SagittalView;
	m_SagittalView->GetResliceCursorWidget()->AddObserver( 
		vtkResliceCursorWidget::ResliceAxesChangedEvent, cbk );

	cbk->view[2] = m_CoronalView;
	m_CoronalView->GetResliceCursorWidget()->AddObserver( 
		vtkResliceCursorWidget::ResliceAxesChangedEvent, cbk );

	cbk->view3D = m_3DView;

	m_3DView->Render();
}
LRESULT VtkDialog::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT lRes = 0;

	BOOL bHandled = TRUE;
	switch (uMsg)
	{
	case WM_SIZE:
		OnSize(uMsg, wParam, lParam, bHandled);
		break;
	}

	if( m_pm.MessageHandler(uMsg, wParam, lParam, lRes) ) {
		return lRes;
	}

	return __super::HandleMessage(uMsg, wParam, lParam);
}