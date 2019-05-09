#pragma once

#include <fstream>
#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <algorithm>
#include <xfunctional>
#include <sstream>
#include <atlstr.h>

using namespace std;

/// 模板函数：将string类型变量转换为常用的数值类型
template <class REAL>  
REAL StringToNumber(const string& str)  
{  
	istringstream iss(str);  
	REAL num;  
	iss >> num;  
	return num;      
}  

inline string& ltrim(string &ss)
{
	int(*pf)(int) = isspace;
	string::iterator   p = find_if(ss.begin(), ss.end(), not1(ptr_fun(pf)));
	ss.erase(ss.begin(), p);
	return   ss;
}

inline string& rtrim(string &ss)
{
	int(*pf)(int) = isspace;
	string::reverse_iterator   p = find_if(ss.rbegin(), ss.rend(), not1(ptr_fun(pf)));
	ss.erase(p.base(), ss.end());
	return   ss;
}

inline string& trim(string &st)
{
	ltrim(rtrim(st));
	return   st;
}

inline std::vector<std::string> Split(const std::string &str, const std::string &pattern)
{
	std::vector<std::string> resVec;

	if (str.empty())
	{
		return resVec;
	}
	/// 方便截取最后一段数据
	std::string strs = str + pattern;

	size_t pos = strs.find(pattern);
	size_t size = strs.size();

	while (pos != std::string::npos)
	{
		std::string x = strs.substr(0, pos);
		resVec.push_back(x);
		strs = strs.substr(pos + 1, size);
		pos = strs.find(pattern);
	}

	return resVec;
}

inline std::string GenerateGUID()
{
	char buf[64] = {0};
	GUID guid;
	_snprintf(buf, sizeof(buf)
		, "{%08X-%04X-%04x-%02X%02X-%02X%02X%02X%02X%02X%02X}"
		, guid.Data1
		, guid.Data2
		, guid.Data3
		, guid.Data4[0], guid.Data4[1]
	, guid.Data4[2], guid.Data4[3], guid.Data4[4], guid.Data4[5]
	, guid.Data4[6], guid.Data4[7]
	);
	return string(buf);  
}