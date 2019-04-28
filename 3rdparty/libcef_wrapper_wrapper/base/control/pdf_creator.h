#pragma once

#include "include/cef_client.h"

#include "base/Cef3/common/process_message_handler.h"
#include "base/Cef3/common/cef_client_handler.h"
#include "base/Cef3/renderer/v8handler_function.h"
#include "include/cef_client.h"
#include <objbase.h>
#include <stdio.h>

/************************************************************************/
/*
	生成PDF文件
*/
/************************************************************************/

class CCefPDFCreationHandler;
class IPdfCreationCallback;
class Client;

class CPDFCreation //: public CefBase
{
public:
	CPDFCreation(HWND hParentWnd, const CefString& url, const CefString& path, const CefString& identity_url, IPdfCreationCallback* callback);
	~CPDFCreation();

	void OnAfterCreated(CefRefPtr<CefBrowser> browser);
	void OnLoadError(CefRefPtr<CefFrame> frame
		, int errorCode
		, const CefString& errorText
		, const CefString& failedUrl);
	void OnLoadEnd(CefRefPtr<CefBrowser> browser
		, CefRefPtr<CefFrame> frame
		, int httpStatusCode);
	void PrintToPdf(const CefString& targetPath, CefPdfPrintSettings settings);
	void NavigateString();
	// 通知打印窗口pdf文件已经生成
	void NotifyPdfPrinted(const string& guid, const CefString& path, bool ok);
	
private:
	// browser创建完成前缓存的任务
	std::queue<std::function<void(void)>>	m_AfterCreatedCacheTasks;
	CefRefPtr<CCefPDFCreationHandler>		m_pClientHandler;
	CefRefPtr<CefBrowser>					m_pBrowser;
	CefString								url_to_print_;
	CefString								identity_url_to_print_;
	CefString								path_to_generate_;
	IPdfCreationCallback*					creation_callback_;
	vector<CefRefPtr<Client>>				pdf_clients_;

	//IMPLEMENT_REFCOUNTING(CPDFCreation);
};

class CCefPDFCreationHandler
	: public CefClient
	, public CefLifeSpanHandler
	, public CefLoadHandler
{
public:
	CCefPDFCreationHandler(CPDFCreation* pOwner);

	CefRefPtr<CefLifeSpanHandler>		GetLifeSpanHandler()		{ return this; }
	CefRefPtr<CefLoadHandler>			GetLoadHandler()			{ return this; }

	void CreateBrowser(HWND hParentWnd, const CefString& startupUrl);
	void CloseBrowser(CefRefPtr<CefBrowser> browser);

 	// CefLoadHandler methods:
 	void OnLoadEnd(CefRefPtr<CefBrowser> browser
 		, CefRefPtr<CefFrame> frame
 		, int httpStatusCode) OVERRIDE;
  	void OnLoadError(CefRefPtr<CefBrowser> browser
  		, CefRefPtr<CefFrame> frame
  		, ErrorCode errorCode
  		, const CefString& errorText
  		, const CefString& failedUrl) OVERRIDE;

	// CefLifeSpanHandler methods:
	void OnAfterCreated(CefRefPtr<CefBrowser> browser) OVERRIDE;

private:
	void exec_on_main_thread(std::function<void(void)> task);

private:
	CPDFCreation* m_pOwner;
	// Include the default reference counting implementation.
	IMPLEMENT_REFCOUNTING(CCefPDFCreationHandler);

};

class IPdfCreationCallback //: public CefBase
{
public:
	virtual void OnAfterCreated(CefRefPtr<CefBrowser> browser) = 0;
	virtual void OnLoadEnd(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, int httpStatusCode) = 0;
	virtual void OnPdfPrintFinished(const std::wstring& file_path, bool ok) = 0;

private:
	//IMPLEMENT_REFCOUNTING(IPdfCreationCallback);
};

class Client : public CefPdfPrintCallback {
	public:
		explicit Client(CefRefPtr<CefBrowser> browser, CefString target_path, CefPdfPrintSettings settings, CPDFCreation* handler)
			: browser_(browser)
			, target_path_(target_path)
			, handler_(handler)
			, settings_(settings){				
			guid_ = GenerateGUID();
		}
		//1.生成pdf预览文件
		void RunPrint(){
			if (browser_){
				// Print to the selected PDF file.
				browser_->GetHost()->PrintToPDF(target_path_, settings_, this);
			}
		}

		void OnPdfPrintFinished(const CefString& path, bool ok) OVERRIDE {
	 		// 2.创建预览窗口
			if (handler_){
				handler_->NotifyPdfPrinted(guid_, path, ok);
			}
		}

		void Close(){
			handler_ = NULL;
		}

		string GetUID(){
			return guid_;
		}

	private:
		const char* GenerateGUID(){
			static char buf[64] = {0};
			GUID guid;
			if (S_OK == ::CoCreateGuid(&guid)){
				_snprintf(buf, sizeof(buf)
					, "{%08X-%04X-%04x-%02X%02X-%02X%02X%02X%02X%02X%02X}"
					, guid.Data1
					, guid.Data2
					, guid.Data3
					, guid.Data4[0], guid.Data4[1]
				, guid.Data4[2], guid.Data4[3], guid.Data4[4], guid.Data4[5]
				, guid.Data4[6], guid.Data4[7]
				);
			}
			return (const char*)buf;
		}

	private:
		CefRefPtr<CefBrowser> browser_;
		CefPdfPrintSettings settings_;
		CefString target_path_;
		CPDFCreation* handler_;
		string guid_;

		IMPLEMENT_REFCOUNTING(Client);
	};