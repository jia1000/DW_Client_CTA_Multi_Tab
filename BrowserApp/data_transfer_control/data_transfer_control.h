
/****************************************************************************************************

 * \file data_transfer_control.h
 * \date 2019/05/05 10:33
 *
 * \author jiayf
 * Contact: @deepwise.com
 *
 * \brief : c++和js 数据交互管理 

 Copyright(c) 2018-2030 DeepWise Corporation

****************************************************************************************************/

#pragma once

//C++输入输出库头文件
#include "opencv2/core/core.hpp"    
#include "opencv2/highgui/highgui.hpp"  
#include "opencv2/imgproc/imgproc.hpp" 
#include "opencv2/opencv.hpp"
#include <iostream> 

class DataTransferController
{
public:
	~DataTransferController();

	static DataTransferController* GetInstance();

	bool ParseImageOperationData(std::string json_data, std::string& js_data);



private:
	DataTransferController();
	static DataTransferController* instance;

	bool ImageZoom(std::string str_rate, std::string& in_image_data, std::string& out_image_data);
	//逆时针旋转图像degree角度（原尺寸）
	bool ImageRotate1(std::string str_angel, std::string& in_image_data, std::string& out_image_data);
	//旋转图像内容不变，尺寸相应变大
	bool ImageRotate2(std::string str_angel, std::string& in_image_data, std::string& out_image_data);

	// 平移，大小不变
	bool ImageMove1(std::string str_move_position, std::string& in_image_data, std::string& out_image_data);
	// 平移，但会改变图像大小
	bool ImageMove2(std::string str_move_position, std::string& in_image_data, std::string& out_image_data);


	// opencv Mat和base64的互转
	static std::string base64Decode(const char* Data, int DataByte);
	static std::string base64Encode(const unsigned char* Data, int DataByte);
	static std::string Mat2Base64(const cv::Mat &img, std::string imgType);
	static cv::Mat Base2Mat(std::string &base64_data);
};
