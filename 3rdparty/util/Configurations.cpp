#include "StdAfx.h"
#include "Configurations.h"

//extern LoggerPtr logger;

#define SECTION_COMMON		TEXT("Common")
#define SECTION_USER_DATA	TEXT("DeepWiseCore")
#define STARTUPURL_KEY		TEXT("StartupUrl")
#define LOGLEVEL_KEY		TEXT("LogLevel")
#define ISAES_KEY			TEXT("UsingAES")
#define ISMAXIMIZED_KEY		TEXT("IsMaximized")
#define MAINWNDSIZE_KEY		TEXT("MainWndSize")
#define NEWWNDSIZE_KEY		TEXT("NewWndSize")
#define MARGIN_TYPE_KEY		TEXT("MarginType")
#define MARGIN_KEY			TEXT("Margin")
#define HEADER_FOOTER_KEY	TEXT("HeaderFooter")
#define RENDERER_LOG_KEY	TEXT("RenderLog")
#define BG_COLOR_KEY		TEXT("BackgroundColor")
#define ARCHIVE_FILES_KEY		TEXT("Archive")
#define BROWSER_ZOOM_INDEX_KEY	TEXT("BrowserZoomIndex")

Configurations::Configurations()
{
	m_zMainWndSize.Width = 1022;
	m_zMainWndSize.Height = 680;
	m_zNewWndSize.Width = 1022;
	m_zNewWndSize.Height = 680;
	m_bIsMainWndMaximize = 0;
	m_bIsPromptAlert = 0;
	margin_type_ = 0;
	margin_left_ = 10;
	margin_right_ = 10;
	margin_top_ = 10;
	margin_bottom_ = 10;
	header_footer_ = 0;
    browser_zoom_level_index_ = -1;
}

Configurations* Configurations::m_pInstance = 0;
Configurations* Configurations::getInstance()
{
	if(m_pInstance == 0)
	{
		m_pInstance = new Configurations;
	}
	return m_pInstance;
}

