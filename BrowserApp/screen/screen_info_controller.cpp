#include "stdafx.h"
#include "screen_info_controller.h"

ScreenInfoController* ScreenInfoController::screen_info_instance = NULL;

const LPCWSTR DEFALT_MONITOR = L"\\\\.\\DISPLAY1";

ScreenInfoController* ScreenInfoController::GetInstance()
{
	if (NULL == screen_info_instance) {
		screen_info_instance = new ScreenInfoController;
	}

	return screen_info_instance;
}

std::map<std::wstring, std::wstring> ScreenInfoController::GetScreenShot()
{
	InitDisplayMonitors();
	std::map<std::wstring, std::wstring> device_screeshot_map;
	std::wstring sreenshot_path =  config_file_path + L"ScreenShot/"; //屏幕截图缓存目录
	if (!PathFileExists(sreenshot_path.c_str())) {
		::CreateDirectory(sreenshot_path.c_str(), NULL);
	}

	for (auto iter = monitor_info_map.begin(); iter != monitor_info_map.end(); ++iter) {
		std::wstring screenshot_file = std::wstring(sreenshot_path + iter->second.szDevice + L".bmp");
		SaveBitmapToFile(GetCaptureBmp(iter->second.rcVirtual), screenshot_file.c_str());
		device_screeshot_map.insert(std::pair<std::wstring, std::wstring>(iter->second.szDevice, screenshot_file));
	}

	return device_screeshot_map;
}

std::wstring ScreenInfoController::DetectionDeafaultScreenName()
{
	//从配置文件读取上次保存的显示器名称
	WCHAR monitor_name[MAX_PATH] = {0};
	::GetPrivateProfileString(L"DeepWiseCore", L"Monitor", L"", monitor_name, MAX_PATH, config_file_name.c_str());
	device_name = monitor_name;
	if (device_name.empty() || monitor_info_map.find(device_name) == monitor_info_map.end()) {
		if (!monitor_info_map.empty()) {
			device_name = monitor_info_map.begin()->first;
		} else {
			device_name = DEFALT_MONITOR;
		}
	}

	return device_name;
}

ScreenInfoController::ScreenInfoController()
{
	Initialize();
}

RECT ScreenInfoController::GetSelectedScreenRect()
{	
	InitDisplayMonitors();
	auto iter = monitor_info_map.find(device_name);
	if (iter != monitor_info_map.end()) {
		return iter->second.rcVirtual;
	} else {
		device_name = DEFALT_MONITOR;
	}

	iter = monitor_info_map.find(device_name);
	if (iter != monitor_info_map.end()) {
		return iter->second.rcVirtual;
	}

	RECT rect;
	rect.left = 0;
	rect.right = 0;
	rect.top = 0;
	rect.bottom = 0;
	return rect;
}

bool ScreenInfoController::SetSelectedScreen(const std::wstring &deviceName)
{
	InitDisplayMonitors();

	//名称deviceName的显示器存在
	if (monitor_info_map.find(deviceName) != monitor_info_map.end()) {
		this->device_name = deviceName;
		if (!PathFileExists(config_file_path.c_str())) {
			::CreateDirectory(config_file_path.c_str(), NULL);
		}
		WritePrivateProfileString(L"DeepWiseCore", L"Monitor", deviceName.c_str(), config_file_name.c_str());
		return true;
	} 

	return false;
}

void ScreenInfoController::Initialize()
{
	config_file_path = Configurations::getInstance()->GetUserDataDir();
	config_file_name = config_file_path + L"DeepWiseUser.ini";
	InitDisplayMonitors();
}

void ScreenInfoController::InitDisplayMonitors()
{ 
	monitor_info_map.clear();
	m_iMontorNumber = 0;
	::EnumDisplayMonitors(NULL, NULL, MonitorEnumProc, (LPARAM)this);
	DetectionDeafaultScreenName();
}

BOOL CALLBACK ScreenInfoController::MonitorEnumProc(HMONITOR hMonitor, HDC hdcMonitor, LPRECT lprcMonitor,  LPARAM dwData)
{
	ScreenInfoController *pThis = (ScreenInfoController *)dwData;
	MONITORINFOEX infoEx;
	memset(&infoEx, 0,  sizeof(infoEx));
	infoEx.cbSize = sizeof(infoEx);
	GetMonitorInfo(hMonitor, &infoEx);
	//保存显示器信息
	VMONITORINFO monitor_info;
	monitor_info.hMonitor = hMonitor;
	if(lprcMonitor)
	{
		monitor_info.rcVirtual = *lprcMonitor;
	}
	monitor_info.rcMonitor = infoEx.rcMonitor;
	monitor_info.rcWork = infoEx.rcWork;
	monitor_info.bPrimary = infoEx.dwFlags == MONITORINFOF_PRIMARY;

	_tcscpy_s(monitor_info.szDevice, infoEx.szDevice);

	pThis->monitor_info_map.insert(std::pair<std::wstring, VMONITORINFO>(std::wstring(infoEx.szDevice), monitor_info));
	return TRUE; 
}

HBITMAP ScreenInfoController::GetCaptureBmp(RECT rect)
{
	HDC       hDC;   
	HDC       MemDC;   
	BYTE*     Data;   
	HBITMAP   hBmp;   
	BITMAPINFO   bi;   
	LONG width  = abs(rect.right - rect.left);
	LONG height = abs(rect.bottom - rect.top);
	memset(&bi, 0, sizeof(bi));   
	bi.bmiHeader.biSize     =  sizeof(BITMAPINFO);
	bi.bmiHeader.biWidth    =  width;    //GetSystemMetrics(SM_CXSCREEN);   
	bi.bmiHeader.biHeight   =  height;   //GetSystemMetrics(SM_CYSCREEN);   
	bi.bmiHeader.biPlanes   =  1;   
	bi.bmiHeader.biBitCount =  24;    

	hDC   = GetDC(NULL);   
	MemDC = CreateCompatibleDC(hDC);   
	hBmp  = CreateDIBSection(MemDC,   &bi, DIB_RGB_COLORS,   (void**)&Data,   NULL,   0);   
	SelectObject(MemDC,   hBmp);   
	BitBlt(MemDC,   0,   0,   width,   height, hDC,   rect.left,   rect.top,   SRCCOPY);   
	ReleaseDC(NULL,   hDC);     
	DeleteDC(MemDC);   
	return   hBmp;
}

