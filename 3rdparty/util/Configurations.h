
#ifndef C_PARSE_INI_FILE_H_
#define C_PARSE_INI_FILE_H_

#include <fstream>
#include <iostream>
#include <string>
#include <map>
#include "StringUtil.h"

using namespace std;
#define COMMENT_CHAR '#'

class Configurations
{
public:
	static Configurations* getInstance();
	void Initialize();
	//程序根目录
	wstring GetRootDir();
	//资源目录，存放静态资源
	wstring GetResourceDir();
	//皮肤目录，存放皮肤相关的图标等资源
	wstring GetSkinDir();
	wstring GetStartupUrl();
	//日志输出级别
	wstring GetLogLevel() {return m_sLogLevel;}
	//默认下载对话框打开位置
	wstring GetDefaultDownloadDir();
	//自定义协议名称
	wstring GetLocalAppProtocol();
	//程序生成的临时文件
	wstring GetTempDir();
	//用户目录，存放运行中产生的所有配置和临时文件
	wstring GetUserDataDir();
	//打印相关设置参数
	int GetMarginType();
	void GetMargin(int& left, int& right, int& top, int& bottom);
	bool GetHeaderEnabled();
	//end
	//renderer进程日志输出开关
	bool IsRendererLog();

	bool IsMainWndMaximize();

	Size GetMainWndSize();
	Size GetMainWndMinSize();
	Size GetNewWndSize();
	Size GetNewWndMinSize();

	void SetStartupUrl(const wstring& url);
	void SetMainWndSize(Size size);
	//打印相关设置参数
	void SetMarginType(int margin_type);
	void SetMargin(int left, int right, int top, int bottom);
	void SetHeaderEnabled(bool enabled);
	//end
	//进程类型
	void SetProcessType(int type);
	int GetProcessType();
	//背景颜色
	wstring GetBackgroundColor();
	//本地web压缩包资源文件
	const map<wstring, wstring>& GetArchiveFiles();
    // 审阅界面浏览器的放大倍数
    double GetBrowserZoomLevelValue();
    int    GetBrowserZoomLevelIndex();
    double SetBrowserZoomLevelIndex(int index);
private:
	Configurations();
	static Configurations* m_pInstance;
	void SetIniValue(const wstring& ini_file, const wstring& section, const wstring& key, const wstring& val);
	void FormatStringParam(const wstring& paramStr, Size& defaultWndSize, Size& miniWndSize);
	void FormatStringParam(const wstring& paramStr, map<wstring, wstring>& files);
	
private:
	wstring m_sRootDir;
	wstring m_sStartupUrl;
	wstring m_sLogLevel;
	Size m_zMainWndSize;
	Size m_zMainWndMinSize;
	Size m_zNewWndSize;
	Size m_zNewWndMinSize;
	int m_bIsMainWndMaximize;
	int m_bIsPromptAlert;
	wstring user_data_dir_;
	//以下与打印相关
	int margin_type_;
	int margin_left_;
	int margin_right_;
	int margin_top_;
	int margin_bottom_;
	bool header_footer_;
	//是否开启renderer进程的日志
	bool renderer_log_enabled_;
	//当前进程的CEF类型
	int current_process_type_;
	//浏览器背景，CSS颜色格式，如#FFFFFF
	wstring background_color_;
	//本地压缩包资源文件：key作为url地址，value为磁盘文件名称
	map<wstring, wstring> archive_files_;
    // 审阅界面浏览器的放大倍数的索引
    int browser_zoom_level_index_;
};

#endif
