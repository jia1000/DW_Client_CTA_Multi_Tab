/*=========================================================================

  Program:   ImagingEngine
  Module:    utf8_string.h
  author: 	 zhangjian
  Brief:	 提供utf8/unicode/string/wstring等字符串类型之间的转换方法

=========================================================================*/
#pragma once
#ifndef UTF8_STRING_H
#define UTF8_STRING_H

#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <string>
// Windows 头文件: 
#include <windows.h>
// C 运行时头文件
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

std::string StringToUtf8(const std::string& str);

std::string Utf8ToString(const std::string& str);

std::wstring Utf8ToUnicode(const std::string& strUTF8);

std::string UnicodeToUtf8(const std::wstring &strUnicode);

std::wstring StringToWString(const std::string& str);

std::string WStringToString(const std::wstring &wstr);

#endif