void Configurations::Initialize()
{
	// 程序根目录
	m_sRootDir = CPaintManagerUI::GetInstancePath();	
	WCHAR userdata_path[MAX_PATH] = {0};
	::GetEnvironmentVariable(L"APPDATA", userdata_path, MAX_PATH);
	user_data_dir_ = userdata_path + std::wstring(L"\\Dr. Wise Client\\");
	if (!PathFileExists(user_data_dir_.c_str())) {
		::CreateDirectory(user_data_dir_.c_str(), NULL);
	}
	//--------------------------------------------------------  
	//DWORD GetPrivateProfileString(  
	//                              LPCTSTR lpAppName,            // 节名  
	//                              LPCTSTR lpKeyName,            // 键名，读取该键的值  
	//                              LPCTSTR lpDefault,            // 若指定的键不存在，该值作为读取的默认值  
	//                              LPTSTR lpReturnedString,      // 一个指向缓冲区的指针，接收读取的字符串  
	//                              DWORD nSize,                  // 指定lpReturnedString指向的缓冲区的大小  
	//                              LPCTSTR lpFileName            // 读取信息的文件名。若该ini文件与程序在同一个目录下，  
	//                                                                也可使用相对路径,否则需要给出绝度路径  
	//UINT GetPrivateProfileInt(  
	//                              LPCTSTR lpAppName,            // 节名  
	//                              LPCTSTR lpKeyName,            // 键名，读取该键的值  
	//                              INT nDefault,                 // 若指定的键名不存在，该值作为读取的默认值  
	//                              LPCTSTR lpFileName            // 同上  
	//  
	//-------------------------------------------------------- 
	TCHAR szIniFilePath[MAX_PATH] = {0};
	wstring iniFilePath = m_sRootDir + TEXT("config.ini");
	wcscpy(szIniFilePath, iniFilePath.c_str());

	TCHAR startupUrlStr[MAX_PATH] = { 0 };
	TCHAR loglevelStr[MAX_PATH] = { 0 }; 
	TCHAR mainWndSizeStr[MAX_PATH] = { 0 };
	TCHAR newWndSizeStr[MAX_PATH] = { 0 };
	TCHAR margin_val[MAX_PATH] = { 0 };
	TCHAR background_color[MAX_PATH] = { 0 };
	TCHAR archive_file[MAX_PATH] = { 0 };

	//////////////////////////////////////////////////////////////////////////
	// 运行参数不需要程序修改，还保存在config.ini文件中 [1/8/2019 Modified by zhangjian]
	// 启动页地址
	::GetPrivateProfileString(SECTION_COMMON, STARTUPURL_KEY, NULL, startupUrlStr, MAX_PATH, szIniFilePath);
	m_sStartupUrl = startupUrlStr;
	// 日志级别
	::GetPrivateProfileString(SECTION_COMMON, LOGLEVEL_KEY, NULL, loglevelStr, MAX_PATH, szIniFilePath);
	m_sLogLevel = loglevelStr;
	transform(m_sLogLevel.begin(),m_sLogLevel.end(),m_sLogLevel.begin(),::toupper);
	
	// 主窗口是否最大化，启动时优先级高于<主窗口尺寸>
	m_bIsMainWndMaximize = ::GetPrivateProfileInt(SECTION_COMMON, ISMAXIMIZED_KEY, 1, szIniFilePath);

	// 主窗口尺寸:: 格式为 "1022,680|1022,680" 或简写为 "1022,680"
	DWORD ret = ::GetPrivateProfileString(SECTION_COMMON, MAINWNDSIZE_KEY, NULL, mainWndSizeStr, MAX_PATH, szIniFilePath);
	if (ret){
		FormatStringParam(mainWndSizeStr, m_zMainWndMinSize, m_zMainWndMinSize);
	}
	// 新打开窗口尺寸:: 格式为 "1022,680|1022,680" 或简写为 "1022,680"
	ret = ::GetPrivateProfileString(SECTION_COMMON, NEWWNDSIZE_KEY, NULL, newWndSizeStr, MAX_PATH, szIniFilePath);
	if (ret){
		FormatStringParam(newWndSizeStr, m_zNewWndSize, m_zNewWndMinSize);
	}
	// renderer进程的日志是否输出
	// 每打开一个浏览器窗口，对应创建一个renderer进程
	renderer_log_enabled_ = ::GetPrivateProfileInt(SECTION_COMMON, RENDERER_LOG_KEY, 1, szIniFilePath);
	// 浏览器背景颜色，默认颜色#1E2124
	::GetPrivateProfileString(SECTION_COMMON, BG_COLOR_KEY, NULL, background_color, MAX_PATH, szIniFilePath);
	background_color_ = background_color;
	// 本地压缩包资源文件
	::GetPrivateProfileString(SECTION_COMMON, ARCHIVE_FILES_KEY, NULL, archive_file, MAX_PATH, szIniFilePath);
	FormatStringParam(archive_file, archive_files_);
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	// 打印相关参数写入用户目录 [1/8/2019 Modified by zhangjian]
	TCHAR user_file_path[MAX_PATH] = {0};
	wstring user_file = user_data_dir_ + L"DeepWiseUser.ini";
	wcscpy(user_file_path, user_file.c_str());

	// margin type
	margin_type_ = ::GetPrivateProfileInt(SECTION_USER_DATA, MARGIN_TYPE_KEY, 0, user_file_path);
	// margin
	ret = ::GetPrivateProfileString(SECTION_USER_DATA, MARGIN_KEY, NULL, margin_val, MAX_PATH, user_file_path);
	if (ret){
		vector<std::wstring> vec = Split(margin_val, _T(","));
		if (vec.size() > 0){
			int* margin_arr = new int[4];
			for (int i=0; i<4; ++i){
				if (vec.size() > i){
					wstringstream streamw_m(vec[i]);
					streamw_m >> margin_arr[i];
				}
				else{
					if (i == 1){
						margin_arr[i] = margin_arr[0];
					}
					else if (i == 2){
						margin_arr[i] = margin_arr[0];
					}
					else if (i == 3){
						margin_arr[i] = margin_arr[1];
					}
				}
			}
			margin_left_ = margin_arr[0];
			margin_right_ = margin_arr[1];
			margin_top_ = margin_arr[2];
			margin_bottom_ = margin_arr[3];
			delete margin_arr;
		}
	}
	// header footer
	header_footer_ = ::GetPrivateProfileInt(SECTION_USER_DATA, HEADER_FOOTER_KEY, 0, user_file_path);
	//////////////////////////////////////////////////////////////////////////
}

void Configurations::SetMarginType(int margin_type)
{
	if (margin_type_ != margin_type){
		margin_type_ = margin_type;
		SetIniValue(user_data_dir_ + L"DeepWiseUser.ini", SECTION_USER_DATA, MARGIN_TYPE_KEY, to_wstring(margin_type));
	}
}

