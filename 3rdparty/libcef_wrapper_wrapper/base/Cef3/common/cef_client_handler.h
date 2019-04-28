#pragma once

#include "include/cef_client.h"
#include "include/wrapper/cef_message_router.h"
#include "include/wrapper/cef_resource_manager.h"
#include <functional>
#include <set>
#include "client_callback_handler.h"
#include "include/cef_download_handler.h"
#include "include/cef_download_item.h"
#include "include/cef_request_handler.h"

#define _CEF_BLANK_ _T("about:blank")

class CCefBrowserUI;

typedef std::function<CefRefPtr<CefValue> (CefRefPtr<CefListValue>)> CustomFunction;
typedef std::set<CefMessageRouterBrowserSide::Handler*> MessageHandlerSet;

// Custom menu command Ids.
enum client_menu_ids {
  CLIENT_ID_BACK   = MENU_ID_USER_FIRST,
  CLIENT_ID_FORWORD,
  CLIENT_ID_REFRESH,
  CLIENT_ID_STOPLOAD,
  CLIENT_ID_PRINT,
  CLIENT_ID_EXIT,
};

class CCefClientHandler
	: public CefClient
	, public CefLifeSpanHandler
	, public CefContextMenuHandler
	, public CefLoadHandler
	, public CefDragHandler
	, public CefRequestHandler
	, public CefDisplayHandler
	, public CefDownloadHandler	
	, public CefKeyboardHandler
{
public:
	CCefClientHandler(CCefBrowserUI* pOwner);

	CefRefPtr<CefLifeSpanHandler>		GetLifeSpanHandler()		{ return this; }
	CefRefPtr<CefContextMenuHandler>	GetContextMenuHandler()		{ return this; }
	CefRefPtr<CefLoadHandler>			GetLoadHandler()			{ return this; }
	CefRefPtr<CefDragHandler>			GetDragHandler()			{ return this; }
	CefRefPtr<CefRequestHandler>		GetRequestHandler()			{ return this; }
	CefRefPtr<CefDisplayHandler>		GetDisplayHandler()			{ return this; }
	CefRefPtr<CefDownloadHandler>		GetDownloadHandler()		{ return this; }
	CefRefPtr<CefKeyboardHandler>		GetKeyboardHandler()		{ return this; }


	void CreateBrowser(HWND hParentWnd, RECT rc, const CefString& startupUrl);
	void CloseBrowser(CefRefPtr<CefBrowser> browser);
	void Close();
	void SetRedirectEnabled(bool enabled);

	// CefClient methods:
	//接收消息
	virtual bool OnProcessMessageReceived(CefRefPtr<CefBrowser> browser,
		CefProcessId source_process,
		CefRefPtr<CefProcessMessage> message);


	// CefLifeSpanHandler methods:
	void OnAfterCreated(CefRefPtr<CefBrowser> browser) OVERRIDE;
	bool OnBeforePopup(
		CefRefPtr<CefBrowser> browser,
		CefRefPtr<CefFrame> frame,
		const CefString& target_url,
		const CefString& target_frame_name,
		CefLifeSpanHandler::WindowOpenDisposition target_disposition,
		bool user_gesture,
		const CefPopupFeatures& popupFeatures,
		CefWindowInfo& windowInfo,
		CefRefPtr<CefClient>& client,
		CefBrowserSettings& settings,
		bool* no_javascript_access) OVERRIDE;
	void OnBeforeClose(CefRefPtr<CefBrowser> browser) OVERRIDE;


	// CefContextMenuHandler methods:
	virtual void OnBeforeContextMenu(CefRefPtr<CefBrowser> browser
		, CefRefPtr<CefFrame> frame
		, CefRefPtr<CefContextMenuParams> params
		, CefRefPtr<CefMenuModel> model) OVERRIDE;
    virtual bool OnContextMenuCommand(CefRefPtr<CefBrowser> browser,
                                    CefRefPtr<CefFrame> frame,
                                    CefRefPtr<CefContextMenuParams> params,
                                    int command_id,
                                    EventFlags event_flags) OVERRIDE;


	// CefLoadHandler methods:
	void OnLoadingStateChange(CefRefPtr<CefBrowser> browser,
		bool isLoading,
		bool canGoBack,
		bool canGoForward) OVERRIDE;
	virtual void OnLoadStart(CefRefPtr<CefBrowser> browser
		, CefRefPtr<CefFrame> frame) /*OVERRIDE*/;
	void OnLoadEnd(CefRefPtr<CefBrowser> browser
		, CefRefPtr<CefFrame> frame
		, int httpStatusCode) OVERRIDE;
	void OnLoadError(CefRefPtr<CefBrowser> browser
		, CefRefPtr<CefFrame> frame
		, ErrorCode errorCode
		, const CefString& errorText
		, const CefString& failedUrl) OVERRIDE;


	// CefDragHandler Event
	typedef cef_drag_operations_mask_t DragOperationsMask;
	void OnDraggableRegionsChanged(
		CefRefPtr<CefBrowser> browser,
		const std::vector<CefDraggableRegion>& regions) OVERRIDE;
	bool OnDragEnter(CefRefPtr<CefBrowser> browser
		, CefRefPtr<CefDragData> dragData
		, DragOperationsMask mask)OVERRIDE;

	
	// CefRequestHandler Event
	//Called on the UI thread before browser navigation.
	virtual bool OnBeforeBrowse(CefRefPtr<CefBrowser> browser,
		CefRefPtr<CefFrame> frame,
		CefRefPtr<CefRequest> request,
		bool is_redirect) /*OVERRIDE*/;
	void OnRenderProcessTerminated(CefRefPtr<CefBrowser> browser,
		TerminationStatus status) OVERRIDE;
	cef_return_value_t OnBeforeResourceLoad(
		CefRefPtr<CefBrowser> browser,
		CefRefPtr<CefFrame> frame,
		CefRefPtr<CefRequest> request,
		CefRefPtr<CefRequestCallback> callback) OVERRIDE;
	CefRefPtr<CefResourceHandler> GetResourceHandler(
		CefRefPtr<CefBrowser> browser,
		CefRefPtr<CefFrame> frame,
		CefRefPtr<CefRequest> request) OVERRIDE;
	void OnProtocolExecution(CefRefPtr<CefBrowser> browser,
		const CefString& url,
		bool& allow_os_execution) OVERRIDE;
	bool OnOpenURLFromTab(CefRefPtr<CefBrowser> browser,
                                CefRefPtr<CefFrame> frame,
                                const CefString& target_url,
                                CefRequestHandler::WindowOpenDisposition target_disposition,
								bool user_gesture) OVERRIDE;

	// CefDisplayHandler methods
	void OnAddressChange(CefRefPtr<CefBrowser> browser,
		CefRefPtr<CefFrame> frame,
		const CefString& url) OVERRIDE;
	void OnTitleChange(CefRefPtr<CefBrowser> browser,
		const CefString& title) OVERRIDE;
	void OnFullscreenModeChange(CefRefPtr<CefBrowser> browser,
		bool fullscreen) OVERRIDE;
	virtual bool OnConsoleMessage(CefRefPtr<CefBrowser> browser,
		const CefString& message,
		const CefString& source,
		int line) /*OVERRIDE*/;

	// CefDownloadHandler methods
	void OnBeforeDownload(
		CefRefPtr<CefBrowser> browser,
		CefRefPtr<CefDownloadItem> download_item,
		const CefString& suggested_name,
		CefRefPtr<CefBeforeDownloadCallback> callback) OVERRIDE;
	void OnDownloadUpdated(
		CefRefPtr<CefBrowser> browser,
		CefRefPtr<CefDownloadItem> download_item,
		CefRefPtr<CefDownloadItemCallback> callback) OVERRIDE;

	// CefKeyboardHandler methods
	virtual bool OnPreKeyEvent(CefRefPtr<CefBrowser> browser,
		const CefKeyEvent& event,
		CefEventHandle os_event,
		bool* is_keyboard_shortcut);

	// other
	void PrintToPDF(CefRefPtr<CefBrowser> browser);
	
private:
	void exec_on_main_thread(std::function<void(void)> task);

private:
	CCefBrowserUI* m_pOwner;
	//加载状态
	bool m_bLoading;
	// Handles the browser side of query routing. The renderer side is handled
	// in client_renderer.cc.
	CefRefPtr<CefMessageRouterBrowserSide> message_router_;
	// Set of Handlers registered with the message router.
	// 目前仅注册一个handler：WindowStateHandler
	MessageHandlerSet message_handler_set_;
	CefRefPtr<CefResourceManager> resource_manager_;
	// 允许链接跳转的标识
	bool redirect_enabled;

	IMPLEMENT_REFCOUNTING(CCefClientHandler);
};