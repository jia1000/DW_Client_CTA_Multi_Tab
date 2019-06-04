
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

//#define USE_RAPID_JSON

class ImageProcessBase;

class DataTransferController
{
public:
	~DataTransferController();

	static DataTransferController* GetInstance();

	bool ParseImageOperationData(char* json_data, std::string& js_data);
#ifdef USE_RAPID_JSON
	bool ParseImageOperationDataUseRapidJson(char* json_data, std::string& js_data);
#endif



private:
	DataTransferController();
	static DataTransferController* instance;
	
	ImageProcessBase* image_process;
};
