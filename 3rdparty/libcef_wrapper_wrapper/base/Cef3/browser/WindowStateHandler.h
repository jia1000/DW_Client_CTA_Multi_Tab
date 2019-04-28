#include <algorithm>
#include <string>

#include "include/wrapper/cef_message_router.h"
#include "include/wrapper/cef_stream_resource_handler.h"
#include "main/BrowserWndManager.h"

const char kMessageName[] = "WindowState";

// define window state.
#define WINDOW_MAXIMIZED "MAXIMIZED"
#define WINDOW_MINIMIZED "MINIMIZED"
#define WINDOW_NORMAL "NORMAL"
#define WINDOW_UNKNOWN_MSG "Unknown State"

// Handle messages in the browser process.
class WindowStateHandler : public CefMessageRouterBrowserSide::Handler 
{
public:
	WindowStateHandler() {}

	virtual bool OnQuery(CefRefPtr<CefBrowser> browser,
		CefRefPtr<CefFrame> frame,
		int64 query_id,
		const CefString& request,
		bool persistent,
		CefRefPtr<Callback> callback) OVERRIDE 
	{
		const std::string& message_name = request;
 		if (message_name.find(kMessageName) == 0) {
			// 拿到当前窗体
			HWND hParentWnd = TrayWindowManager::getInstance()->FindWndByBrowser(browser);
			WINDOWPLACEMENT ws;      
			ws.length = sizeof(WINDOWPLACEMENT);
			// 查询最大最小化状态
			GetWindowPlacement(hParentWnd, &ws);      
			switch   (ws.showCmd)      
			{            
			case   1://正常;
				callback->Success(WINDOW_NORMAL);
				break;
			case   2://最小化;
				callback->Success(WINDOW_MINIMIZED);
				break;
			case   3://最大化;
				callback->Success(WINDOW_MAXIMIZED);
				break;
			default:
				//LOG(ERROR) << "Failed to query correct window state.";
				CGLogger::Error("Failed to query correct window state.");

				callback->Failure(-1, WINDOW_UNKNOWN_MSG);
			}
			return true;
 		}
		// let other handlers to process.
		return false;
	}

	virtual void OnQueryCanceled(CefRefPtr<CefBrowser> browser,
		CefRefPtr<CefFrame> frame,
		int64 query_id) OVERRIDE
	{
		//LOG(INFO) << "Query request [Id: " << query_id << "] was cancelled.";
		CGLogger::Info("Query request [Id: " + to_string(query_id) + "] was cancelled.");

		// cancel async query task...
	}
};