void Configurations::SetMargin(int left, int right, int top, int bottom)
{
	if (margin_left_ != left ||
		margin_right_ != right ||
		margin_top_ != top ||
		margin_bottom_ != bottom){
			margin_left_ = left;
			margin_right_ = right;
			margin_top_ = top;
			margin_bottom_ = bottom;

			wstring margin_val = to_wstring(margin_left_) + L"," + to_wstring(margin_right_) + L"," + to_wstring(margin_top_) + L"," + to_wstring(margin_bottom_);
			SetIniValue(user_data_dir_ + L"DeepWiseUser.ini", SECTION_USER_DATA, MARGIN_KEY, margin_val);
	}
}

void Configurations::SetHeaderEnabled(bool enabled)
{
	if (header_footer_ != enabled){
		header_footer_ = enabled;
		SetIniValue(user_data_dir_ + L"DeepWiseUser.ini", SECTION_USER_DATA, HEADER_FOOTER_KEY, to_wstring(header_footer_));
	}
}

int Configurations::GetMarginType()
{
	return margin_type_;
}

void Configurations::GetMargin(int& left, int& right, int& top, int& bottom)
{
	left = margin_left_;
	right = margin_right_;
	top = margin_top_;
	bottom = margin_bottom_;
}

bool Configurations::GetHeaderEnabled()
{
	return header_footer_;
}

bool Configurations::IsMainWndMaximize()
{
	return m_bIsMainWndMaximize > 0;
}

Size Configurations::GetMainWndSize()
{
	return m_zMainWndSize;
}

Size Configurations::GetMainWndMinSize()
{
	return m_zMainWndMinSize;
}

Size Configurations::GetNewWndSize()
{
	return m_zNewWndSize;
}

Size Configurations::GetNewWndMinSize()
{
	return m_zNewWndMinSize;
}

wstring Configurations::GetStartupUrl()
{
	return m_sStartupUrl;
}

wstring Configurations::GetRootDir()
{
	return m_sRootDir;
}

wstring Configurations::GetResourceDir()
{
	return m_sRootDir + _T("resources\\");
}

wstring Configurations::GetSkinDir()
{
	return m_sRootDir + _T("Skin\\");
}

wstring Configurations::GetTempDir()
{
	return user_data_dir_ + _T("temp\\");
}

wstring Configurations::GetUserDataDir()
{
	return user_data_dir_;
}

void Configurations::FormatStringParam(const wstring& paramStr, Size& defaultWndSize, Size& miniWndSize)
{
	if (paramStr.empty()) return;

	vector<std::wstring> vecPartSize = Split(paramStr, _T("|"));
	if (vecPartSize.size() == 0) return;

	// 第一部分: 默认窗口大小
	vector<std::wstring> vecSize = Split(vecPartSize[0], _T(","));
	if (vecSize.size() > 1){
		// 窗口默认大小
		wstringstream streamw(vecSize[0]);
		streamw >> defaultWndSize.Width;
		wstringstream streamh(vecSize[1]);
		streamh >> defaultWndSize.Height;
	}

	// 继续第二部分: 窗口最小尺寸
	if (vecPartSize.size() > 1){
		vecSize = Split(vecPartSize[0], _T(","));
		if (vecSize.size() > 1){
			// 窗口最小尺寸
			wstringstream streamw_m(vecSize[0]);
			streamw_m >> miniWndSize.Width;
			wstringstream streamh_m(vecSize[1]);
			streamh_m >> miniWndSize.Height;
		}
	}else{
		// 使用默认窗口大小
		miniWndSize.Width = defaultWndSize.Width;
		miniWndSize.Height = defaultWndSize.Height;
	}
}

void Configurations::SetStartupUrl(const wstring& url)
{
	m_sStartupUrl = url;

	SetIniValue(m_sRootDir + L"config.ini", SECTION_COMMON, STARTUPURL_KEY, url);
}

void Configurations::SetMainWndSize(Size size)
{
	m_zMainWndSize.Width = size.Width;
	m_zMainWndSize.Height = size.Height;

	wstring sizeStr;
	sizeStr = size.Width + _T(",") + size.Height;
	SetIniValue(m_sRootDir + L"config.ini", SECTION_COMMON, MAINWNDSIZE_KEY, sizeStr);
}

