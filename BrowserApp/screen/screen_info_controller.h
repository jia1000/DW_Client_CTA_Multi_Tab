/****************************************************************************************************

作者: mamx 2018-12-19
备注: 识别显示器信息、保存程序默认显示器配置
审核:

Copyright(c) 2018-2030 DeepWise Corporation

****************************************************************************************************/

#pragma once
#include <map>
#include <tchar.h>
#include <windows.h>
class ScreenInfoController
{
public:
	static ScreenInfoController* GetInstance();
	RECT GetSelectedScreenRect();                            //获取上次设置的显示器位置 
	bool SetSelectedScreen(const std::wstring &deviceName);  //设置选择的显示器
	std::map<std::wstring, std::wstring> GetScreenShot();    //获取显示器截图和截图文件目录
	std::wstring DetectionDeafaultScreenName();              //检测主程序当前显示的显示器名称

protected:
	ScreenInfoController();
	~ScreenInfoController(){}

private:
	void Initialize();

	typedef struct VMONITORINFO
	{
		HMONITOR hMonitor;      //显示器句柄
		TCHAR    szDevice[32];  //显示器名
		RECT     rcVirtual;     //虚拟显示屏坐标
		RECT     rcMonitor;     //物理显示坐标
		RECT     rcWork;        //工作显示坐标
		BOOL     bPrimary;      //主显示器？

		VMONITORINFO() {
			memset(this, 0, sizeof(*this));
		}
	};

	void InitDisplayMonitors();

	//枚举显示器回调函数
	static BOOL CALLBACK MonitorEnumProc(
		HMONITOR hMonitor,  // handle to display monitor
		HDC hdcMonitor,     // handle to monitor-appropriate device context
		LPRECT lprcMonitor, // pointer to monitor intersection rectangle
		LPARAM dwData       // data passed from EnumDisplayMonitors
		); 

	HBITMAP GetCaptureBmp(RECT rect); // 获取屏幕截图
	bool SaveBitmapToFile(HBITMAP hBitmap, LPCWSTR lpFileName);

private:
	static ScreenInfoController* screen_info_instance;
	VMONITORINFO m_MonitorInfoExArray[64];  
	std::map<std::wstring, VMONITORINFO> monitor_info_map; //显示器信息列表
	int m_iMontorNumber;
	std::wstring device_name;      //上次保存的显示器名称
	std::wstring config_file_name; 
	std::wstring config_file_path;
};