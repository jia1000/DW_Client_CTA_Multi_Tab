#pragma once

//#include "StdAfx.h"
#include <stdarg.h>
#include <stdint.h>
#include <string> 
#include "Path.h"

#include <glog/logging.h>
#include <glog/log_severity.h> 

#include "encrypt/Utf8String.h"
using namespace std;

class CGLogger
{
public:
	CGLogger(void)
	{
	}

	~CGLogger(void)
	{
		//  here, cannot free the object.
		/*if (NULL != m_pInstance)
		{
			delete m_pInstance;
			m_pInstance = NULL;
		}*/
	}

	static void Info(string msg)
	{
		LOG(INFO) << msg;
	}

	static void Error(string msg, string ex = "")
	{
		if (ex.empty()){
			LOG(ERROR) << msg;
		}
		else{
			LOG(ERROR) << msg << " \\r\\nException:" + ex;
		}
	}

	static void Debug(string msg)
	{
#ifdef _DEBUG		
		LOG(WARNING) << msg;	
#else
		wstring logLevel = Configurations::getInstance()->GetLogLevel();		
		if (logLevel == _T("DEBUG")) {
			LOG(WARNING) << msg;
		}
#endif // _DEBUG
	}
	
	static void InitGLog(string procName)
	{
		google::InitGoogleLogging(procName.c_str());

		SYSTEMTIME st;
		::GetLocalTime(&st);
		wchar_t chYearMonth[1000] = { 0 };
		swprintf_s(chYearMonth, _T("%04d%02d"), st.wYear, st.wMonth);
		wchar_t chDay[1000] = { 0 };
		swprintf_s(chDay, _T("%02d"), st.wDay);

#ifdef DEBUG_MODE
		google::SetStderrLogging(google::GLOG_INFO);								//设置级别高于 google::INFO 的日志同时输出到屏幕
#else
		google::SetStderrLogging(google::GLOG_FATAL);								//设置级别高于 google::FATAL 的日志同时输出到屏幕
#endif
		// get log directory according to current date
		wstring logDir = _T("log\\") + wstring(chYearMonth) + _T("\\") + wstring(chDay) + _T("\\");
		// create log directory
		wstring logPath = Configurations::getInstance()->GetUserDataDir() + logDir;
		if (!PathFileExists(logPath.c_str())) {
			CreateMultipleDirectory(logPath.c_str());
		}

		FLAGS_colorlogtostderr = true;												//设置输出到屏幕的日志显示相应颜色
		google::SetLogDestination(google::GLOG_INFO, WStringToString(logPath).c_str());				//设置 google::INFO 级别的日志存储路径和文件名前缀
		google::SetLogDestination(google::GLOG_WARNING, WStringToString(logPath).c_str());				//设置 google::WARNING 级别的日志存储路径和文件名前缀
		google::SetLogDestination(google::GLOG_ERROR, WStringToString(logPath).c_str());				//设置 google::ERROR 级别的日志存储路径和文件名前缀
		google::SetLogDestination(google::GLOG_FATAL, WStringToString(logPath).c_str());				//设置 google::ERROR 级别的日志存储路径和文件名前缀
		FLAGS_logbufsecs =0;														//缓冲日志输出，默认为30秒，此处改为立即输出
		FLAGS_max_log_size =1;														//最大日志大小为 1MB
		FLAGS_stop_logging_if_full_disk = true;										//当磁盘被写满时，停止日志输出
		google::SetLogFilenameExtension(".log");									//设置文件名扩展，如平台？或其它需要区分的信息
//  		google::InstallFailureSignalHandler();										//捕捉 程序 dumped
//  		google::InstallFailureWriter(&SignalHandle);								//默认捕捉 SIGSEGV 信号信息输出会输出到 stderr，可以通过下面的方法自定义输出>方式：
	}

private:
	//将信息输出到LOG(FATAL)
	static void SignalHandle(const char* data, int size)
	{
		std::string str = std::string(data,size);
		/*
		std::ofstream fs("glog_dump.log",std::ios::app);
		fs<<str;
		fs.close();
		*/

		//也可以直接在这里发送邮件或短信通知，不过这个方法是被回调多次的（每次回调只输出一行错误信息，所以如上面的记录到文件，也需要>以追加模式方可），
		//所以这里发邮件或短信不是很适合，不过倒是可以调用一个 SHELL 或 PYTHON 脚本，而此脚本会先 sleep 3秒左右，然后将错
		//误信息通过邮件或短信发送出去，这样就不需要监控脚本定时高频率执行，浪费效率了。
	}

};