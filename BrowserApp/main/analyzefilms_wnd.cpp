#include "stdafx.h"

#include "analyzefilms_wnd.h"
#include "vtk_dicom_demo/DicomView.h"
#include "MultiSlicesImageDemoSameSource.h"
#include "main/vtk_3d/widgets_mpr_vtk2.h"

#include <thread>

AnalyzeFilms_Wnd::AnalyzeFilms_Wnd(void)
{
    mpr_vtk2 = NULL;

}

AnalyzeFilms_Wnd::~AnalyzeFilms_Wnd(void)
{
    if (mpr_vtk2) {
        delete mpr_vtk2;
        mpr_vtk2 = NULL;
    }

	//PostQuitMessage(0);	
}

LPCTSTR AnalyzeFilms_Wnd::GetWindowClassName() const 
{ 
	return _T("AnalyzeFilms_Wnd"); 
}

void AnalyzeFilms_Wnd::OnFinalMessage(HWND hWnd)
{
	WindowImplBase::OnFinalMessage(hWnd);
	delete this; 
}

void AnalyzeFilms_Wnd::InitWindow()
{
	
}

CControlUI* AnalyzeFilms_Wnd::CreateControl(LPCTSTR pstrClass, CPaintManagerUI *pManager)
{    
	return NULL;
}

CDuiString AnalyzeFilms_Wnd::GetSkinFile()
{
	return _T("analyze_films\\analyze_films.xml");
}

CDuiString AnalyzeFilms_Wnd::GetSkinFolder()
{
	return CDuiString(CPaintManagerUI::GetInstancePath()) + _T("skin\\");

}

LRESULT AnalyzeFilms_Wnd::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	//
	__super::OnSize(uMsg, wParam, lParam, bHandled);
	return 0;
}

void    AnalyzeFilms_Wnd::Notify(TNotifyUI& msg) 
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
            std::vector<CButtonUI*> controls;
            CButtonUI* pVtkShowBtn1 = static_cast<CButtonUI*>(m_pm.FindControl(_T("Button1")));
            CButtonUI* pVtkShowBtn2 = static_cast<CButtonUI*>(m_pm.FindControl(_T("Button2")));
            CButtonUI* pVtkShowBtn3 = static_cast<CButtonUI*>(m_pm.FindControl(_T("Button3")));
            CButtonUI* pVtkShowBtn4 = static_cast<CButtonUI*>(m_pm.FindControl(_T("Button4")));

            controls.push_back(pVtkShowBtn1);
            controls.push_back(pVtkShowBtn2);
            controls.push_back(pVtkShowBtn3);
            controls.push_back(pVtkShowBtn4);

            if (!mpr_vtk2) {
                mpr_vtk2 = new WidgetsMprVtk2(this->m_hWnd, controls);
                mpr_vtk2->ShowWidgets_Test();
            }
        }
	} else if (_tcsicmp(msg.sType, _T("selectchanged")) == 0) {
		//
	} else if (msg.sType == _T("itemclick")){       
		//
	}
}

LRESULT AnalyzeFilms_Wnd::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
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