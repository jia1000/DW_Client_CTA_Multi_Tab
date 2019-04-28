#include "stdafx.h"

#include "cef_client_handler.h"

#include "include/wrapper/cef_closure_task.h"
#include "include/wrapper/cef_helpers.h"

#include "../browser/main_message_loop.h"
#include "base/control/UICefBrowser.h"
#include "../browser/WindowStateHandler.h"
#include "../browser/ClientResourceProvider.h"
#include "../browser/DicomImageStateHandler.h"
#include "../browser/print_to_pdf_handler.h"

const char kTestOrigin[] = "http://client/";
const char kResourceArchiveOrigin[] = "http://archive/";
const char kArchivePassword[] = "";

CCefClientHandler::CCefClientHandler(CCefBrowserUI* pOwner)
	:m_pOwner(pOwner)
{
	resource_manager_ = new CefResourceManager();
	// 默认为允许调整url；如果设为false，则不能打开链接
	redirect_enabled = true;

	//////////////////////////////////////////////////////////////////////////
	// 创建ResourceHandler
	const std::string& test_origin = kTestOrigin;
	const std::string& archive_origin = kResourceArchiveOrigin;
	// Add provider for ordinary resource on local disk.
	resource_manager_->AddProvider(
		new ClientResourceProvider(test_origin),
		0, std::string());
	// Add provider for compressed resources on local disk
	wstring archive_dir = Configurations::getInstance()->GetResourceDir() + L"archive\\";
	const map<wstring, wstring> archive_files = Configurations::getInstance()->GetArchiveFiles();
	for(auto it = archive_files.begin(); it != archive_files.end(); it++) {
        resource_manager_->AddArchiveProvider(
			archive_origin + WStringToString(it->first),
			WStringToString(archive_dir + it->second),
			kArchivePassword,
			0, std::string());
    }
	//////////////////////////////////////////////////////////////////////////
}

void CCefClientHandler::CreateBrowser(HWND hParentWnd, RECT rc, const CefString& startupUrl)
{
	CEF_REQUIRE_MAIN_THREAD();

	CefBrowserSettings settings;
	CefWindowInfo info;
	info.SetAsChild(hParentWnd, rc);

	CefBrowserHost::CreateBrowser(info, this, startupUrl, settings, NULL);
}

void CCefClientHandler::CloseBrowser(CefRefPtr<CefBrowser> browser)
{
	CEF_REQUIRE_MAIN_THREAD();

	browser->GetHost()->CloseBrowser(true);
}

void CCefClientHandler::Close()
{
	CEF_REQUIRE_MAIN_THREAD();

	m_pOwner = nullptr;
}

void CCefClientHandler::SetRedirectEnabled(bool enabled)
{
	redirect_enabled = enabled;
}

bool CCefClientHandler::OnProcessMessageReceived(CefRefPtr<CefBrowser> browser
												 , CefProcessId source_process
												 , CefRefPtr<CefProcessMessage> message)
{
	CEF_REQUIRE_UI_THREAD();

	// message router处理完成之后直接返回
	if (message_router_->OnProcessMessageReceived(browser, source_process,
		message)) {
		return true;
	}

	CefRefPtr<CefProcessMessage> msg = message->Copy();
	
	exec_on_main_thread([this, msg]
	{
		if (m_pOwner != nullptr)
		{
			m_pOwner->OnProcessMessageReceived(msg);
		}
	});

	return true;
}

void CCefClientHandler::OnAfterCreated(CefRefPtr<CefBrowser> browser)
{
	CEF_REQUIRE_UI_THREAD();

 	if (!message_router_) {
 		// Create the browser-side router for query handling.
 		CefMessageRouterConfig config;
 		// 以下使用默认的名称
 		//config.js_query_function
 		//config.js_cancel_function
 		message_router_ = CefMessageRouterBrowserSide::Create(config);
 
 		//// Register handlers with the router.
 		//test_runner::CreateMessageHandlers(message_handler_set_);
		message_handler_set_.insert(new WindowStateHandler());
		message_handler_set_.insert(new PrintToPdfHandler());
		message_handler_set_.insert(new DicomImageStateHandler(m_pOwner));

 		auto it = message_handler_set_.begin();
 		for (; it != message_handler_set_.end(); ++it)
 			message_router_->AddHandler(*(it), false);
 	}


	exec_on_main_thread([this, browser]
	{
		m_pOwner->OnAfterCreated(browser);
	});
}

