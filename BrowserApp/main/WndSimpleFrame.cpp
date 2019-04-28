#include "stdafx.h"

#include "WndSimpleFrame.h"

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