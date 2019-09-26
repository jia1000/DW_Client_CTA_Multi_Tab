#include "stdafx.h"

#include "WndSimpleFrame.h"
#include "EntryFrameWnd.h"
#include "BrowserWndManager.h"

//DUI_BEGIN_MESSAGE_MAP(CWndSimpleFrame, WindowImplBase)
//	DUI_ON_MSGTYPE(DUI_MSGTYPE_CLICK,OnClick)
//DUI_END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

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
		pTabTest->SelectItem(0);
	} else if (strSelName == _T("option_review2")) {
		pTabTest->SelectItem(1);
		TrayWindowManager::getInstance()->CreateRootWindowAsPopup(L"http://www.baidu.com", this->m_hWnd);
	} else if (strSelName == _T("option_restruct")) {		
		pTabTest->SelectItem(2);
		m_EntryFrameWnd = new CEntryFrameWnd();		
		m_EntryFrameWnd->Create(NULL, _T("DUIWnd"), UI_WNDSTYLE_FRAME, 0L);
		m_EntryFrameWnd->CenterWindow();
		m_EntryFrameWnd->ShowWindow(true);
	}
}