//////////////////////////////////////////////////////////////////////////
// CefContextMenuHandler methods
void CCefClientHandler::OnBeforeContextMenu(CefRefPtr<CefBrowser> browser
	, CefRefPtr<CefFrame> frame
	, CefRefPtr<CefContextMenuParams> params
	, CefRefPtr<CefMenuModel> model)
{
	if ((params->GetTypeFlags()&(CM_TYPEFLAG_PAGE | CM_TYPEFLAG_FRAME)) != 0) {
        // Add a separator if the menu already has items.
        if (model->GetCount() > 0) {
            //清除默认项
            model->Clear();
            //model->AddSeparator();
        }
		// 删除右键菜单 [12/27/2018 Modified by zhangjian]
  //      //添加菜单项
  //      model->AddItem(CLIENT_ID_BACK, L"后退");
  //      model->AddItem(CLIENT_ID_FORWORD, L"前进");
  //      model->AddSeparator();
		//model->AddItem(CLIENT_ID_REFRESH, L"刷新");
		//model->AddItem(CLIENT_ID_STOPLOAD, L"停止");
  //      //model->AddItem(CLIENT_ID_PRINT, L"打印");
  //      model->AddSeparator();
  //      model->AddItem(CLIENT_ID_EXIT, L"退出");

		////std::string url = browser->GetMainFrame()->GetURL();
		//model->SetEnabled(CLIENT_ID_BACK, browser->CanGoBack());
		//model->SetEnabled(CLIENT_ID_FORWORD, browser->CanGoForward());
		//model->SetEnabled(CLIENT_ID_REFRESH, !m_bLoading);
		//model->SetEnabled(CLIENT_ID_STOPLOAD, m_bLoading);
    }
}

  bool CCefClientHandler::OnContextMenuCommand(CefRefPtr<CefBrowser> browser,
                                    CefRefPtr<CefFrame> frame,
                                    CefRefPtr<CefContextMenuParams> params,
                                    int command_id,
                                    EventFlags event_flags)
  {
	switch (command_id) {
        case CLIENT_ID_BACK:
            if(browser->CanGoBack()) {
                browser->GoBack();
            }
			return true;
        case CLIENT_ID_FORWORD:
            if(browser->CanGoForward()) {
                browser->GoForward();
            }
            return true;
		case CLIENT_ID_REFRESH:
			if (!m_bLoading)
				browser->Reload();
			return true;
		case CLIENT_ID_STOPLOAD:
			if (m_bLoading)
				browser->StopLoad();
			return true;
        case CLIENT_ID_PRINT:
            
            return true;
        case CLIENT_ID_EXIT:
            browser->GetHost()->CloseBrowser(true);
            return true;
    }
    return false;
  }
  //end
  //////////////////////////////////////////////////////////////////////////

  //////////////////////////////////////////////////////////////////////////
  // CefLoadHandler methods
  void CCefClientHandler::OnLoadingStateChange(CefRefPtr<CefBrowser> browser,
	  bool isLoading,
	  bool canGoBack,
	  bool canGoForward)
  {
	  CEF_REQUIRE_UI_THREAD();

	  m_bLoading = isLoading;
	  // Only update the address for the main (top-level) frame.
	  exec_on_main_thread([this, isLoading, canGoBack, canGoForward]
	  {
		  m_pOwner->OnLoadingStateChange(isLoading, canGoBack, canGoForward);
	  });
  }

void CCefClientHandler::OnLoadStart(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame)
{
	CEF_REQUIRE_UI_THREAD();
}

void CCefClientHandler::OnLoadEnd(CefRefPtr<CefBrowser> browser
	, CefRefPtr<CefFrame> frame
	, int httpStatusCode)
{
	CEF_REQUIRE_UI_THREAD();

	exec_on_main_thread([this, browser]
	{
		m_pOwner->OnAfterCreated(browser);
	});
}

