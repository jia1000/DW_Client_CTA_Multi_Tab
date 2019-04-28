#include "tools/bmp_writer.h"
#include<iostream>

using namespace std;


bool SaveBmp1(char *fileName,void *imgBuffer, int imWidth, int imHeight)
{
	if (!imgBuffer)
	{
		return 0;
	}

	int biBitCount = 32;
	int colorTablesize = 0;				// 1024;	//灰度图像颜色表
	int lineByte = (imWidth * biBitCount / 8 + 3)/ 4 * 4;
	FILE *fp = fopen(fileName, "wb");

	if (!fp)
	{
		return 0 ;
	}

	LBITMAPFILEHEADER filehead;
	filehead.bfType = 0x4D42;
	filehead.bfSize = sizeof(LBITMAPFILEHEADER) + sizeof(LBITMAPINFOHEADER) + colorTablesize + lineByte * imHeight;
	filehead.bfReserved1 = 0;
	filehead.bfReserved2 = 0;
	filehead.bfOffBits = 54 + colorTablesize;

	//写位图文件头进文件
	fwrite(&filehead , sizeof(LBITMAPFILEHEADER), 1, fp);

    //申请位图文件信息头结构变量， 填写文件信息头信息
	LBITMAPINFOHEADER infoHead;
	infoHead.biBitCount = biBitCount;
	infoHead.biClrImportant = 0;
	infoHead.biClrUsed = 0;
	infoHead.biSize = 40;
	infoHead.biWidth = imWidth;
	infoHead.biHeight = imHeight;
	infoHead.biPlanes = 1;
	infoHead.biCompression = 0;
	infoHead.biSizeImage = lineByte * imHeight;
	infoHead.biXPelsPerMeter = 0;
	infoHead.biYPelsPerMeter = 0;

	fwrite(&infoHead, sizeof(LBITMAPINFOHEADER), 1, fp);
	//fwrite(&filehead, sizeof(LBITMAPFILEHEADER), 1, fp);

	LRGBQUAD *pColorTable = new LRGBQUAD[256];

	for (int i = 0 ; i < 256 ; i++)
	{
		pColorTable[i].rgbBlue = i;
		pColorTable[i].rgbGreen = i;
		pColorTable[i].rgbRed = i;
		//pColorTable[i].rgbReserved = 0;
	}
	fwrite(pColorTable, sizeof(LRGBQUAD), 256, fp);

	//写位图数据进文件
	fwrite(imgBuffer, imHeight*lineByte, 1, fp);
	fclose(fp);

	return 1;
}

// 存储bitmap图像，但是需要更改bmp图片存储顺序位BGRBGRBGR...
bool RmwWriteByteImg2BmpFile(const char *filename, BYTE *pImg, int width, int height, int bitcount)
{
	FILE * BinFile;
	BITMAPFILEHEADER FileHeader;
	BITMAPINFOHEADER BmpHeader;
	int extend=0;
	bool Suc = true;
	BYTE *pCur;
	BYTE* ex = nullptr;
	unsigned char biBitCount = bitcount;//24位真彩色

	extend = (width*(biBitCount/8) + 3) / 4 * 4 - width*(biBitCount / 8);//为了4字节对齐每行需要扩展的

	// Open File
	if ((BinFile = fopen(filename, "w+b")) == NULL) { return false; }
	//写入文件头
	FileHeader.bfType = ((WORD)('M' << 8) | 'B');
	FileHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);// +256 * 4L;//2个头结构后加调色板
	FileHeader.bfSize = FileHeader.bfOffBits + (width*(biBitCount / 8) + extend)*height;
	FileHeader.bfReserved1 = 0;
	FileHeader.bfReserved2 = 0;
	if (fwrite((void *)&FileHeader, 1, sizeof(FileHeader), BinFile) != sizeof(FileHeader)) Suc = false;
	
	// 写入信息头
	BmpHeader.biSize = 40;				//信息头大小，固定40
	BmpHeader.biWidth = width;
	BmpHeader.biHeight = -height;
	BmpHeader.biPlanes = 1;//固定值
	BmpHeader.biBitCount = biBitCount;	//像素位数 8/24/32;
	BmpHeader.biCompression = BI_RGB;	//BI_RGB不压缩
	BmpHeader.biSizeImage = 0;			//位图全部像素占用的字节数，BI_RGB时可设为0
	BmpHeader.biXPelsPerMeter = 0;		//水平分辨率(像素/米)
	BmpHeader.biYPelsPerMeter = 0;		//垂直分辨率(像素/米)
	BmpHeader.biClrUsed = 0;			//位图使用的颜色数,如果为0，则颜色数为2的biBitCount次方
	BmpHeader.biClrImportant = 0;		//重要的颜色数，0代表所有颜色都重要
	if (fwrite((void *)&BmpHeader, 1, sizeof(BmpHeader), BinFile) != sizeof(BmpHeader)) Suc = false;
	
	//// 写入调色板
	//for (i = 0, p[3] = 0; i<256; i++)
	//{
	//	p[0] = p[1] = p[2] = 255 - i; // blue,green,red;
	//	if (fwrite((void *)p, 1, 4, BinFile) != 4) { Suc = false; break; }
	//}

	if (extend)
	{
		ex = new BYTE[extend]; //填充数组大小为 0~3
		memset(ex, 0, extend);
	}

	//// 灰度值x0.5
	//for (pCur = pImg; pCur < pImg+ height*(width*(biBitCount / 8) + extend)-1; pCur++)
	//{
	//	*pCur = *pCur *0.5;
	//}
	//write data
	for (pCur = pImg + (height - 1)*(width*(biBitCount / 8) + extend); pCur >= pImg; pCur -= (width*(biBitCount / 8) + extend))
	{
		if (fwrite((void *)pCur, 1, width*(biBitCount / 8), BinFile) != (unsigned int)width) Suc = false; // 真实的数据
		if (extend) // 扩充的数据 这里填充0
			if (fwrite((void *)ex, 1, extend, BinFile) != 1) Suc = false;
	}



	// return;
	fclose(BinFile);
	if (extend)
		delete[] ex;
	return Suc;
}
