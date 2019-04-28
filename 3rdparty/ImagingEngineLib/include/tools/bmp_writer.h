/*=========================================================================

  Program:   ImagingEngine
  Module:    bmp_writer.h
  author: 	 zhangjian
  Brief:	 not in using

=========================================================================*/
#ifndef BMP_H
#define BMP_H 

#include "data_definition.h"

typedef unsigned char LBYTE;	//
typedef unsigned short LWORD;	//
typedef unsigned int LDWORD;
typedef long LLONG;				//
//

#pragma pack(2)

typedef struct
{
	LWORD bfType;
	LDWORD bfSize;
	LWORD bfReserved1;
	LWORD bfReserved2;
	LDWORD bfOffBits;
}LBITMAPFILEHEADER;
// #pragma pack(pop) 

typedef struct
{
	LDWORD biSize;
	LLONG biWidth;
	LLONG biHeight;
	LWORD biPlanes;
	LWORD biBitCount;
	LDWORD biCompression;
	LDWORD biSizeImage;
	LLONG biXPelsPerMeter;
	LLONG biYPelsPerMeter;
	LDWORD biClrUsed;
	LDWORD biClrImportant;
}LBITMAPINFOHEADER;

 
typedef struct
{
	LBYTE rgbBlue;
	LBYTE rgbGreen;
	LBYTE rgbRed;
	LBYTE rgbReserved;
}LRGBQUAD;
 

typedef struct
{
	unsigned char* buf;
	int width_x;
	int height_y;
	int deepth;
	LRGBQUAD* palette;

}LBmpbase;

 

//bool saveBmp(char*, unsigned char*, int, int, int, RGBQUAD*);
//Bmpbase readbmp(char*);
bool saveBmp(char*, unsigned char*, int, int, int, LRGBQUAD* ); 

bool SaveBmp1(char *fileName,void *imgBuffer, int imWidth, int imHeight); 

bool wDataToBitBmp(BYTE*, int imWidth, int imHeight, char *imName); 

bool RmwWriteByteImg2BmpFile(const char *filename, BYTE *pImg, int width, int height, int bitcount = 24);

// Bmpbasel readbmp(char*);
// Bmpbase imrotate(unsigned char*, int, int, int, RGBQUAD*, float);

#endif