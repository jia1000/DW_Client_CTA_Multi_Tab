/****************************************************************************************************

 * \file image_process.h
 * \date 2019/05/06 11:18
 *
 * \author jiayf
 * Contact: @deepwise.com
 *
 * \brief  

 Copyright(c) 2018-2030 DeepWise Corporation

****************************************************************************************************/

#pragma once

//C++输入输出库头文件
#include "opencv2/core/core.hpp"    
#include "opencv2/highgui/highgui.hpp"  
#include "opencv2/imgproc/imgproc.hpp" 
#include "opencv2/opencv.hpp"
#include <iostream> 

class ImageProcessBase
{
public:
	ImageProcessBase();
	~ImageProcessBase();

	virtual bool Excute(std::string& out_image_data);

protected:	
	// opencv Mat和base64的互转
	static std::string base64Decode(const char* Data, int DataByte);
	static std::string base64Encode(const unsigned char* Data, int DataByte);
	std::string Mat2Base64(const cv::Mat &img, std::string imgType);
	cv::Mat Base2Mat(std::string &base64_data);
};

//////////////////////////////////////////////////////////////////////////
class ImageZoomProcess : public ImageProcessBase
{
public:
	ImageZoomProcess(std::string str_rate, std::string& in_image_data);
	~ImageZoomProcess();

	virtual bool Excute(std::string& out_image_data); // 图像缩放后数据，base64编码

private:
	std::string m_str_rate;			// 缩放倍率
	std::string m_in_image_data;	// 图像原始数据，base64编码
};

//////////////////////////////////////////////////////////////////////////
class ImageRotateProcess : public ImageProcessBase
{
public:
	ImageRotateProcess(std::string str_angel, std::string& in_image_data);
	~ImageRotateProcess();

	virtual bool Excute(std::string& out_image_data); // 图像缩放后数据，base64编码

private:
	std::string m_str_angel;		// 旋转角度
	std::string m_in_image_data;	// 图像原始数据，base64编码
};

//////////////////////////////////////////////////////////////////////////
// 平移，大小不变
class ImageMoveProcess1 : public ImageProcessBase
{
public:
	ImageMoveProcess1(std::string str_move_position, std::string& in_image_data);
	~ImageMoveProcess1();

	virtual bool Excute(std::string& out_image_data); // 图像缩放后数据，base64编码

private:
	std::string m_str_move_position;// 移动像素点
	std::string m_in_image_data;	// 图像原始数据，base64编码
};

//////////////////////////////////////////////////////////////////////////
// 平移，但会改变图像大小
class ImageMoveProcess2 : public ImageProcessBase
{
public:
	ImageMoveProcess2(std::string str_move_position, std::string& in_image_data);
	~ImageMoveProcess2();

	virtual bool Excute(std::string& out_image_data); // 图像缩放后数据，base64编码

private:
	std::string m_str_move_position;// 移动像素点
	std::string m_in_image_data;	// 图像原始数据，base64编码
};