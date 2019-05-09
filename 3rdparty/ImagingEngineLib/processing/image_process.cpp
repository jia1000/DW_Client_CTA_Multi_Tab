/*=========================================================================

  Program:   ImagingEngine
  Module:    image_process.cpp
  author: 	 zhangjian
  Brief:	 

=========================================================================*/
#include "processing/image_process.h"

#include <algorithm>
#include <fstream>
#include <io.h>

using namespace DW::CV;

ImageProcessBase::ImageProcessBase(HBITMAP& in_image_data, int width, int height, int bits)
	: in_image_data_(in_image_data)
	, image_width_(width)
	, image_height_(height)
	, bits_per_pixel_(bits)
{

}

ImageProcessBase::~ImageProcessBase()
{

}

void ImageProcessBase::Mat2Buffer(const cv::Mat &img, UNITDATASHOW *output) 
{
	// 一个像素的bits
	int nFlag = img.channels() * 8;
	int nHeight = img.rows;
	int nWidth = img.cols;
	// 图像总的字节
	int nBytes = nHeight * nWidth * nFlag / 8;

	if(output)
		delete output;

	// new的单位为字节
	output = new UNITDATASHOW[nBytes];
	// 转化函数,注意Mat的data成员
	memcpy(output, img.data, nBytes);
}

void ImageProcessBase::Buffer2Mat(const UNITDATASHOW *input, int width, int height, int bits, cv::Mat &img)
{
	if (input == NULL) return;

	int bytes = width * height * bits / 8;//字节计算
	int type = bits == 8 ? CV_8UC1 : CV_8UC3;

	img = cv::Mat::zeros(width, height, bits / 8);
	memcpy(img.data, input, bytes);



	//img = cv::Mat(height,width, , input).clone(); // make a copy
}

BOOL ImageProcessBase::HBitmapToMat(HBITMAP& _hBmp, cv::Mat& _mat)
{
	BITMAP bmp;    
	GetObject(_hBmp,sizeof(BITMAP),&bmp);    
	int nChannels = bmp.bmBitsPixel == 1 ? 1 : bmp.bmBitsPixel / 8 ;
	int cv_channel = nChannels == 4 ? CV_8UC4 : CV_8UC3;
	int depth = bmp.bmBitsPixel == 1 ? IPL_DEPTH_1U : IPL_DEPTH_8U;    
	cv::Mat v_mat;
	v_mat.create(cvSize(bmp.bmWidth, bmp.bmHeight), CV_MAKETYPE(cv_channel, nChannels));
	GetBitmapBits(_hBmp, bmp.bmHeight * bmp.bmWidth * nChannels, v_mat.data);  
	_mat=v_mat;
	return TRUE;   
}

BOOL ImageProcessBase::MatToHBitmap(cv::Mat& _mat, HBITMAP& _hBmp)
{
	//MAT类的TYPE=（nChannels-1+ CV_8U）<<3
	int nChannels = (_mat.type() >> 3) - CV_8U + 1;
	int cv_channel = nChannels == 4 ? CV_8UC4 : CV_8UC3;
	int iSize=_mat.cols * _mat.rows * nChannels;
	_hBmp = CreateBitmap(_mat.cols, _mat.rows, 1, nChannels * 8, _mat.data);
	return TRUE;
}

