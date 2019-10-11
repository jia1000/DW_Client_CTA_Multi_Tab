#pragma once
#include <iostream>
#include <string>
#include <functional>
#include <algorithm>
#include <vector>
inline std::wstring& ltrim(std::wstring &ss)
{
	int(*pf)(int) = isspace;
	std::wstring::iterator   p = find_if(ss.begin(), ss.end(), std::not1(std::ptr_fun(pf)));
	ss.erase(ss.begin(), p);
	return   ss;
}
inline std::wstring& rtrim(std::wstring &ss)
{
	int(*pf)(int) = isspace;
	std::wstring::reverse_iterator   p = find_if(ss.rbegin(), ss.rend(), std::not1(std::ptr_fun(pf)));
	ss.erase(p.base(), ss.end());
	return   ss;
}
inline std::wstring& trim(std::wstring &st)
{
	ltrim(rtrim(st));
	return   st;
}

inline std::vector<std::wstring> Split(const std::wstring &str, const std::wstring &pattern)
{
	std::vector<std::wstring> resVec;

	if (str.empty())
	{
		return resVec;
	}
	//方便截取最后一段数据
	std::wstring strs = str + pattern;

	size_t pos = strs.find(pattern);
	size_t size = strs.size();

	while (pos != std::string::npos)
	{
		std::wstring x = strs.substr(0, pos);
		resVec.push_back(x);
		strs = strs.substr(pos + 1, size);
		pos = strs.find(pattern);
	}

	return resVec;
}

inline std::vector<std::string> SplitString(const std::string &str, const std::string &pattern)
{
	std::vector<std::string> resVec;

	if (str.empty())
	{
		return resVec;
	}
	//方便截取最后一段数据
	std::string strs = str + pattern;

	size_t pos = strs.find(pattern);
	size_t size = strs.size();

	while (pos != std::string::npos)
	{
		std::string x = strs.substr(0, pos);
		if (!x.empty()) {
			resVec.push_back(x);
		}
		strs = strs.substr(pos + 1, size);
		pos = strs.find(pattern);
	}

	return resVec;
}