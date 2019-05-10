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

#include "io/dcm_reader.h"

#define JSON_KEY_REQUEST_TYPE				"request_type"
#define JSON_KEY_IMAGE_OPERATION			"image_operation"
#define JSON_KEY_IMAGE_PARAS				"image_paras"
#define JSON_KEY_IMAGE_DATA					"image_data"

//  第一级 "请求类型"    的枚举
#define JSON_VALUE_REQUEST_TYPE_MPR			"mpr"
#define JSON_VALUE_REQUEST_TYPE_VR			"vr"
#define JSON_VALUE_REQUEST_TYPE_MIP			"mip"
#define JSON_VALUE_REQUEST_TYPE_CPR			"cpr"

//  第二级 "操作操作类型"的枚举
#define JSON_VALUE_IMAGE_OPERATION_ZOOM		"zoom"
#define JSON_VALUE_IMAGE_OPERATION_ROTATE	"rotate"
#define JSON_VALUE_IMAGE_OPERATION_MOVE		"move"
#define JSON_VALUE_IMAGE_OPERATION_SKIP		"skip"

class ImageProcessBase
{
public:
	ImageProcessBase(std::string str_paras, std::string& in_image_data);
	virtual ~ImageProcessBase();

	void SetKey1_RequestType(std::string str_req_type);
	void SetKey2_ImageOperation(std::string str_opertation);
	void SetKey3_ImageOperationParas(std::string str_paras);
	void SetKey4_InImageData(std::string& in_image_data) ;

	virtual bool Excute(std::string& out_image_data);

protected:	
	// opencv Mat和base64的互转
	std::string base64Decode(const char* Data, int DataByte);
	std::string base64Encode(const unsigned char* Data, int DataByte);
	std::string Mat2Base64(const cv::Mat &img, std::string imgType);
	cv::Mat Base2Mat(std::string &base64_data);
	bool SaveBitmapToFile(HBITMAP hBitmap, LPCWSTR lpFileName);

	int req_type;
	std::string m_str_req_type;		// 请求类型，如MPR VR CPR等
	std::string m_in_image_data;	// 图像原始数据，base64编码
	std::string m_str_opertation;	// 不同图像操作类型，如， zoom rotate move等
	std::string m_str_paras;		// 不同图像操作类型的参数，参数含义会有不同。具体需要见产品设计
};

//////////////////////////////////////////////////////////////////////////
class ImageZoomProcess : public ImageProcessBase
{
public:
	ImageZoomProcess(std::string str_paras, std::string& in_image_data);
	~ImageZoomProcess();

	virtual bool Excute(std::string& out_image_data); // 图像缩放后数据，base64编码
};

//////////////////////////////////////////////////////////////////////////
class ImageRotateProcess : public ImageProcessBase
{
public:
	ImageRotateProcess(std::string str_paras, std::string& in_image_data);
	~ImageRotateProcess();

	virtual bool Excute(std::string& out_image_data); // 图像缩放后数据，base64编码
};

//////////////////////////////////////////////////////////////////////////
// 平移，大小不变
class ImageMoveProcess1 : public ImageProcessBase
{
public:
	ImageMoveProcess1(std::string str_paras, std::string& in_image_data);
	~ImageMoveProcess1();

	virtual bool Excute(std::string& out_image_data); // 图像缩放后数据，base64编码
};

//////////////////////////////////////////////////////////////////////////
// 平移，但会改变图像大小
class ImageMoveProcess2 : public ImageProcessBase
{
public:
	ImageMoveProcess2(std::string str_paras, std::string& in_image_data);
	~ImageMoveProcess2();

	virtual bool Excute(std::string& out_image_data); // 图像缩放后数据，base64编码

private:
};

//////////////////////////////////////////////////////////////////////////
// MPR 请求类型的处理
class ImageMPRProcess : public ImageProcessBase
{
public:
	ImageMPRProcess(std::string str_paras, std::string& in_image_data);
	~ImageMPRProcess();
	virtual bool Excute(std::string& out_image_data); // 图像缩放后数据，base64编码

private:

	//GNC::GCS::Ptr<GNC::GCS::IStreamingLoader>         Loader;
	/// 窗口名称
	std::string wnd_mpr1_;
};

//////////////////////////////////////////////////////////////////////////
// VR 请求类型的处理
class ImageVRProcess : public ImageProcessBase
{
public:
	ImageVRProcess(std::string str_paras, std::string& in_image_data);
	~ImageVRProcess();
	virtual bool Excute(std::string& out_image_data); // 图像缩放后数据，base64编码

private:

	//GNC::GCS::Ptr<GNC::GCS::IStreamingLoader>         Loader;
	/// 窗口名称
	std::string wnd_vr_;
};