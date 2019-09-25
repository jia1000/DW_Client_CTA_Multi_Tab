#pragma once
//#include "base/Cef3/common/client_callback_handler.h"
#include "WndFrameBase.h"

#include <UIlib.h>
using namespace DuiLib;

class CCefBrowserUI;

//////////////////////////////////////////////////////////////////////////
// 该类使用xml文件创建窗口,其中，有cef浏览器控件(参考了官方cefclient示例）
//////////////////////////////////////////////////////////////////////////

#define KEY_MAINWINDOW "main"

#include <algorithm>

class CCefBrowserClientFrameWnd
	: public CWndFrameBase
	//, public ICallbackDelegate
{
public:
	// This interface is implemented by the owner of the CCefBrowserClientFrameWnd. The methods
	// of this class will be called on the main thread.
	class Delegate {
	public:
		// Called to retrieve the CefRequestContext for browser. Only called for
		// non-popup browsers. May return NULL.
		virtual CefRefPtr<CefRequestContext> GetRequestContext(
			CCefBrowserClientFrameWnd* root_window) = 0;
		// Called to exit the application.
		virtual void OnExit(CCefBrowserClientFrameWnd* root_window) = 0;
		// Called when the RootWindow has been destroyed.
		virtual void OnRootWindowDestroyed(CCefBrowserClientFrameWnd* root_window) = 0;

	protected:
		virtual ~Delegate() {}
	};

	CCefBrowserClientFrameWnd(const wstring& url);
	virtual ~CCefBrowserClientFrameWnd(void);

	virtual CDuiString GetSkinFile() ;
	virtual LPCTSTR GetWindowClassName(void) const { return  L"{1101EC6A-570B-4983-96AC-3956AC093F16}"; }
	virtual void Notify(TNotifyUI &msg);
	virtual void InitWindow();
	//消息集成处理函数
	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual CControlUI* CreateControl(LPCTSTR pstrClass, CPaintManagerUI *pManager);
	virtual LRESULT OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	virtual LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	void SetMinInfo(int cx, int cy);
	void OnPrepare();
	virtual string GetBrowserWindowID() {return KEY_MAINWINDOW;}
	virtual void Minimize();
	virtual void Maximize();
	virtual void Restore();
	//virtual void Exit();
	virtual void LoadUrl(wstring url);

	// ICallbackDelegate methods
	void OnSetAddress(const std::wstring& url) OVERRIDE;
	// Set the window title.
	void OnSetTitle(const std::wstring& title) OVERRIDE;
	// Set fullscreen mode.
	void OnSetFullscreen(bool fullscreen) OVERRIDE;
	// Set the loading state.
	void OnSetLoadingState(bool isLoading,
		bool canGoBack,
		bool canGoForward) OVERRIDE;
	// Set the draggable regions.
	void OnSetDraggableRegions(
		const std::vector<CefDraggableRegion>& regions) OVERRIDE;
	// Set load error message log
	void OnLoadError(CefRefPtr<CefFrame> frame
		, int errorCode
		, const CefString& errorText
		, const CefString& failedUrl);
	// Minimize window.
	void OnMinimized() OVERRIDE;
	// Exit application.
	void OnExit() OVERRIDE;
	//
	virtual void OnZoomValueChanged(int step);
	//
	virtual void OnDoSpecialSomething(int type_id, std::wstring command);

	bool IsDragRect(POINT pt);
	void SetDelegate(CCefBrowserClientFrameWnd::Delegate* wndDelegate){ m_pDelegate = wndDelegate;}
	virtual void NotifyWindowDestroyed();
	wstring GetUrl();
	wstring GetTitle();

	void SetBrowserZoomLevel(double level);

	void OnSelChanged(CControlUI* pSender);
	//cef
	void ShowCefWindow();
	void HideCefWindow();

protected:
	CCefBrowserClientFrameWnd::Delegate* m_pDelegate;
	//基类中也有该变量，随后要调整下代码
	CCefBrowserUI* m_pCtaOnlyBrowserUI;

private:
	// 定义导航和地址栏控件
	CButtonUI* m_pOkBtn;
	CEditUI* m_pAddressEdit;
	CButtonUI* m_pBackBtn;
	CButtonUI* m_pForwardBtn;
	CButtonUI* m_pRefreshBtn;
	CButtonUI* m_pRefreshBtnNew;
	CButtonUI* m_pStopBtn;
	// 定义导航和地址栏控件结束
	CLabelUI* m_title_label;
};