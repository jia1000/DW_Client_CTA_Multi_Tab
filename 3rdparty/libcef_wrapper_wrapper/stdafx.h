// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             //  从 Windows 头文件中排除极少使用的信息
// Windows 头文件: 
#include <windows.h>

// C 运行时头文件
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

// TODO:  在此处引用程序需要的其他头文件
#include "UIlib.h"

using namespace DuiLib;
using namespace Gdiplus;

#define GOOGLE_GLOG_DLL_DECL
#define GLOG_NO_ABBREVIATED_SEVERITIES

#include "base/control//UICefBrowser.h"
#include "util/Configurations.h"
#include "encrypt/Utf8String.h"
//#include <glog/logging.h>
#include "util/Logger.h"

typedef CCefBrowserUI CCefUI;
#define ENABLE_ENCODE

#ifdef _DEBUG
#ifdef _USED_CHROME_69_3497
    #pragma comment(lib, "libcef_dll_wrapper_69_3497/libcef_dll_wrapper_d.lib")
#else
    #pragma comment(lib, "libcef_dll_wrapper_49_2623/libcef_dll_wrapper_d.lib")
#endif
    #pragma comment(lib, "encrypt/encrypt_d.lib")
    #pragma comment(lib, "util/util_d.lib")
#else
#ifdef _USED_CHROME_69_3497
    #pragma comment(lib, "libcef_dll_wrapper_69_3497/libcef_dll_wrapper.lib")
#else
    #pragma comment(lib, "libcef_dll_wrapper_49_2623/libcef_dll_wrapper.lib")
#endif
    #pragma comment(lib, "encrypt/encrypt.lib")
    #pragma comment(lib, "util/util.lib")
#endif 

// TODO: 在此处引用程序需要的其他头文件
