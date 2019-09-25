#pragma once

#include <queue>

#include "include/cef_client.h"

#include "base/Cef3/common/process_message_handler.h"
#include "base/Cef3/common/cef_client_handler.h"
#include "base/Cef3/renderer/v8handler_function.h"

//using CefCacheTask = std::function<void(void)>;
typedef std::function<void(void)> CefCacheTask;
//类名称
const TCHAR CCefBrowserUIClassName[] = L"CefBrowserUI";		
//控件名称
const TCHAR CCefBrowserUIItemInterface[] = L"CefBrowser";

class CCefBrowserUI : public CControlUI
	, public IDialogBuilderCallback
{
public:
	CCefBrowserUI(ICallbackDelegate* callbackDelegate, const CefString& url);
	virtual ~CCefBrowserUI();

	// Control
	LPCTSTR GetClass() const;
	LPVOID GetInterface(LPCTSTR pstrName);
	void DoInit();
	void DoPaint(HDC hDC, const RECT& rcPaint);
	void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);
	void SetVisible(bool bVisible);
	void SetInternVisible(bool bVisible);
	
	// CefClientHandler回调接口
	void OnProcessMessageReceived(CefRefPtr<CefProcessMessage> message);
	void OnAfterCreated(CefRefPtr<CefBrowser> browser);

	//////////////////////////////////////////////////////////////////////////
	//以下是自定义接口
	//是否可以前进
	bool CanGoForward();
	//前进
	void GoForward();
	//重新加载
	void Reload();
	//忽略缓存加载
	void ReloadIgnoreCache();
	//停止加载
	void StopLoad();
	//是否可以后退
	bool CanGoBack();
	//后退
	void GoBack();
	//加载链接
	void Navigate2(CefString url);
	//执行JS
	void RunJs(CefString JsCode);
	//设置缩放比例
	void SetZoomLevel(double scale);
	void SetNextChangeZoomValue(int step);
	//调用系统打印对话框
	void Print();
	//获取当前网页的地址
	CefString GetCurrentUrl();
	//设置是否可以跳转链接
	void SetRedirectEnabled(bool enabled);
	//////////////////////////////////////////////////////////////////////////

	// Execute Delegate notifications on the main thread.
	// CefClientHandler回调接口
	void OnAddressChange(const CefString& url);
	void OnTitleChange(const CefString& title);
	void OnFullscreenModeChange(bool fullscreen);
	void OnLoadingStateChange(bool isLoading, bool canGoBack, bool canGoForward);
	void OnDraggableRegionsChanged(const std::vector<CefDraggableRegion>& regions);
	void OnLoadError(CefRefPtr<CefFrame> frame
		, int errorCode
		, const CefString& errorText
		, const CefString& failedUrl);

	// 进程消息处理
	void SetProcessMessageHandler(CefRefPtr<CProcessMessageHandler> pHandler);

	// 实现自定义控件的创建
	CControlUI* CreateControl(LPCTSTR pstrClass, CPaintManagerUI *pm) override
	{
		//if (_tcscmp(pstrClass, _T("Cef")) == 0) return new CCefUI();
		if (_tcscmp(pstrClass, _T("Cef")) == 0) return this;
		return NULL;
	}
	//获取缩放比例
	double GetZoomLevel();

private:
	void resize();
	//设置焦点
	void SetFocus(bool enable);
	string GetErrorString(int code);

private:	
	CefRefPtr<CCefClientHandler>			m_pClientHandler;
	CefRefPtr<CefBrowser>					m_pBrowser;
	CefRefPtr<CProcessMessageHandler>		m_pProcessMessageHandler;
	CefString								m_sStartupUrl;
	ICallbackDelegate*						m_CallbackDelegate;
	// browser创建完成前缓存的任务
	std::queue<std::function<void(void)>>	m_AfterCreatedCacheTasks;
	wstring									last_opened_url_;
};