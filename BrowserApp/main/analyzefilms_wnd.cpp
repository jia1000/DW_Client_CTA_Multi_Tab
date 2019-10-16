#include "stdafx.h"

#include "analyzefilms_wnd.h"
#include "vtk_dicom_demo/DicomView.h"
#include "MultiSlicesImageDemoSameSource.h"


#include <thread>

AnalyzeFilms_Wnd::AnalyzeFilms_Wnd(void)
{
	
}

AnalyzeFilms_Wnd::~AnalyzeFilms_Wnd(void)
{
	
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