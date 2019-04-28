#pragma once

#include "cef_client_handler.h"

//using CefProcessTask = std::function<void(CefRefPtr<CefProcessMessage> message)>;
typedef std::function<void(CefRefPtr<CefProcessMessage> message)> CefProcessTask;

#ifdef _USED_CHROME_69_3497
    class CProcessMessageHandler: public CefBaseRefCounted
#else
    class CProcessMessageHandler: public CefBase
#endif
{
public:
	virtual void OnProcessMessageReceived(CefRefPtr<CefProcessMessage> message);

	IMPLEMENT_REFCOUNTING(CProcessMessageHandler);
};