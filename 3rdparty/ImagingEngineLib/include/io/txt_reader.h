///

#pragma once

#include <fstream>
#include <string>
#include <iostream>
#include <vector>

using namespace std;

inline vector<string> ReadTxt(const char *path)
{
	ifstream in(path);
	string filename;
	string line;
	vector<string> result;

	if (in) // 有该文件
	{
		while (getline(in, line)) // line中不包括每行的换行符
		{
			cout << line << endl;
			result.push_back(line);
		}
	}
	else // 没有该文件
	{
		cout << "no such file" << endl;
	}
	return result;
}