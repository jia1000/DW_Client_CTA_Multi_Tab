#include "StdAfx.h"

#include "EncryptUtil.h"
#include "AES.h"
#include "Base64.h"

//////////////////////////////////////////////////////////////////////////
// 解码解密流程
//	1.传入密文字符串(content)
//	2.去除字符串中的换行符
//	3.base64_decode解码
//	4.AES::CBC模式解密
//	5.去掉AES::PKCS5Padding填充
//	6.使用UTF-8编码字符串
string EncryptDecryptUtils::ExecDecodeDecrypt(const string& content)
{   
	//1.去掉字符串中的\r\n换行符
	string noWrapContent = FormatText(content);
	//2. Base64解码
	string strData = base64_decode(noWrapContent);
	size_t length = strData.length();

	//3. new些数组，给解密用
	char *szDataIn = new char[length + 1];
	::memcpy(szDataIn, strData.c_str(), length + 1);
	char *szDataOut = new char[length + 1];
	::memcpy(szDataOut, strData.c_str(), length + 1);

	//4. 进行AES的CBC模式解密
	AES aes;
	//在这里传入密钥，和偏移量，以及指定密钥长度和iv长度，如果你的密钥长度不是16字节128bit，那么需要在这里传入相对应的参数。
	aes.MakeKey(string(AES_KEY).c_str(), string(AES_IV).c_str(), 16, 16);
	//这里参数有传入指定加解密的模式，AES::CBC，如果你不是这个模式，需要传入相对应的模式，源码中都有注释说明
	aes.Decrypt(szDataIn, szDataOut, length, AES::CBC);

	//5.去PKCS5Padding填充:解密后需要将字符串中填充的去掉，根据填充规则进行去除，感兴趣可去搜索相关的填充规则
	if (0x00 < szDataOut[length - 1] <= 0x16)
	{
		int tmp = szDataOut[length - 1];
		for (int i = length - 1; i >= length - tmp; i--)
		{
			if (szDataOut[i] != tmp)
			{
				::memset(szDataOut, 0, length);
				break;
			}
			else
				szDataOut[i] = 0;
		}
	}

	//6. 将二进制的明文串转成UTF-8格式的编码方式，输出
	string srcDest = Utf8ToString(szDataOut);
	//string srcDest = szDataOut;
	delete[] szDataIn;
	delete[] szDataOut;
	return srcDest;
}

// 加密编码流程
//	1.传入明文字符串(content)
//	2.使用Utf-8解码字符串
//	3.AES::PKCS5Padding 填充
//	4.AES::CBC模式加密
//	5.base64_encode编码
string EncryptDecryptUtils::ExecEncryptEncode(const string& content)
{
	//1. 先获取UTF-8解码后的二进制串
	string utf8Content = StringToUtf8(content);
	size_t length = utf8Content.length();
	int block_num = length / BLOCK_SIZE + 1;

	//2. new 些数组供加解密使用
	char* szDataIn = new char[block_num * BLOCK_SIZE + 1];
	::memset(szDataIn, 0x00, block_num * BLOCK_SIZE + 1);
	strcpy(szDataIn, utf8Content.c_str());

	//3. 进行PKCS5Padding填充：进行CBC模式加密前，需要填充明文串，确保可以分组后各组都有相同的大小。
	// BLOCK_SIZE是在AES.h中定义的常量，PKCS5Padding 和 PKCS7Padding 的区别就是这个 BLOCK_SIZE 的大小，我用的PKCS5Padding，所以定义成 8。如果你是使用 PKCS7Padding，那么就根据你服务端具体大小是在 1-255中的哪个值修改即可。
	int k = length % BLOCK_SIZE;
	int j = length / BLOCK_SIZE;
	int padding = BLOCK_SIZE - k;
	for (int i = 0; i < padding; i++)
	{
		szDataIn[j * BLOCK_SIZE + k + i] = padding;
	}
	szDataIn[block_num * BLOCK_SIZE] = '\0';

	char* szDataOut = new char[block_num * BLOCK_SIZE + 1];
	::memset(szDataOut, 0, block_num * BLOCK_SIZE + 1);

	//4. 进行AES的CBC模式加密
	AES aes;
	//在这里传入密钥，和偏移量，以及指定密钥长度和iv长度，如果你的密钥长度不是16字节128bit，那么需要在这里传入相对应的参数。
	aes.MakeKey(string(AES_KEY).c_str(), string(AES_IV).c_str(), 16, 16);
	//这里参数有传入指定加解密的模式，AES::CBC，如果你不是这个模式，需要传入相对应的模式，源码中都有注释说明
	aes.Encrypt(szDataIn, szDataOut, block_num * BLOCK_SIZE, AES::CBC);

	//5. Base64编码
	string str = base64_encode((unsigned char*)szDataOut, block_num * BLOCK_SIZE);
	delete[] szDataIn;
	delete[] szDataOut;
	return str;
}
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// Base64解码流程
//	1.传入密文字符串(content)
//	2.base64_decode解码
//	3.使用UTF-8编码字符串
string EncryptDecryptUtils::ExecDecode(const string& content)
{
	string noWrapContent = FormatText(content);
	std::string decodedStr = base64_decode(noWrapContent);
	string srcDest = Utf8ToString(decodedStr);
	return srcDest;
}

// Base64编码流程
//	1.传入明文字符串(content)
//	2.使用Utf-8解码字符串
//	3.base64_encode编码
string EncryptDecryptUtils::ExecEncode(const string& content)
{
	string utf8Content = StringToUtf8(content);
	unsigned char* data = (unsigned char*)utf8Content.c_str();
	size_t len = utf8Content.length();
	std::string encodedStr = base64_encode(data, len);

	return encodedStr;
}
//////////////////////////////////////////////////////////////////////////


//去除字符串中的空格、换行符
string EncryptDecryptUtils::FormatText(const string& src)
{
	int len = src.length();
	char *dst = new char[len + 1];
	int i = -1, j = 0;
	while (src[++i])
	{
		switch (src[i])
		{
		case '\n':
		case '\t':
		case '\r':
			continue;
		}
		dst[j++] = src[i];
	}
	dst[j] = '\0';
	string rel = string(dst);
	delete dst;
	return rel;
}