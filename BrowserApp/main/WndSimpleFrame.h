#pragma once

/************************************************************************/
/*
	SimpleFrame´°¿Ú
*/
/************************************************************************/

//#define KEY_MAINWINDOW "main"
//#define KEY_CHILDWINDOW "child"
#include "WndFrameBase.h"

class CEntryFrameWnd;
class CEntryFrameWnd2;
class AnalyzeFilms_Wnd;
class VtkDialog;

class CWndSimpleFrame
	: public CWndFrameBase
{
public:
	CWndSimpleFrame(const wstring& url);

	CDuiString GetSkinFile() OVERRIDE;
	LPCTSTR GetWindowClassName() const OVERRIDE;
	void OnFinalMessage (HWND /*hWnd*/) OVERRIDE;
	void OnExit() OVERRIDE;
	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) OVERRIDE;

	string GetBrowserWindowID() OVERRIDE {return KEY_MAINWINDOW;}
	
	virtual void Notify(TNotifyUI &msg);
	void OnSelChanged(CControlUI* pSender);

	void SetShowCefBrowser(CCefBrowserUI* browser, bool is_show);

private:
	CEntryFrameWnd* m_EntryFrameWnd;
	CEntryFrameWnd2* m_EntryFrameWnd2;
    AnalyzeFilms_Wnd* m_EntryFrameWnd3;
	VtkDialog*		m_EntryFrameWnd4;
    
};

	