// hbitmap convert to IplImage   
IplImage* ImageProcessBase::HBitmapToIplImage(HBITMAP hBmp)   
{   
	BITMAP bmp;    
	GetObject(hBmp,sizeof(BITMAP),&bmp);    

	// get channels which equal 1 2 3 or 4    
	// bmBitsPixel :   
	// Specifies the number of bits    
	// required to indicate the color of a pixel.    
	int nChannels = bmp.bmBitsPixel == 1 ? 1 : bmp.bmBitsPixel/8 ;   

	// get depth color bitmap or grayscale   
	int depth = bmp.bmBitsPixel == 1 ? IPL_DEPTH_1U : IPL_DEPTH_8U;    


	// create header image   
	IplImage* img = cvCreateImage(cvSize(bmp.bmWidth,bmp.bmHeight),depth,nChannels);    

	// allocat memory for the pBuffer   
	BYTE *pBuffer = new BYTE[bmp.bmHeight*bmp.bmWidth*nChannels];    

	// copies the bitmap bits of a specified device-dependent bitmap into a buffer   
	GetBitmapBits(hBmp,bmp.bmHeight*bmp.bmWidth*nChannels,pBuffer);    

	// copy data to the imagedata   
	memcpy(img->imageData,pBuffer,bmp.bmHeight*bmp.bmWidth*nChannels);   
	delete pBuffer;    

	// create the image   
	IplImage *dst = cvCreateImage(cvGetSize(img),img->depth,3);   
	// convert color   
	cvCvtColor(img,dst,CV_BGRA2BGR);   
	cvReleaseImage(&img);   
	return dst;   
}


//////////////////////////////////////////////////////////////////////////
ImageZoomProcess::ImageZoomProcess(HBITMAP& in_image_data, int width, int height, int bits, float scale)
	: ImageProcessBase(in_image_data, width, height, bits)
	, scale_(scale)
{
}

ImageZoomProcess::~ImageZoomProcess()
{
}

bool ImageZoomProcess::Excute(HBITMAP& out_image_data)
{
	cv::Mat src_image;
	HBitmapToMat(in_image_data_, src_image);
	//cv::imwrite("C:\\ztest2\\a.png", src_image);
	
	// 缩放图像
	cv::Mat dst_image;
	cv::resize(src_image, dst_image, cv::Size(), scale_, scale_);

	MatToHBitmap(dst_image, out_image_data);
	//cv::imwrite("C:\\ztest2\\b.png", dst_image);
	
	return false;
}


//////////////////////////////////////////////////////////////////////////
ImageRotateProcess::ImageRotateProcess(HBITMAP& in_image_data, int width, int height, int bits, float angle)
	: ImageProcessBase(in_image_data, width, height, bits)
	, angle_(angle)
{
}

ImageRotateProcess::~ImageRotateProcess()
{
}

bool ImageRotateProcess::Excute(HBITMAP& out_image_data)
{
	cv::Mat src_image;
	HBitmapToMat(in_image_data_, src_image);

	int degree = (int)(angle_ * 10) % 3;

	// 旋转图像
	double m[6];
	cv::Mat dst_image;
	cv::rotate(src_image, dst_image, degree);

	MatToHBitmap(dst_image, out_image_data);

	return false;
}

//////////////////////////////////////////////////////////////////////////
ImageMoveProcess::ImageMoveProcess(HBITMAP& in_image_data, int width, int height, int bits, float dx, float dy)
	: ImageProcessBase(in_image_data, width, height, bits)
	, offset_x_(dx)
	, offset_y_(dy)
{

}

ImageMoveProcess::~ImageMoveProcess()
{

}

bool ImageMoveProcess::Excute(HBITMAP& out_image_data)
{
	cv::Mat src_image;
	HBitmapToMat(in_image_data_, src_image);
	
	int dx = offset_x_;
	int dy = offset_y_;

	const int rows = src_image.rows;
	const int cols = src_image.cols;

	// 平移图像
	cv::Mat dst_image;
	dst_image.create(rows, cols, src_image.type());

	cv::Vec3b *p;
	for (int i = 0; i < rows ; i++) {
		p = dst_image.ptr<cv::Vec3b>(i);
		for (int j = 0; j < cols ; j++) {
			//平移后坐标映射到原图像
			int x = j - dx;
			int y = i - dy;

			//保证映射后的坐标在原图像范围内
			if (x >=0 && y >= 0 && x < cols && y < rows) {
				p[j] = src_image.ptr<cv::Vec3b>(y)[x];
			}
		}		
	}

	MatToHBitmap(dst_image, out_image_data);

	return false;
}