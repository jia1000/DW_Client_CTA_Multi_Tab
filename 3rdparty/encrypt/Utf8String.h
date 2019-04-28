#pragma once
#ifndef UTF8_STRING_H
#define UTF8_STRING_H

#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <string>

std::string StringToUtf8(const std::string& str);

std::string Utf8ToString(const std::string& str);

std::wstring Utf8ToUnicode(const std::string& strUTF8);

std::string UnicodeToUtf8(const std::wstring &strUnicode);

std::wstring StringToWString(const std::string& str);

std::string WStringToString(const std::wstring &wstr);

#endif