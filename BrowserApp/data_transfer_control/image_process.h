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
#define JSON_KEY_IMAGE_SEQUENCE				"image_sequence"
#define JSON_KEY_IMAGE_MAX					"image_max"



//  第一级 "请求类型"    的枚举
#define JSON_VALUE_REQUEST_TYPE_MPR			"mpr"
#define JSON_VALUE_REQUEST_TYPE_VR			"vr"
#define JSON_VALUE_REQUEST_TYPE_MIP			"mip"
#define JSON_VALUE_REQUEST_TYPE_CPR			"cpr"
#define JSON_VALUE_REQUEST_TYPE_TIME		"time"


//  第二级 "操作操作类型"的枚举
#define JSON_VALUE_IMAGE_OPERATION_ZOOM		"zoom"
#define JSON_VALUE_IMAGE_OPERATION_ROTATE	"rotate"
#define JSON_VALUE_IMAGE_OPERATION_MOVE		"move"
#define JSON_VALUE_IMAGE_OPERATION_SKIP		"skip"

class ImageProcessBase
{
public:
	ImageProcessBase(std::string str_paras);
	virtual ~ImageProcessBase();

	//void SetKey1_RequestType(std::string str_req_type);
	void SetKey2_ImageOperation(std::string str_opertation);
	void SetKey3_ImageOperationParas(std::string str_paras);

	virtual bool Excute(std::string& out_image_data);
	virtual void SetFileName(std::string file_name) ;

protected:	
	// opencv Mat和base64的互转
	std::string base64Decode(const char* Data, int DataByte);
	std::string base64Encode(const unsigned char* Data, int DataByte);
	std::string Mat2Base64(const cv::Mat &img, std::string imgType);
	cv::Mat Base2Mat(std::string &base64_data);
	bool SaveBitmapToFile(HBITMAP hBitmap, LPCWSTR lpFileName);

	//int req_type;
	std::string m_key1_str_req_type;	// 请求类型，如MPR VR CPR等
	std::string m_key2_str_opertation;	// 不同图像操作类型，如， zoom rotate move等
	std::string m_key3_str_paras;		// 不同图像操作类型的参数，参数含义会有不同。具体需要见产品设计
	/// 窗口名称
	std::string m_wnd_name;

	std::string m_file_name;// 用于和web交互时，需要保存的dcm文件的名称
};

//////////////////////////////////////////////////////////////////////////
// MPR 请求类型的处理
class ImageMPRProcess : public ImageProcessBase
{
public:
	ImageMPRProcess(std::string str_paras);
	~ImageMPRProcess();
	virtual bool Excute(std::string& out_image_data); // 图像缩放后数据，base64编码

private:
	//GNC::GCS::Ptr<GNC::GCS::IStreamingLoader>         Loader;	
};

//////////////////////////////////////////////////////////////////////////
// VR 请求类型的处理
class ImageVRProcess : public ImageProcessBase
{
public:
	ImageVRProcess(std::string str_paras);
	~ImageVRProcess();
	virtual bool Excute(std::string& out_image_data); // 图像缩放后数据，base64编码

private:
	//GNC::GCS::Ptr<GNC::GCS::IStreamingLoader>         Loader;	
};

//////////////////////////////////////////////////////////////////////////
// CPR 请求类型的处理
class ImageCPRProcess : public ImageProcessBase
{
public:
	ImageCPRProcess(std::string str_paras);
	~ImageCPRProcess();
	virtual bool Excute(std::string& out_image_data); // 图像缩放后数据，base64编码

private:
	std::string curve_id_;
};

//////////////////////////////////////////////////////////////////////////
// 直接读取dcm文件，和web端联调
class ImageDcmProcess : public ImageProcessBase
{
public:
	ImageDcmProcess(std::string str_paras);
	~ImageDcmProcess();
	virtual bool Excute(std::string& out_image_data); // 图像缩放后数据，base64编码

private:
	//GNC::GCS::Ptr<GNC::GCS::IStreamingLoader>         Loader;	
};
//////////////////////////////////////////////////////////////////////////
// 和web端联调时，web传入文件流，客户端写入保存
class ImageWriteProcess : public ImageProcessBase
{
public:
	ImageWriteProcess(std::string str_paras);
	~ImageWriteProcess();
	virtual bool Excute(std::string& out_image_data); // 图像缩放后数据，base64编码

	bool SaveFile(char * src);
private:
	//GNC::GCS::Ptr<GNC::GCS::IStreamingLoader>         Loader;	
};
//////////////////////////////////////////////////////////////////////////
// 和web端联调时，web请求，客户端读取保存，传给web
class ImageReadDcmProcess : public ImageProcessBase
{
public:
	ImageReadDcmProcess(std::string str_paras);
	~ImageReadDcmProcess();
	virtual bool Excute(std::string& out_image_data); // 图像缩放后数据，base64编码

private:
	//GNC::GCS::Ptr<GNC::GCS::IStreamingLoader>         Loader;	
};
//////////////////////////////////////////////////////////////////////////
// 和web端联调时，web请求，客户端清除内存
class ImageClearFileDcmProcess : public ImageProcessBase
{
public:
	ImageClearFileDcmProcess(std::string str_paras);
	~ImageClearFileDcmProcess();
	virtual bool Excute(std::string& out_image_data); // 图像缩放后数据，base64编码

private:
	//GNC::GCS::Ptr<GNC::GCS::IStreamingLoader>         Loader;	
};