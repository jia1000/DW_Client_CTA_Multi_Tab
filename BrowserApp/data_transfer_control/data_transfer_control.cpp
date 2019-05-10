#include "stdafx.h"

#include "data_transfer_control.h"
#include "image_process.h"

#include <algorithm>

#include "json/json.h"
#include <fstream> // ifstream, ifstream::in
#include <io.h>

DataTransferController* DataTransferController::instance = nullptr;

DataTransferController* DataTransferController::GetInstance()
{
	if (instance == nullptr)
		instance = new DataTransferController();
	return instance;
}

DataTransferController::DataTransferController()
{
	image_process = NULL;
}

DataTransferController::~DataTransferController()
{
	instance = nullptr;
}

bool DataTransferController::ParseImageOperationData(std::string json_data, std::string& js_data)
{
	// 解析从浏览器发送过来的Json数据
	Json::Reader reader;
	Json::Value root;
	bool ret = reader.parse(json_data, root, false);
	if (!ret) {
		return false;
	}
	// 获得关键性的参数
	std::string key_name1("");
	std::string key_name2("");
	std::string key_name3("");
	std::string key_name4("");

	
	if (root[JSON_KEY_REQUEST_TYPE].isString()) {
		key_name1 = root[JSON_KEY_REQUEST_TYPE].asString();
		// 都转为小写字符
		std::transform(key_name1.begin(), key_name1.end(), key_name1.begin(), ::tolower);
	}
	if (root[JSON_KEY_IMAGE_OPERATION].isString()) {
		key_name2 = root[JSON_KEY_IMAGE_OPERATION].asString();
		// 都转为小写字符
		std::transform(key_name2.begin(), key_name2.end(), key_name2.begin(), ::tolower);
	}
	if (root[JSON_KEY_IMAGE_PARAS].isString()) {
		key_name3 = root[JSON_KEY_IMAGE_PARAS].asString();
		// 都转为小写字符
		std::transform(key_name3.begin(), key_name3.end(), key_name3.begin(), ::tolower);
	}
	if (root[JSON_KEY_IMAGE_DATA].isString()) {
		key_name4 = root[JSON_KEY_IMAGE_DATA].asString();
	}

	if(image_process) {
		delete image_process;
		image_process = NULL;
	}

	std::string out_image_data = "";

	if (key_name1 == JSON_VALUE_REQUEST_TYPE_MPR) {
		image_process = new ImageMPRProcess(key_name3, key_name4);
		image_process->SetKey2_ImageOperation(key_name2);
		image_process->Excute(out_image_data);
	} else if (key_name1 == JSON_VALUE_REQUEST_TYPE_VR) {
		image_process = new ImageVRProcess(key_name3, key_name4);
		image_process->SetKey2_ImageOperation(key_name2);
		image_process->Excute(out_image_data);
	} 
	else {
		if (key_name2 == JSON_VALUE_IMAGE_OPERATION_ZOOM) {	
		} else if (key_name2 == JSON_VALUE_IMAGE_OPERATION_ROTATE) {
		} else if (key_name2 == JSON_VALUE_IMAGE_OPERATION_MOVE) {
			static bool test_flag = true;
			if (test_flag) {
				ImageMoveProcess1 image_process(key_name3, key_name4);
				image_process.Excute(out_image_data);
			} else {
				ImageMoveProcess2 image_zoom(key_name3, key_name4);
				image_zoom.Excute(out_image_data);

			}
		} else if (key_name2 == JSON_VALUE_IMAGE_OPERATION_SKIP) {

		}
	}

	// 模拟再发送给浏览器
	Json::FastWriter writer;
	Json::Value inputjson;
	inputjson[JSON_KEY_REQUEST_TYPE]	= key_name1;
	inputjson[JSON_KEY_IMAGE_OPERATION] = key_name2;
	inputjson[JSON_KEY_IMAGE_PARAS]		= key_name3;
	inputjson[JSON_KEY_IMAGE_DATA]		= out_image_data;


	std::string jsonstr = writer.write(inputjson);

	// 有换行符的json字符串， JS不能处理。
	if (*jsonstr.rbegin() == '\n') {
		jsonstr.erase(jsonstr.end() - 1);
	}

	std::string text("jsSendCount('");
	std::string postfix("')");
	text += jsonstr;
	text += postfix;
	//frame_->ExecuteJavaScript(text.c_str(), "", 0);
	//js_data = text;

	// 使用XML Request 的回调机制，传回json字符串
	js_data = jsonstr;

	return true;
}
