#include <algorithm>
#include <string>
#include "include/wrapper/cef_message_router.h"
#include "include/wrapper/cef_stream_resource_handler.h"
//#include "main/BrowserWndManager.h"
#include "include/base/cef_bind.h"
#include "include/cef_parser.h"

const char kPrintToPdf[] = "PrintToPdf";

// Handle messages in the browser process to generate pdf
class PrintToPdfHandler : public CefMessageRouterBrowserSide::Handler 
{
public:
	PrintToPdfHandler() {};
	~PrintToPdfHandler() {};

	virtual bool OnQuery(CefRefPtr<CefBrowser> browser,
		CefRefPtr<CefFrame> frame,
		int64 query_id,
		const CefString& request,
		bool persistent,
		CefRefPtr<Callback> callback) OVERRIDE 
	{
		const std::string& message_name = request.ToString();
		if (message_name.find(kPrintToPdf) == 0) {

			GetSourceTest(frame);
			
			callback->Success("");

			return true;
		}
		// let other handlers to process.
		return false;
	}

	void GetSourceTest(CefRefPtr<CefFrame> frame) 
	{
		class Visitor : public CefStringVisitor {
		public:
			explicit Visitor(CefRefPtr<CefFrame> frame) : frame_(frame) {}
			virtual void Visit(const CefString& str) OVERRIDE {
				std::string data = str;

				CefString urlFromString = "data:text/html;base64," +
					CefURIEncode(CefBase64Encode(data.data(), data.size()), false).ToString();

				TrayWindowManager::getInstance()->CreatePrintWindow(str.ToWString());
			}

			std::string getResult(){
				return source_;
			}
		private:
			CefRefPtr<CefFrame> frame_;
			CefString source_;
			IMPLEMENT_REFCOUNTING(Visitor);
		};

		CefRefPtr<Visitor> vi = new Visitor(frame);
		frame->GetSource(vi);
	}

	virtual void OnQueryCanceled(CefRefPtr<CefBrowser> browser,
		CefRefPtr<CefFrame> frame,
		int64 query_id) OVERRIDE
	{
		// cancel async query task...
	}

private:

};