void Configurations::SetIniValue(const wstring& ini_file, const wstring& section, const wstring& key, const wstring& val)
{
	////--------------------------------------------------------  
	////BOOL WritePrivateProfileString(  
	////                              LPCTSTR lpAppName,      //节的名字，是一个以0结束的字符串  
	////                              LPCTSTR lpKeyName,      //键的名字，是一个以0结束的字符串。若为NULL，则删除整个节  
	////                              LPCTSTR lpString,       //键的值，是一个以0结束的字符串。若为NULL，则删除对应的键  
	////                              LPCTSTR lpFileName      //要写入的文件的文件名。若该ini文件与程序在同一个目录下，  
	////                              )                          也可使用相对路径,否则需要给出绝度路径。   
	::WritePrivateProfileString(section.c_str(), key.c_str(), val.c_str(), ini_file.c_str());
}

wstring Configurations::GetDefaultDownloadDir()
{
	TCHAR folder_path[MAX_PATH];
	std::wstring ret_path;

	// Save the file in the user's "My Documents" folder.
	if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_PERSONAL | CSIDL_FLAG_CREATE,
		NULL, 0, folder_path))) {
            ret_path = std::wstring(folder_path);
			//ret_path = CefString(folder_path);
			ret_path += _T("\\");
	}

	return ret_path;
}

wstring Configurations::GetLocalAppProtocol()
{
	return L"dpclient://";
}

bool Configurations::IsRendererLog()
{
	return renderer_log_enabled_;
}

void Configurations::SetProcessType(int type)
{
	current_process_type_ = type;
}

int Configurations::GetProcessType()
{
	return current_process_type_;
}

static const double level_arr[] = {0.5, 0.75, 0.8, 0.9, 1.0, 1.5, 2.0, 2.5, 3.0};

double Configurations::GetBrowserZoomLevelValue()
{
    double level = 0.0;    
    int arr_max = sizeof(level_arr) / sizeof(double);

	if (-1 == browser_zoom_level_index_) {
		GetBrowserZoomLevelIndex();
	}

    if (browser_zoom_level_index_ >= 0 && browser_zoom_level_index_ < arr_max) {
        return level_arr[browser_zoom_level_index_];
    }

    return level_arr[4];
}


int Configurations::GetBrowserZoomLevelIndex()
{
    TCHAR tc_level[MAX_PATH] = { 0 };
    TCHAR szIniFilePath[MAX_PATH] = {0};
    wstring iniFilePath = user_data_dir_ + L"DeepWiseUser.ini";
    wcscpy(szIniFilePath, iniFilePath.c_str());
    
    bool ret = GetPrivateProfileString(SECTION_USER_DATA, BROWSER_ZOOM_INDEX_KEY, NULL, tc_level, MAX_PATH, szIniFilePath);
    if (ret){
        int level_index = 0;
        std::wstring ws = tc_level;
        std::wstringstream wss;
        wss << ws;
        wss >> level_index;

        browser_zoom_level_index_ = level_index;
    }

    return browser_zoom_level_index_;
}

double Configurations::SetBrowserZoomLevelIndex(int level_index)
{
    int arr_max = sizeof(level_arr) / sizeof(double);
    if (level_index >= 0 && level_index < arr_max) {
        browser_zoom_level_index_ = level_index;
        std::wstring file_name = user_data_dir_ + L"DeepWiseUser.ini";
        std::wstring ws_index = to_wstring(level_index);
        ::WritePrivateProfileString(SECTION_USER_DATA, BROWSER_ZOOM_INDEX_KEY, ws_index.c_str(), file_name.c_str());
    }    

    double value = GetBrowserZoomLevelValue();
    return value;
}

wstring Configurations::GetBackgroundColor()
{
	return background_color_;
}
//解析本地压缩包资源文件配置
void Configurations::FormatStringParam(const wstring& paramStr, map<wstring, wstring>& files)
{
	if (paramStr.empty()) return;

	vector<std::wstring> vecPartFiles = Split(paramStr, _T("|"));
	if (vecPartFiles.size() == 0) return;

	for (auto it = vecPartFiles.begin(); it != vecPartFiles.end(); it ++) {
        vector<std::wstring> vec = Split(*it, _T(","));
		if (vec.size() == 0) continue;

		wstring file_name = trim(vec[0]);
		wstring acc_key;
		if (vec.size() > 1 && trim(vec[1]).length() > 0){
			acc_key = trim(vec[1]);
		}
		else{
			acc_key = file_name;
		}
		//key用来作为url访问地址，如果重名则覆盖之前
		if (files.count(acc_key) > 0){
			files.erase(acc_key);
		}
		files.insert(pair<wstring, wstring>(acc_key, file_name));
		
    }
}

const map<wstring, wstring>& Configurations::GetArchiveFiles()
{
	return archive_files_;
}