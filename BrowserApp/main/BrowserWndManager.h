/****************************************************************************************************

作者: mamx 2018-12-25 改造
备注: CEF套壳客户端托盘及浏览器窗口管理器
审核:

Copyright(c) 2018-2030 DeepWise Corporation

****************************************************************************************************/

// Copyright (c) 2015 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#ifndef CEF_TESTS_CEFCLIENT_BROWSER_ROOT_WINDOW_MANAGER_H_
#define CEF_TESTS_CEFCLIENT_BROWSER_ROOT_WINDOW_MANAGER_H_
#pragma once

#include <set>

#include "BrowserWindow.h"
#include "WndFrameBase.h"

class MenuTraySet_Wnd;
class CCefMainHandler;

template <class T>
CWndFrameBase* CreateMainWnd(LPCTSTR pstrName, CWndFrameBase::Delegate* wndDelegate, const std::wstring& url, HWND parentWnd, DWORD dwStyle)
{
	CWndFrameBase *mainFrame = new T(url);
	mainFrame->Create(parentWnd, pstrName
		, dwStyle
		, WS_EX_OVERLAPPEDWINDOW
		, 0, 0, 0, 0);
	mainFrame->SetDelegate(wndDelegate);
	return mainFrame;
}

// Used to create/manage RootWindow instances. The methods of this class can be
// called from any browser process thread unless otherwise indicated.
class TrayWindowManager : CWndFrameBase::Delegate, public CWindowWnd {
public:
	static TrayWindowManager* getInstance();
	~TrayWindowManager();

	// Create a new top-level native window that loads |url|.
	void CreateRootWindow(const wstring& url);

	// create a new popup window.
	void CreateRootWindowAsPopup(const wstring& url, HWND parent_wnd);
	// create print preview dialog window
	void CreatePrintWindow(const wstring& url);
	
	// CWndFrameBase::Delegate implementation
	CefRefPtr<CefRequestContext> GetRequestContext(
		CWndFrameBase* root_window) OVERRIDE;
	// Called to exit the application.
	void OnExit(CWndFrameBase* root_window) OVERRIDE;
	// Called when the RootWindow has been destroyed.
	void OnRootWindowDestroyed(CWndFrameBase* root_window)OVERRIDE;
	// hold a instance of m_pCefMainHandler
	void SetCefMainHandler(CCefMainHandler* handler){m_pCefMainHandler=handler;}
	// find HWND of window by browser
	HWND FindWndByBrowser(CefRefPtr<CefBrowser> browser);
	//end

	virtual LPCTSTR GetWindowClassName() const;
	//消息集成处理函数
	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual void OnFinalMessage( HWND );

	void DestroyedTrayManager(); //关闭托盘，退出程序
    void CreateMenuTraySetWnd(); //创建托盘--设置窗口
    void DestroyedTraySetWnd();  //关闭托盘--设置窗口
    // 通知缩放相关的控件
    void   NotifyZoomIndexChanged(int zoom_index);
private:
	TrayWindowManager();
	void LoadUrlByCommandLine(LPCWSTR command_line_str);
    
    double SetConfigZoomIndex(int zoom_index);
    void   SetWndFrameZoomLevel(double level);
    void   SetSliderIndex(int level_index);
    void   SetZoomButtonValue(double value);

private:
	static TrayWindowManager* m_pInstance;
	std::map<string, CWndFrameBase*> m_mapBrowserWindows;
	CCefMainHandler* m_pCefMainHandler;
	CTrayIcon* tray_icon;
	CWndFrameBase* browser_wnd;
    MenuTraySet_Wnd* m_menu_tray_set_wnd;
};

#endif  // CEF_TESTS_CEFCLIENT_BROWSER_ROOT_WINDOW_MANAGER_H_