void CCefClientHandler::OnLoadError(CefRefPtr<CefBrowser> browser
	, CefRefPtr<CefFrame> frame
	, ErrorCode errorCode
	, const CefString& errorText
	, const CefString& failedUrl)
{
	CEF_REQUIRE_UI_THREAD();

	exec_on_main_thread([this, frame, errorCode, errorText, failedUrl]
	{
		m_pOwner->OnLoadError(frame, errorCode, errorText, failedUrl);
	});
}
//end
//////////////////////////////////////////////////////////////////////////

void CCefClientHandler::OnDraggableRegionsChanged(
	CefRefPtr<CefBrowser> browser,
	const std::vector<CefDraggableRegion>& regions) {
		CEF_REQUIRE_UI_THREAD();

		// Only update the address for the main (top-level) frame.
		exec_on_main_thread([this, regions]
		{
			m_pOwner->OnDraggableRegionsChanged(regions);
		});
}

bool CCefClientHandler::OnDragEnter(CefRefPtr<CefBrowser> browser, CefRefPtr<CefDragData> dragData, DragOperationsMask mask)
{
	CEF_REQUIRE_UI_THREAD();

	// Forbid dragging
	return true;

	//// Forbid dragging of link URLs.
	//if (mask & DRAG_OPERATION_LINK)
	//	return true;

	//return false;
}

void CCefClientHandler::exec_on_main_thread(std::function<void(void)> task)
{
	if (m_pOwner == nullptr)
		return;

	if (!CEF_RUN_ON_MAIN_THREAD())
	{
		CEF_POST_MAIN_THREAD_TASK(base::Bind(&CCefClientHandler::exec_on_main_thread, this, task)); 
	}
	else
	{
		task();
	}
}

//////////////////////////////////////////////////////////////////////////
// CefRequestHandler methods
// Called on the UI thread before browser navigation.
bool CCefClientHandler::OnBeforeBrowse(CefRefPtr<CefBrowser> browser,
								   CefRefPtr<CefFrame> frame,
								   CefRefPtr<CefRequest> request,
								   bool is_redirect)
{
	CEF_REQUIRE_UI_THREAD();
	
//  	wstring wpro = Configurations::getInstance()->GetLocalAppProtocol();
//  	// 如果是客户端自定义协议则返回true停止加载
//  	if (url.ToWString().find(wpro) == 0){
//  		return true;
//  	}

	// Called on the UI thread before browser navigation. Return true to cancel
	// the navigation or false to allow the navigation to proceed.
	// if return true, OnLoadError will be called with an |errorCode| value of
	// ERR_ABORTED.
	// Only update the address for the main (top-level) frame.
	return !redirect_enabled;
}

bool CCefClientHandler::OnOpenURLFromTab(
    CefRefPtr<CefBrowser> browser,
    CefRefPtr<CefFrame> frame,
    const CefString& target_url,
    CefRequestHandler::WindowOpenDisposition target_disposition,
    bool user_gesture) 
{
	// Called on the UI thread before OnBeforeBrowse in certain limited cases
	// where navigating a new or different browser might be desirable.
	// Return true to cancel the navigation or false to allow the navigation to proceed in the
	// source browser's top-level frame.
	return !redirect_enabled;
}

 void CCefClientHandler::OnProtocolExecution(CefRefPtr<CefBrowser> browser,
 										const CefString& url,
 										bool& allow_os_execution) 
 {
 	CEF_REQUIRE_UI_THREAD();
 
// 	wstring wpro = Configurations::getInstance()->GetLocalAppProtocol();
// 	// 如果是客户端的自定义协议则由操作系统继续处理
// 	if (url.ToWString().find(wpro) == 0){
// 		allow_os_execution = true;
// 	}
 }


