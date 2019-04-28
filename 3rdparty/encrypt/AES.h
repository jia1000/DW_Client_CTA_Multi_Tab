
//AES.h

#ifndef _AES_H
#define _AES_H

#include <exception>
#include <cstring>
#include <string>

// 在PKCS5Padding中，明确定义Block的大小是8位
// 而在PKCS7Padding定义中，对于块的大小是不确定的，可以在1-255之间（块长度超出255的尚待研究），填充值的算法都是一样的
#define BLOCK_SIZE 8
using namespace std;

class AES
{
public:
	enum
	{
		ECB = 0, CBC = 1, CFB = 2
	};
private:
	enum
	{
		DEFAULT_BLOCK_SIZE = 16
	};
	enum
	{
		MAX_BLOCK_SIZE = 32, MAX_ROUNDS = 14, MAX_KC = 8, MAX_BC = 8
	};
public:
	AES();
	virtual ~AES();

private:
	//Key Initialization Flag
	bool m_bKeyInit;
	//Encryption (m_Ke) round key
	int m_Ke[MAX_ROUNDS + 1][MAX_BC];
	//Decryption (m_Kd) round key
	int m_Kd[MAX_ROUNDS + 1][MAX_BC];
	//Key Length
	int m_keylength;
	//Block Size
	int m_blockSize;
	//Number of Rounds
	int m_iROUNDS;
	//Chain Block
	char m_chain0[MAX_BLOCK_SIZE];
	char m_chain[MAX_BLOCK_SIZE];
	//Auxiliary private use buffers
	int tk[MAX_KC];
	int a[MAX_BC];
	int t[MAX_BC];
private:
	void Xor(char* buff, char const* chain);
	void DefEncryptBlock(char const* in, char* result);
	void DefDecryptBlock(char const* in, char* result);
	void EncryptBlock(char const* in, char* result);
	void DecryptBlock(char const* in, char* result);
public:
	void MakeKey(char const* key, char const* chain, int keylength = DEFAULT_BLOCK_SIZE, int blockSize = DEFAULT_BLOCK_SIZE);
	void Encrypt(char const* in, char* result, size_t n, int iMode = ECB);
	void Decrypt(char const* in, char* result, size_t n, int iMode = ECB);
};

#endif // __RIJNDAEL_H__