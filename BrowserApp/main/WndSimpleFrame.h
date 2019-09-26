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

class CWndSimpleFrame
	: public CWndFrameBase
{
public:
	CWndSimpleFrame(const wstring& url) :CWndFrameBase() { m_pBrowserUI = new CCefBrowserUI(this, url); }

	CDuiString GetSkinFile() OVERRIDE;
	LPCTSTR GetWindowClassName() const OVERRIDE;
	void OnFinalMessage (HWND /*hWnd*/) OVERRIDE;
	void OnExit() OVERRIDE;
	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) OVERRIDE;

	string GetBrowserWindowID() OVERRIDE {return KEY_MAINWINDOW;}
	
	virtual void Notify(TNotifyUI &msg);
	void OnSelChanged(CControlUI* pSender);

private:
	CEntryFrameWnd* m_EntryFrameWnd;
};

	