bool ScreenInfoController::SaveBitmapToFile(HBITMAP hBitmap, LPCWSTR lpFileName)
{
	HDC                hDC;                //设备描述表
	int                iBits;              //当前显示分辨率下每个像素所占字节数
	WORD               wBitCount = 0;      //位图中每个像素所占字节数    
	DWORD              dwPaletteSize=0;    //定义调色板大小
	DWORD              dwBmBitsSize;       //位图中像素字节大小
	DWORD              dwDIBSize;          //位图文件大小
	DWORD              dwWritten;          //写入文件字节数
	BITMAP             Bitmap;             //位图结构
	BITMAPFILEHEADER   bmfHdr;             //位图属性结构   
	BITMAPINFOHEADER   bi;                 //位图文件头结构
	LPBITMAPINFOHEADER lpbi;               //位图信息头结构     指向位图信息头结构
	HANDLE             fh;                 //定义文件句柄
	HANDLE             hDib;               //分配内存句柄
	HANDLE             hPal;               //分配内存句柄
	HANDLE             hOldPal=NULL;       //调色板句柄  

	//计算位图文件每个像素所占字节数   
	hDC = CreateDC(L"DISPLAY",NULL,NULL,NULL);   
	iBits = GetDeviceCaps(hDC, BITSPIXEL) * GetDeviceCaps(hDC, PLANES);   
	DeleteDC(hDC);

	if (iBits <= 1)       
		wBitCount = 1;   
	else if (iBits <= 4)       
		wBitCount = 4;   
	else if (iBits <= 8)       
		wBitCount = 8;   
	else if (iBits <= 24)
		wBitCount = 24;
	else if (iBits<=32)
		wBitCount = 24;


	//计算调色板大小   
	if (wBitCount <= 8) {       
		dwPaletteSize = (1 << wBitCount) *sizeof(RGBQUAD);
	}

	//设置位图信息头结构   
	GetObject(hBitmap, sizeof(BITMAP), (LPSTR)&Bitmap);   
	bi.biSize            = sizeof(BITMAPINFOHEADER);   
	bi.biWidth           = Bitmap.bmWidth;   
	bi.biHeight          = Bitmap.bmHeight;   
	bi.biPlanes          = 1;   
	bi.biBitCount         = wBitCount;       
	bi.biCompression      = BI_RGB;   
	bi.biSizeImage        = 0;   
	bi.biXPelsPerMeter     = 0;   
	bi.biYPelsPerMeter     = 0;   
	bi.biClrUsed         = 0;   
	bi.biClrImportant      = 0;   
	dwBmBitsSize = ((Bitmap.bmWidth *wBitCount+31)/32)* 4*Bitmap.bmHeight ;

	//为位图内容分配内存   
	hDib  = GlobalAlloc(GHND,dwBmBitsSize+dwPaletteSize+sizeof(BITMAPINFOHEADER));   
	lpbi = (LPBITMAPINFOHEADER)GlobalLock(hDib);
	if (lpbi==NULL)
	{
		return false;
	}

	*lpbi = bi;   
	// 处理调色板
	hPal = GetStockObject(DEFAULT_PALETTE);   
	if (hPal)
	{       
		hDC  = GetDC(NULL);       
		hOldPal = ::SelectPalette(hDC, (HPALETTE)hPal, FALSE);       
		RealizePalette(hDC);       
	}   
	// 获取该调色板下新的像素值   
	GetDIBits(hDC, hBitmap, 0, (UINT) Bitmap.bmHeight,       
		(LPSTR)lpbi + sizeof(BITMAPINFOHEADER)+dwPaletteSize,
		(LPBITMAPINFO)lpbi, DIB_RGB_COLORS);   
	//恢复调色板      
	if (hOldPal)       
	{       
		SelectPalette(hDC, (HPALETTE)hOldPal, TRUE);       
		RealizePalette(hDC);       
		ReleaseDC(NULL, hDC);       
	}   
	//创建位图文件       
	fh = CreateFile(lpFileName, GENERIC_WRITE,        
		0, NULL, CREATE_ALWAYS,       
		FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, NULL);   

	if (fh == INVALID_HANDLE_VALUE) {
		return false;
	}

	// 设置位图文件头   
	bmfHdr.bfType = 0x4D42;  // "BM"   
	dwDIBSize    = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER)+ dwPaletteSize + dwBmBitsSize;     
	bmfHdr.bfSize = dwDIBSize;   
	bmfHdr.bfReserved1 = 0;   
	bmfHdr.bfReserved2 = 0;   
	bmfHdr.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) + (DWORD)sizeof(BITMAPINFOHEADER)+ dwPaletteSize;

	// 写入位图文件头   
	WriteFile(fh, (LPSTR)&bmfHdr, sizeof(BITMAPFILEHEADER), &dwWritten, NULL);

	// 写入位图文件其余内容   
	WriteFile(fh, (LPSTR)lpbi, dwDIBSize, &dwWritten, NULL);

	//清除      
	GlobalUnlock(hDib);   
	GlobalFree(hDib);   
	CloseHandle(fh);

	return true;
}
