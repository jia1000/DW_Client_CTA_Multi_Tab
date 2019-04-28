#pragma once
#include "WndFrameBase.h"

/************************************************************************/
/*
	CWndSubFrame´°¿Ú
*/
/************************************************************************/

class CWndSubFrame
	: public CWndFrameBase
{
public:
	CWndSubFrame(const wstring& url);

	CDuiString GetSkinFile() OVERRIDE;
	LPCTSTR GetWindowClassName() const OVERRIDE;
	void OnFinalMessage(HWND /*hWnd*/) OVERRIDE;

	string GetBrowserWindowID() OVERRIDE;

private:
	string m_sBrowserID;	
};