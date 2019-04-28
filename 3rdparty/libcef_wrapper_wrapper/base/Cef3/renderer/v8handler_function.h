#pragma once

#include <set>
#include <string>

namespace V8WindowFunciotn
{
#define DECLARE_WINDOWS_FUNCTION(name, function) static std::string name = insert(function);

	static std::set<std::string> Functions;
	static auto insert = [&](const char* function)
	{ 
		Functions.insert(function);

		return function;
	};

	// js调用window.FormatRequest(,,,)
	DECLARE_WINDOWS_FUNCTION(loginFunc, "FormatRequest");		//登录验证
	DECLARE_WINDOWS_FUNCTION(decodeFunc, "HtmlDecode");			//解密字符串
	DECLARE_WINDOWS_FUNCTION(encodeFunc, "HtmlEncode");			//解密字符串
	DECLARE_WINDOWS_FUNCTION(maxFunc, "MaximizeWnd");		//最大化窗口
	DECLARE_WINDOWS_FUNCTION(restoreFunc, "RestoreWnd");		//恢复窗口大小
	DECLARE_WINDOWS_FUNCTION(minFunc, "MinimizeWnd");		//最小化窗口
	DECLARE_WINDOWS_FUNCTION(ExitFunc, "CloseWnd");					//退出程序
	DECLARE_WINDOWS_FUNCTION(domTest, "DomVisitTest");
}