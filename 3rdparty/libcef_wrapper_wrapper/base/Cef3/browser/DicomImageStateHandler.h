#include <algorithm>
#include <string>

#include "base/control/UICefBrowser.h"
#include "include/wrapper/cef_message_router.h"
#include "include/wrapper/cef_stream_resource_handler.h"
#include "main/BrowserWndManager.h"


// define window state.
#define WINDOW_MAXIMIZED "MAXIMIZED"
#define WINDOW_MINIMIZED "MINIMIZED"
#define WINDOW_NORMAL "NORMAL"
#define WINDOW_UNKNOWN_MSG "Unknown State"

// Handle messages in the browser process.
class DicomImageStateHandler : public CefMessageRouterBrowserSide::Handler 
{
public:
	DicomImageStateHandler(CCefBrowserUI* owner) 
		: m_pOwner(owner)
	{}

	virtual bool OnQuery(CefRefPtr<CefBrowser> browser,
		CefRefPtr<CefFrame> frame,
		int64 query_id,
		const CefString& request,
		bool persistent,
		CefRefPtr<Callback> callback) OVERRIDE 
	{
		const std::string& message_name = request;
		if (message_name.find("DicomImageState") == 0) {
			return true;
		}
		else if (request == "success")
		{
			callback->Success("HelloCefQuery_Test_Success Ok");

			//////////////////////////////////////////////////////////////////////////
			std::wstring text;
			std::wstringstream wss;
			wss << _T("jsSendCount('");
			wss << _T(".send_txt:");
			wss << _T("success");

			wss << _T("')");
			wss >> text;

			m_pOwner->RunJs(text);
			return true;
		}
		else if (request == "failure")
		{
			callback->Failure(404, "There are none thus query!  --- HelloCefQuery_Test_Failure");
			//////////////////////////////////////////////////////////////////////////
			std::wstring text;
			std::wstringstream wss;
			wss << _T("jsSendCount('");
			wss << _T(".send_txt:");
			wss << _T("failure");

			wss << _T("')");
			wss >> text;

			m_pOwner->RunJs(text);
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
private:
	CCefBrowserUI* m_pOwner;
};