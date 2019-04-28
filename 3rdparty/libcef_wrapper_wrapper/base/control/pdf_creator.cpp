#include "StdAfx.h"

#include "pdf_Creator.h"
#include "include/wrapper/cef_closure_task.h"
#include "base/Cef3/browser/main_message_loop.h"
//#include "main/BrowserWndManager.h"
#include "util/path.h"

//////////////////////////////////////////////////////////////////////////

CPDFCreation::CPDFCreation(HWND hParentWnd, const CefString& url, const CefString& identity_url, const CefString& path, IPdfCreationCallback* callback)
	: m_pClientHandler(new CCefPDFCreationHandler(this))
	, url_to_print_(url)
	, identity_url_to_print_(identity_url)
	, path_to_generate_(path)
	, creation_callback_(callback)
{
	m_pClientHandler->CreateBrowser(hParentWnd, _CEF_BLANK_);
}

CPDFCreation::~CPDFCreation()
{
	// let client object know CPDFCreation was deleted.
	auto it=pdf_clients_.begin();
	for (;it!=pdf_clients_.end(); it++){
		(*it)->Close();
    }
	pdf_clients_.clear();

	if (m_pClientHandler){
		m_pClientHandler->CloseBrowser(m_pBrowser);
		m_pClientHandler = NULL;
	}
	creation_callback_ = NULL;
	m_pBrowser = NULL;
}

void CPDFCreation::OnAfterCreated(CefRefPtr<CefBrowser> browser)
{
	// 只执行一次
	if (m_pBrowser != nullptr)
		return;

	m_pBrowser = browser;

	// use NavigateString in window instead
	//m_pBrowser->GetMainFrame()->LoadString(url_to_print_, identity_url_to_print_);	

	// 执行缓存的任务
	CefCacheTask task;
	while (!m_AfterCreatedCacheTasks.empty())
	{
		task = move(m_AfterCreatedCacheTasks.front());
		m_AfterCreatedCacheTasks.pop();

		task();
	}

	if (creation_callback_){
		creation_callback_->OnAfterCreated(browser);
	}
}

//加载链接
void CPDFCreation::NavigateString()
{
	if (m_pBrowser != nullptr)
	{
		// load string after browser is created..
		m_pBrowser->GetMainFrame()->LoadString(url_to_print_, identity_url_to_print_);	
	}
	else
	{
		m_AfterCreatedCacheTasks.push([this]
		{
			NavigateString();
		});
	}
}

void CPDFCreation::OnLoadError(CefRefPtr<CefFrame> frame
				 , int errorCode
				 , const CefString& errorText
				 , const CefString& failedUrl)
{

}

void CPDFCreation::OnLoadEnd(CefRefPtr<CefBrowser> browser
			   , CefRefPtr<CefFrame> frame
			   , int httpStatusCode)
{
	if (creation_callback_){
		creation_callback_->OnLoadEnd(browser, frame, httpStatusCode);
	} 
}

void CPDFCreation::NotifyPdfPrinted(const string& guid, const CefString& path, bool ok)
{
	auto it = pdf_clients_.begin();
    for(;it != pdf_clients_.end();++it){
        if((*it)->GetUID() == guid){
            pdf_clients_.erase(it);
			break;
		}
    }

	if (creation_callback_){
		creation_callback_->OnPdfPrintFinished(path, ok);
	}
}

// function to do print to pdf
void CPDFCreation::PrintToPdf(const CefString& targetPath, CefPdfPrintSettings settings)
{
	if (m_pBrowser && targetPath.length() > 0){
		CefRefPtr<Client> cl = new Client(m_pBrowser, targetPath, settings, this);
		pdf_clients_.push_back(cl);
		cl->RunPrint();
	}
}
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
 
CCefPDFCreationHandler::CCefPDFCreationHandler(CPDFCreation* pOwner)
	:m_pOwner(pOwner)
{

}

void CCefPDFCreationHandler::exec_on_main_thread(std::function<void(void)> task)
{
 	if (m_pOwner == nullptr)
 		return;
 
 	if (!CEF_RUN_ON_MAIN_THREAD())
 	{
 		CEF_POST_MAIN_THREAD_TASK(base::Bind(&CCefPDFCreationHandler::exec_on_main_thread, this, task)); 
 	}
 	else
 	{
 		task();
 	}
}

void CCefPDFCreationHandler::CreateBrowser(HWND hParentWnd, const CefString& startupUrl)
{
 	CEF_REQUIRE_MAIN_THREAD();
 
 	CefBrowserSettings settings;
 	CefWindowInfo info;
 	//info.SetAsChild(hParentWnd, rc);
 
 	CefBrowserHost::CreateBrowser(info, this, startupUrl, settings, NULL);
}

void CCefPDFCreationHandler::CloseBrowser(CefRefPtr<CefBrowser> browser)
{
 	CEF_REQUIRE_MAIN_THREAD();
 
 	browser->GetHost()->CloseBrowser(true);
}

void CCefPDFCreationHandler::OnAfterCreated(CefRefPtr<CefBrowser> browser)
{
	CEF_REQUIRE_UI_THREAD();

	exec_on_main_thread([this, browser]
	{
		m_pOwner->OnAfterCreated(browser);
	});
}

void CCefPDFCreationHandler::OnLoadError(CefRefPtr<CefBrowser> browser
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

void CCefPDFCreationHandler::OnLoadEnd(CefRefPtr<CefBrowser> browser
								  , CefRefPtr<CefFrame> frame
								  , int httpStatusCode)
{
	CEF_REQUIRE_UI_THREAD();

	exec_on_main_thread([this, browser, frame, httpStatusCode]
	{
		m_pOwner->OnLoadEnd(browser, frame, httpStatusCode);
	});
}