#pragma once

#include <string>

using namespace std;

#ifndef AES_INFO
#define AES_INFO

#define AES_KEY "zV4pLw69R3gGFqrM"		//AES 16B的密钥
#define AES_IV "vN2jMe7INLXFCRUE"		//AES CBC加解密模式所需的偏移量

#endif 

class EncryptDecryptUtils {
public:
	EncryptDecryptUtils(){};
	~EncryptDecryptUtils(){};
	//解码解密
	static string ExecDecodeDecrypt(const string& content);
	//加密编码
	static string ExecEncryptEncode(const string& content);
	//解码
	static string ExecDecode(const string& content);
	//编码
	static string ExecEncode(const string& content);
private:
	//去除字符串中的空格、换行符
	static string FormatText(const string& content);
};