// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

#pragma once

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

#define WM_PRINT_WND_CLOSED			WM_USER+1001
#define WM_PRINT_PREVIEW			WM_USER+1002

#define GOOGLE_GLOG_DLL_DECL
#define GLOG_NO_ABBREVIATED_SEVERITIES

#include "base/control/UICefBrowser.h"
#include "util/Configurations.h"
#include "encrypt/Utf8String.h"
//#include <glog/logging.h>
#include "util/Logger.h"

typedef CCefBrowserUI CCefUI;
#define ENABLE_ENCODE

////OpenCV核心库头文件
//#include <opencv2\core\core.hpp>
////OpenCV图形处理头文件
//#include <opencv2\highgui\highgui.hpp>

#ifdef _USED_CHROME_69_3497
    #pragma comment(lib, "libcef_69_3497/libcef.lib")
#else
    #pragma comment(lib, "libcef_49_2623/libcef.lib")
#endif


#ifdef _DEBUG
#ifdef _USED_CHROME_69_3497
    #pragma comment(lib, "libcef_dll_wrapper_69_3497/libcef_dll_wrapper_d.lib")
    #pragma comment(lib, "libcef_wrapper_wrapper_69_3497/libcef_wrapper_wrapper_d.lib")
#else
    #pragma comment(lib, "libcef_dll_wrapper_49_2623/libcef_dll_wrapper_d.lib")
    #pragma comment(lib, "libcef_wrapper_wrapper_49_2623/libcef_wrapper_wrapper_d.lib")
#endif
    #pragma comment(lib, "Duilib/DuiLib_d.lib")
    #pragma comment(lib, "glog/glog_d.lib")
	////OpenCV核心动态链接库，和core.hpp头文件对应
	//#pragma comment(lib,"Debug/opencv_core320d.lib")
	////OpenCV图形处理动态链接库，和highgui.hpp头文件对应
	//#pragma comment(lib,"Debug/opencv_highgui320d.lib")
#else	
#ifdef _USED_CHROME_69_3497
    #pragma comment(lib, "libcef_dll_wrapper_69_3497/libcef_dll_wrapper.lib")
    #pragma comment(lib, "libcef_wrapper_wrapper_69_3497/libcef_wrapper_wrapper.lib")
#else
    #pragma comment(lib, "libcef_dll_wrapper_49_2623/libcef_dll_wrapper.lib")
    #pragma comment(lib, "libcef_wrapper_wrapper_49_2623/libcef_wrapper_wrapper.lib")
#endif
    #pragma comment(lib, "Duilib/DuiLib.lib")
    #pragma comment(lib, "glog/glog.lib")
	////OpenCV核心动态链接库，和core.hpp头文件对应
	//#pragma comment(lib,"Release/opencv_core320.lib")
	////OpenCV图形处理动态链接库，和highgui.hpp头文件对应
	//#pragma comment(lib,"Release/opencv_highgui320.lib")
#endif

//#include <vld.h>