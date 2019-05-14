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
#include "tools/math.h"

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
	// 扩充图像边缘full
	int padding_val = 0;
	cv::Mat src_image;
	HBitmapToMat(in_image_data_, src_image);

	// 旋转图像
	double m[6];
	cv::Mat dst_image;
	//cv::rotate(src_image, dst_image, angle_);

	//// warpAffine默认的旋转方向是逆时针，所以加负号表示转化为顺时针
	//angle_ = -angle_;
	// 转换为弧度  
	double angle_radian = MathTool::DegreesToRadians(angle_);
	double a = sin(angle_radian), b = cos(angle_radian);
	int width = src_image.cols;
	int height = src_image.rows;
	int width_rotate = int(width * fabs(b) + height * fabs(a));
	int height_rotate = int(height * fabs(b) + width * fabs(a));

	if(width_rotate <= 20 || height_rotate <= 20){
		width_rotate = 20;
		height_rotate = 20;
	}

	// 旋转数组map
	// [ m0  m1  m2 ] ===>  [ A11  A12   b1 ]
	// [ m3  m4  m5 ] ===>  [ A21  A22   b2 ]
	float map[6];
	cv::Mat map_matrix = cv::Mat(2, 3, CV_32F, map);
	// 旋转中心
	CvPoint2D32f center = cvPoint2D32f(width / 2, height / 2);
	CvMat map_matrix2 = map_matrix;
	// 计算二维旋转的仿射变换矩阵
	cv2DRotationMatrix(center, angle_, 1.0, &map_matrix2);
	map[2] += (width_rotate - width) / 2;
	map[5] += (height_rotate - height) / 2;

	// Mat img_rotate;
	// 对图像做仿射变换
	// CV_WARP_FILL_OUTLIERS - 填充所有输出图像的象素。
	// 如果部分象素落在输入图像的边界外，那么它们的值设定为 fillval.
	// CV_WARP_INVERSE_MAP - 指定 map_matrix 是输出图像到输入图像的反变换，
	int chnnel = src_image.channels();
	if(chnnel == 3)
		warpAffine(src_image, dst_image, map_matrix, cv::Size(width_rotate, height_rotate), 1, 0, cv::Scalar(padding_val,padding_val,padding_val));
	else
		warpAffine(src_image, dst_image, map_matrix, cv::Size(width_rotate, height_rotate), 1, 0, padding_val);

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