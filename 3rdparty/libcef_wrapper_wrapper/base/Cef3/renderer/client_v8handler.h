#pragma once

#include "include/cef_v8.h"
#include "v8handler_function.h"
//#include "client_Dom_visitor.h"

//class IOnContextCreatedSolt : public CefBase {
//	public:
//		////V8环境创建回调
//		//virtual void OnContextCreated(CefRefPtr<CefBrowser> browser,
//		//	CefRefPtr<CefFrame> frame,
//		//	CefRefPtr<CefV8Context> context) = 0;
//
//		////链接命名管道
//		//virtual void ConnectionNamePipe(const CefString& pipe_name) = 0;
//		////设置函数名
//		//virtual void SetFunction(const CefString& name) = 0;
//	private:
//		IMPLEMENT_REFCOUNTING(IOnContextCreatedSolt);
//	};

class V8Handler: public CefV8Handler
				//,public IOnContextCreatedSolt
{
public:

	virtual bool Execute(const CefString& name, CefRefPtr<CefV8Value> object, \
		const CefV8ValueList& arguments, CefRefPtr<CefV8Value>& retval, CefString& exception);

 	// 登陆验证返回加密字符串
 	std::string GenerateKey(const string& str);
 	// 加解密接口
 	std::string Decode(const string& str);
 	std::string Encode(const string& str);

	// Include the default reference counting implementation.
	IMPLEMENT_REFCOUNTING(V8Handler);

private:
	//HANDLE					handle_name_pipe_;	//命名管道句柄
};

