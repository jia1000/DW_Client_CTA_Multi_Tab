#include "StdAfx.h"

#include "WndSubFrame.h"

CWndSubFrame::CWndSubFrame(const wstring& url) :CWndFrameBase() 
{
	m_pBrowserUI = new CCefBrowserUI(this, url);
	m_sBrowserID = KEY_CHILDWINDOW;
}

void CWndSubFrame::OnFinalMessage(HWND hWnd)
{
	NotifyWindowDestroyed();
	delete this;
}

string CWndSubFrame::GetBrowserWindowID()
{
	return m_sBrowserID;
}

CDuiString CWndSubFrame::GetSkinFile()
{
	return _T("WndSimpleFrame.xml");
}

LPCTSTR CWndSubFrame::GetWindowClassName(void) const 
{ 
	return _T("UISubFrame"); 
}