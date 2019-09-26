#include "stdafx.h"

#include "EntryFrameWnd.h"
#include "MultiSlicesImageDemo.h"

#include <thread>

CEntryFrameWnd::CEntryFrameWnd(void)
{
}

CEntryFrameWnd::~CEntryFrameWnd(void)
{
	PostQuitMessage(0);
}

LPCTSTR CEntryFrameWnd::GetWindowClassName() const 
{ 
	return _T("CefBrowserFrameWnd"); 
}

void CEntryFrameWnd::OnFinalMessage(HWND hWnd)
{
	WindowImplBase::OnFinalMessage(hWnd);
	//delete this; 
}

void CEntryFrameWnd::InitWindow()
{
	CButtonUI* pVtkShowBtn1 = static_cast<CButtonUI*>(m_pm.FindControl(_T("Button3")));
	if (pVtkShowBtn1) {
		pVtkShowBtn1->SetText(_T("Please wait..."));
	}
	MultiSlicesImageDemo* box_widgets1 = new MultiSlicesImageDemo(this->m_hWnd, pVtkShowBtn1, false);
	std::thread th1(&MultiSlicesImageDemo::ShowWidgets_Test, box_widgets1);
	th1.detach();

	CButtonUI* pVtkShowBtn2 = static_cast<CButtonUI*>(m_pm.FindControl(_T("Button4")));
	if (pVtkShowBtn2) {
		pVtkShowBtn2->SetText(_T("Please wait..."));
	}
	MultiSlicesImageDemo* box_widgets2 = new MultiSlicesImageDemo(this->m_hWnd, pVtkShowBtn2, true);			
	std::thread th2(&MultiSlicesImageDemo::ShowWidgets_Test, box_widgets2);
	th2.detach();
}

CControlUI* CEntryFrameWnd::CreateControl(LPCTSTR pstrClass, CPaintManagerUI *pManager)
{
	return NULL;
}

CDuiString CEntryFrameWnd::GetSkinFile()
{
	return _T("frame_wnd_demo\\entry_frame.xml");
}

CDuiString CEntryFrameWnd::GetSkinFolder()
{
	return CDuiString(CPaintManagerUI::GetInstancePath()) + _T("skin\\");

}

LRESULT CEntryFrameWnd::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	//
	__super::OnSize(uMsg, wParam, lParam, bHandled);
	return 0;
}

void    CEntryFrameWnd::Notify(TNotifyUI& msg) 
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
		} else if (_tcscmp(pszCtrlName, _T("Button1")) == 0) {
			
		} else if (_tcscmp(pszCtrlName, _T("Button2")) == 0) {
			
		} else if (_tcscmp(pszCtrlName, _T("Button3")) == 0) {
			//CButtonUI* pVtkShowBtn = static_cast<CButtonUI*>(m_pm.FindControl(_T("Button3")));
			//if (pVtkShowBtn) {
			//	pVtkShowBtn->SetText(_T("Please wait..."));
			//}
			//MultiSlicesImageDemo* box_widgets = new MultiSlicesImageDemo(this->m_hWnd, pVtkShowBtn, false);
			//std::thread th(&MultiSlicesImageDemo::ShowWidgets_Test, box_widgets);
			//th.detach();			
		} else if (_tcscmp(pszCtrlName, _T("Button4")) == 0) {
			//CButtonUI* pVtkShowBtn = static_cast<CButtonUI*>(m_pm.FindControl(_T("Button4")));
			//if (pVtkShowBtn) {
			//	pVtkShowBtn->SetText(_T("Please wait..."));
			//}
			//MultiSlicesImageDemo* box_widgets = new MultiSlicesImageDemo(this->m_hWnd, pVtkShowBtn, true);			
			//std::thread th(&MultiSlicesImageDemo::ShowWidgets_Test, box_widgets);
			//th.detach();
		} 
	} else if (_tcsicmp(msg.sType, _T("selectchanged")) == 0) {
		//
	}
}

LRESULT CEntryFrameWnd::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
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