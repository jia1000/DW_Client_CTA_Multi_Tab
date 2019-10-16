#include "stdafx.h"

#include "WndSimpleFrame.h"
#include "EntryFrameWnd.h"
#include "EntryFrameWnd2.h"
#include "analyzefilms_wnd.h"
#include "BrowserWndManager.h"

//DUI_BEGIN_MESSAGE_MAP(CWndSimpleFrame, WindowImplBase)
//	DUI_ON_MSGTYPE(DUI_MSGTYPE_CLICK,OnClick)
//DUI_END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

CWndSimpleFrame::CWndSimpleFrame(const wstring& url) 
	:CWndFrameBase() 
{
	m_pBrowserUI = new CCefBrowserUI(this, L"http://127.0.0.1:8080/cta_only.html"); 
	//m_pBrowserUI2 = new CCefBrowserUI(this, L"http://127.0.0.1:8080/cta_multi.html"); 
	// 和web端联调时，测试网址  
	//m_pBrowserUI2 = new CCefBrowserUI(this, L"http://172.16.11.134:8080/home"); // huyong pc
	m_pBrowserUI2 = new CCefBrowserUI(this, L"http://172.16.100.29:8080/home"); 
	//m_pBrowserUI2 = new CCefBrowserUI(this, L"http://172.16.30.167:8080/home"); // duandongyi pc

}


void CWndSimpleFrame::OnFinalMessage(HWND /*hWnd*/)
{
	NotifyWindowDestroyed();

	delete this;
}

void CWndSimpleFrame::OnExit()
{
	//if(MessageBox(this->GetHWND(), L"是否立即退出？",L"退出",MB_ICONEXCLAMATION|MB_OKCANCEL)==IDCANCEL)
	//	return;

	Close(IDOK);
}

CDuiString CWndSimpleFrame::GetSkinFile()
{
	return _T("WndSimpleFrame.xml");
}

LPCTSTR CWndSimpleFrame::GetWindowClassName(void) const 
{ 
	return L"{A5F70D30-7F37-4567-A829-7F1F9E4F659E}"; 
}

LRESULT CWndSimpleFrame::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	return __super::HandleMessage(uMsg,wParam,lParam);
}

void CWndSimpleFrame::Notify(TNotifyUI& msg)
{
	if (_tcsicmp(msg.sType, _T("selectchanged")) == 0) {
		OnSelChanged(msg.pSender);
	}
	__super::Notify(msg);
}
void CWndSimpleFrame::OnSelChanged(CControlUI* pSender)
{
	CTabLayoutUI* pTabTest = static_cast <CTabLayoutUI*>(m_pm.FindControl(_T("tabTest")));
	if (pTabTest == NULL)
	{
		return;
	}
	CDuiString strSelName = pSender->GetName();
	if (strSelName == _T("option_review1")) 
	{
		SetShowCefBrowser(m_pBrowserUI, true);
		SetShowCefBrowser(m_pBrowserUI2, false);
		pTabTest->SelectItem(0);
	} else if (strSelName == _T("option_review2")) {
		SetShowCefBrowser(m_pBrowserUI, false);
		SetShowCefBrowser(m_pBrowserUI2, true);
		pTabTest->SelectItem(1);
		//TrayWindowManager::getInstance()->CreateRootWindowAsPopup(L"http://www.baidu.com", this->m_hWnd);
	} else if (strSelName == _T("option_restruct")) {		
		pTabTest->SelectItem(2);
		m_EntryFrameWnd = new CEntryFrameWnd();		
		m_EntryFrameWnd->Create(NULL, _T("DUIWnd"), UI_WNDSTYLE_FRAME, 0L);
		m_EntryFrameWnd->CenterWindow();
		m_EntryFrameWnd->ShowWindow(true);
	} else if (strSelName == _T("option_restruct2")) {		
		pTabTest->SelectItem(2);
		m_EntryFrameWnd2 = new CEntryFrameWnd2();		
		m_EntryFrameWnd2->Create(NULL, _T("DUIWnd"), UI_WNDSTYLE_FRAME, 0L);
		m_EntryFrameWnd2->CenterWindow();
		m_EntryFrameWnd2->ShowWindow(true);
	} else if (strSelName == _T("option_restruct3")) {
        m_EntryFrameWnd3 = new AnalyzeFilms_Wnd();
        m_EntryFrameWnd3->Create(NULL, _T("DUIWnd"), UI_WNDSTYLE_FRAME, 0L);
        m_EntryFrameWnd3->CenterWindow();
        m_EntryFrameWnd3->ShowWindow(true);
    }
}

void CWndSimpleFrame::SetShowCefBrowser(CCefBrowserUI* browser, bool is_show)
{
	// 暂时，使用这种方式，去显示隐藏cef。
	HWND hwnd = browser->m_pBrowser->GetHost()->GetWindowHandle();
	if (hwnd) {
		if (is_show){
			::ShowWindow(hwnd, SW_SHOW);			
		} else {
			::SetWindowPos(hwnd, NULL, 0, 0, 0, 0, SWP_NOZORDER | SWP_NOMOVE | SWP_NOACTIVATE);			
		}
	}
}