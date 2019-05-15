#include "stdafx.h"

#include "data_transfer_control.h"
#include "image_process.h"

#include <algorithm>

#include "json/json.h"
#include <fstream> // ifstream, ifstream::in
#include <io.h>
#include <chrono>

#ifdef USE_RAPID_JSON
#include "rapidjson/rapidjson.h"
#include "rapidjson/document.h"
#include "rapidjson/reader.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
using namespace rapidjson;
#endif

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

bool DataTransferController::ParseImageOperationData(char* json_data, std::string& js_data)
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

	if(image_process) {
		delete image_process;
		image_process = NULL;
	}

	std::string out_image_data = "";

	if (key_name1 == JSON_VALUE_REQUEST_TYPE_MPR) {
		image_process = new ImageMPRProcess(key_name3);
		image_process->SetKey2_ImageOperation(key_name2);
		image_process->Excute(out_image_data);
	} else if (key_name1 == JSON_VALUE_REQUEST_TYPE_VR) {
		image_process = new ImageVRProcess(key_name3);
		image_process->SetKey2_ImageOperation(key_name2);
		image_process->Excute(out_image_data);
	} else if (key_name1 == JSON_VALUE_REQUEST_TYPE_CPR) {		
	} else if (key_name1 == JSON_VALUE_REQUEST_TYPE_TIME) {		
		return true;
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
	// 使用XML Request 的回调机制，传回json字符串
	js_data = jsonstr;

	return true;
}

#ifdef USE_RAPID_JSON
bool DataTransferController::ParseImageOperationDataUseRapidJson(char* json_data, std::string& js_data)
{
	std::chrono::steady_clock::time_point t1 = std::chrono::steady_clock::now();
	// 解析从浏览器发送过来的Json数据  //json字段解析要做保护判断。
	Document doc;
	doc.Parse(json_data);

	std::chrono::steady_clock::time_point t2 = std::chrono::steady_clock::now();
	std::chrono::duration<double> span = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);
	static std::chrono::duration<double> total_diff = std::chrono::duration<double>(0);;
	total_diff += span;

	// 获得关键性的参数
	std::string key_name1("");
	std::string key_name2("");
	std::string key_name3("");
	std::string key_name4("");
	//std::string key_name5("");
	//std::string key_name6("");
	int key_name5 = 0;
	int key_name6 = 0;

	if (doc.HasMember(JSON_KEY_REQUEST_TYPE)) {
		const Value& value = doc[JSON_KEY_REQUEST_TYPE];
		key_name1 = value.GetString();
		// 都转为小写字符
		std::transform(key_name1.begin(), key_name1.end(), key_name1.begin(), ::tolower);
	}
	if (doc.HasMember(JSON_KEY_IMAGE_OPERATION)) {
		const Value& value = doc[JSON_KEY_IMAGE_OPERATION];
		key_name2 = value.GetString();
		// 都转为小写字符
		std::transform(key_name2.begin(), key_name2.end(), key_name2.begin(), ::tolower);
	}

	if (doc.HasMember(JSON_KEY_IMAGE_PARAS)) {
		const Value& value = doc[JSON_KEY_IMAGE_PARAS];
		key_name3 = value.GetString();
		// 都转为小写字符
		std::transform(key_name3.begin(), key_name3.end(), key_name3.begin(), ::tolower);
	}
	if (doc.HasMember(JSON_KEY_IMAGE_DATA)) {
		const Value& value = doc[JSON_KEY_IMAGE_DATA];
		key_name4 = value.GetString();
	}
	if (doc.HasMember(JSON_KEY_IMAGE_SEQUENCE)) {
		const Value& value = doc[JSON_KEY_IMAGE_SEQUENCE];
		key_name5 = value.GetInt();
	}
	if (doc.HasMember(JSON_KEY_IMAGE_MAX)) {
		const Value& value = doc[JSON_KEY_IMAGE_MAX];
		key_name6 = value.GetInt();
	}

	if(image_process) {
		delete image_process;
		image_process = NULL;
	}

	std::string out_image_data = "";

	if (key_name1 == JSON_VALUE_REQUEST_TYPE_MPR) {
		CGLogger::Debug("Debug_CTA:MPR---begin");
		image_process = new ImageMPRProcess(key_name3, key_name4);
		image_process->SetKey2_ImageOperation(key_name2);
		image_process->Excute(out_image_data);
		CGLogger::Debug("Debug_CTA:MPR---");
	} else if (key_name1 == JSON_VALUE_REQUEST_TYPE_VR) {
		CGLogger::Debug("Debug_CTA:VR---begin");
		image_process = new ImageVRProcess(key_name3, key_name4);
		image_process->SetKey2_ImageOperation(key_name2);
		image_process->Excute(out_image_data);
		CGLogger::Debug("Debug_CTA:VR---");
	} else if (key_name1 == JSON_VALUE_REQUEST_TYPE_CPR) {		
	} else if (key_name1 == JSON_VALUE_REQUEST_TYPE_TIME) {
		if (0 == key_name5) {
			total_diff = std::chrono::duration<double>(0);
			CGLogger::Debug("Debug_CTA:TIME_TEST_TOTAL ------------------begin");
		}
		if (key_name5 == key_name6 - 1) {
			CGLogger::Debug("Debug_CTA:TIME_TEST_TOTAL -------------------end");
			std::stringstream ss; 
			ss << "Debug_CTA:TIME_TEST_TOTAL ----------------------------diff :";
			ss << total_diff.count();
			CGLogger::Debug(ss.str().c_str());
		}

		CGLogger::Debug("Debug_CTA:TIME_TEST---begin");
		js_data  = "";
		CGLogger::Debug("Debug_CTA:TIME_TEST---");
		return true;
	} else {		
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
	
	// 使用XML Request 的回调机制，传回json字符串
	js_data = jsonstr;

	return true;
}
#endif