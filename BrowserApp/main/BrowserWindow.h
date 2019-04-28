#pragma once

#include <windows.h>
#include <commdlg.h>
#include <string>

#include "include/base/cef_scoped_ptr.h"
#include "include/base/cef_ref_counted.h"
#include "include/cef_browser.h"


class BrowserWindow {
public:
	// Constructor may be called on any thread.
	BrowserWindow();
	~BrowserWindow();

	// RootWindow methods.
	void Init(const CefRect& rect,
		const CefBrowserSettings& settings,
		const std::string& url);
	void InitAsPopup(const CefPopupFeatures& popupFeatures,
		CefWindowInfo& windowInfo,
		CefRefPtr<CefClient>& client,
		CefBrowserSettings& settings);

	void Close(bool force);
	CefRefPtr<CefBrowser> GetBrowser();

private:
	void CreateBrowserWindow(const std::wstring& startup_url);
	void CreateRootWindow(const CefBrowserSettings& settings);
	
	//DISALLOW_COPY_AND_ASSIGN(BrowserWindow);
};