void CCefClientHandler::OnRenderProcessTerminated(CefRefPtr<CefBrowser> browser,
											  TerminationStatus status) 
{
	CEF_REQUIRE_UI_THREAD();

	message_router_->OnRenderProcessTerminated(browser);

	//// Don't reload if there's no start URL, or if the crash URL was specified.
	//if (startup_url.empty() || startup_url == "chrome://crash")
	//	return;

	//CefRefPtr<CefFrame> frame = browser->GetMainFrame();
	//std::string url = frame->GetURL();

	//// Don't reload if the termination occurred before any URL had successfully
	//// loaded.
	//if (url.empty())
	//	return;

	//start_url = startup_url_;

	//// Convert URLs to lowercase for easier comparison.
	//std::transform(url.begin(), url.end(), url.begin(), tolower);
	//std::transform(start_url.begin(), start_url.end(), start_url.begin(),
	//	tolower);

	//// Don't reload the URL that just resulted in termination.
	//if (url.find(start_url) == 0)
	//	return;

	//frame->LoadURL(startup_url_);

	CefRefPtr<CefFrame> frame = browser->GetMainFrame();
	int errorCode = -10000 - status;
	CefString failedUrl = frame->GetURL();
	exec_on_main_thread([this, frame, errorCode, failedUrl]
	{
		m_pOwner->OnLoadError(frame, errorCode, "<OnRenderProcessTerminated>", failedUrl);
	});
}

bool CCefClientHandler::OnBeforePopup(
	CefRefPtr<CefBrowser> browser,  CefRefPtr<CefFrame> frame,
	const CefString& target_url, 	const CefString& target_frame_name,
	CefLifeSpanHandler::WindowOpenDisposition target_disposition,
	bool user_gesture,
	const CefPopupFeatures& popupFeatures,
	CefWindowInfo& windowInfo,
	CefRefPtr<CefClient>& client,
	CefBrowserSettings& settings,
	bool* no_javascript_access) {
#ifdef _USED_CHROME_69_3497
        CEF_REQUIRE_UI_THREAD();
#else
        CEF_REQUIRE_IO_THREAD();
#endif        
		//// Return true to cancel the popup window.
		//return !CreatePopupWindow(browser, false, popupFeatures, windowInfo, client,
		//                          settings);

#ifdef OPEN_NEW_WINDOW
		// 创建子窗口的逻辑交由TrayWindowManager
		exec_on_main_thread([this, target_url]
		{
			TrayWindowManager::getInstance()->CreateRootWindowAsPopup(target_url);
		});
#else
        //////////////////////////////////////////////////////////////////////////
		// 禁止弹出新窗口，在当前窗口内跳转
		switch (target_disposition) 
		{ 
		case WOD_NEW_FOREGROUND_TAB: 
		case WOD_NEW_BACKGROUND_TAB: 
		case WOD_NEW_POPUP: 
		case WOD_NEW_WINDOW: 
			browser->GetMainFrame()->LoadURL(target_url); 
			return true; 
			//cancel create 
		} 
		//////////////////////////////////////////////////////////////////////////
#endif
        
		return true;
	}


void CCefClientHandler::OnBeforeClose(CefRefPtr<CefBrowser> browser) {
	CEF_REQUIRE_UI_THREAD();
	
	// Remove and delete message router handlers.
	auto it = message_handler_set_.begin();
	for (; it != message_handler_set_.end(); ++it) {
		message_router_->RemoveHandler(*(it));
		delete *(it);
	}
	message_handler_set_.clear();
	message_router_ = NULL;
}

cef_return_value_t CCefClientHandler::OnBeforeResourceLoad(
	CefRefPtr<CefBrowser> browser,
	CefRefPtr<CefFrame> frame,
	CefRefPtr<CefRequest> request,
	CefRefPtr<CefRequestCallback> callback) 
{
	CEF_REQUIRE_IO_THREAD();

	std::string url = request->GetURL();
	CGLogger::Debug("OnBeforeResourceLoad->request url: " + url);

	return resource_manager_->OnBeforeResourceLoad(browser, frame, request, callback);
}

CefRefPtr<CefResourceHandler> CCefClientHandler::GetResourceHandler(
	CefRefPtr<CefBrowser> browser,
	CefRefPtr<CefFrame> frame,
	CefRefPtr<CefRequest> request) 
{
	CEF_REQUIRE_IO_THREAD();

	return resource_manager_->GetResourceHandler(browser, frame, request);
}
//end
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// CefDisplayHandler methods
void CCefClientHandler::OnAddressChange(CefRefPtr<CefBrowser> browser,
									CefRefPtr<CefFrame> frame,
									const CefString& url) 
{
	CEF_REQUIRE_UI_THREAD();

	// Only update the address for the main (top-level) frame.
	exec_on_main_thread([this, url]
	{
		m_pOwner->OnAddressChange(url);
	});
}

