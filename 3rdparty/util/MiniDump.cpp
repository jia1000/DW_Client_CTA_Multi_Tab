#include "StdAfx.h"
#include "MiniDump.h"

#include <windows.h>
#include <DbgHelp.h>
#include <Shlwapi.h>

#pragma comment(lib, "version.lib")

wstring GetFileDescription()
{
	wchar_t exeFullPath[1024]; // MAX_PATH
	GetModuleFileName(NULL, exeFullPath, 1024);//得到程序模块名称，全路径 
	// 读文件信息 
	DWORD dwVerHnd = 0;
	DWORD dwVerInfoSize = ::GetFileVersionInfoSize(exeFullPath, &dwVerHnd);

	if (dwVerInfoSize)
	{
		// If we were able to get the information, process it: 
		HANDLE hMem;
		LPVOID lpvMem;

		hMem = ::GlobalAlloc(GMEM_MOVEABLE, dwVerInfoSize);
		lpvMem = ::GlobalLock(hMem);
		::GetFileVersionInfo(exeFullPath, dwVerHnd, dwVerInfoSize, lpvMem);

		struct LANGANDCODEPAGE {
			WORD wLanguage;
			WORD wCodePage;
		} *lpTranslate;

		// Read the list of languages and code pages.
		UINT cbTranslate = sizeof(LANGANDCODEPAGE);
		VerQueryValue(lpvMem,
			TEXT("\\VarFileInfo\\Translation"),
			(LPVOID*)&lpTranslate,
			&cbTranslate);

		wchar_t chSubBlock[1000] = { 0 };
		swprintf_s(chSubBlock, _T("\\StringFileInfo\\%04x%04x\\ProductVersion"), lpTranslate->wLanguage, lpTranslate->wCodePage);

		wchar_t* buffer = NULL;
		UINT nSize = 0;
		VerQueryValue(lpvMem, chSubBlock, (LPVOID*)&buffer, &nSize);

		std::wstring text(buffer);

		::GlobalUnlock(hMem);
		::GlobalFree(hMem);

		//return buffer;
		return text;
	}
	return wstring();
}

wstring GetDumpPath()
{
	//得到当前时间
	SYSTEMTIME st;
	::GetLocalTime(&st);
	wchar_t chName[1000] = { 0 };
	swprintf_s(chName, _T("_%04d%02d%02d%02d%02d%02d%03d.dmp"), st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
	//得到程序所在文件夹
	wstring strFullPath = Configurations::getInstance()->GetUserDataDir() + _T("dump\\");
	if (!PathFileExists(strFullPath.c_str())) {
		CreateDirectory(strFullPath.c_str(), NULL);
	}
	int process_type = Configurations::getInstance()->GetProcessType();
	strFullPath = strFullPath + GetFileDescription() + _T("_") + to_wstring(process_type) + wstring(chName);
	return strFullPath;
}

//程序未捕获的异常处理函数 
LONG WINAPI ExceptionFilter(struct _EXCEPTION_POINTERS *ExceptionInfo) 
{ 
	wstring dumpPath = GetDumpPath();
	HANDLE hFile = ::CreateFile(dumpPath.c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	CGLogger::Error("MiniDump file: " + WStringToString(dumpPath));

	if( hFile != INVALID_HANDLE_VALUE) 
	{ 
		MINIDUMP_EXCEPTION_INFORMATION einfo; 
		einfo.ThreadId = ::GetCurrentThreadId(); 
		einfo.ExceptionPointers = ExceptionInfo; 
		einfo.ClientPointers = FALSE; 

		::MiniDumpWriteDump(::GetCurrentProcess(), ::GetCurrentProcessId(), hFile, MiniDumpNormal, &einfo, NULL, NULL); 
		::CloseHandle(hFile); 
	} 
	exit(0);
	return 0; 
} 

void InitMiniDump()
{
	::SetUnhandledExceptionFilter(ExceptionFilter); //设置异常处理函数 
}