void CCefClientHandler::OnTitleChange(CefRefPtr<CefBrowser> browser,
								  const CefString& title) 
{
	CEF_REQUIRE_UI_THREAD();

	exec_on_main_thread([this, title]
	{
		m_pOwner->OnTitleChange(title);
	});									  
}

void CCefClientHandler::OnFullscreenModeChange(CefRefPtr<CefBrowser> browser,
										   bool fullscreen) 
{
	CEF_REQUIRE_UI_THREAD();

	exec_on_main_thread([this, fullscreen]
	{
		m_pOwner->OnFullscreenModeChange(fullscreen);
	});
}

bool CCefClientHandler::OnConsoleMessage(CefRefPtr<CefBrowser> browser,
									 const CefString& message,
									 const CefString& source,
									 int line) 
{
	CEF_REQUIRE_UI_THREAD();

// 										 FILE* file = fopen(console_log_file_.c_str(), "a");
// 										 if (file) {
// 											 std::stringstream ss;
// 											 ss << "Message: " << message.ToString() << NEWLINE <<
// 												 "Source: " << source.ToString() << NEWLINE <<
// 												 "Line: " << line << NEWLINE <<
// 												 "-----------------------" << NEWLINE;
// 											 fputs(ss.str().c_str(), file);
// 											 fclose(file);
// 
// 											 // 不再弹出对话框
// 											 //if (first_console_message_) {
// 											 //  test_runner::Alert(
// 											 //      browser, "Console messages written to \"" + console_log_file_ + "\"");
// 											 //  first_console_message_ = false;
// 											 //}
// 										 }

	return false;
}
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// CefDownloadHandler methods
void CCefClientHandler::OnBeforeDownload(
	CefRefPtr<CefBrowser> browser,
	CefRefPtr<CefDownloadItem> download_item,
	const CefString& suggested_name,
	CefRefPtr<CefBeforeDownloadCallback> callback)
{
	CEF_REQUIRE_UI_THREAD();

	// Continue the download and show the "Save As" dialog.
	wstring defaultDownloadPath = Configurations::getInstance()->GetDefaultDownloadDir();
	wstring fileName = suggested_name;
	CefString downloadPath = defaultDownloadPath + fileName;
	callback->Continue(downloadPath, true);
}

void CCefClientHandler::OnDownloadUpdated(
	CefRefPtr<CefBrowser> browser,
	CefRefPtr<CefDownloadItem> download_item,
	CefRefPtr<CefDownloadItemCallback> callback) 
{
	CEF_REQUIRE_UI_THREAD();

// 	if (download_item->IsComplete()) {
// 
// 	}
}

bool CCefClientHandler::OnPreKeyEvent(CefRefPtr<CefBrowser> browser,
									  const CefKeyEvent& event,
									  CefEventHandle os_event,
									  bool* is_keyboard_shortcut)
{
	CEF_REQUIRE_UI_THREAD();

	if (event.type == KEYEVENT_RAWKEYDOWN) {
		bool ctrl_state = GetKeyState(VK_CONTROL) < 0;
		if (ctrl_state) {
			if (event.windows_key_code == VK_ADD				// 数字键盘的 '+ '
				|| event.windows_key_code == VK_OEM_PLUS		// '-" 
				) {
				exec_on_main_thread([this]
				{
					m_pOwner->SetNextChangeZoomValue(1);
				});	
				return true;
			} else if (event.windows_key_code == VK_SUBTRACT	// 数字键盘的 '-'
				|| event.windows_key_code == VK_OEM_MINUS		// '='
				) {
				exec_on_main_thread([this]
				{
					m_pOwner->SetNextChangeZoomValue(-1);
				});	
				return true;
			}
			
		}
	}

	return false;
}
//////////////////////////////////////////////////////////////////////////
