#include "stdafx.h"

#include "data_transfer_control.h"

#include "json/json.h"
#include <fstream> // ifstream, ifstream::in
#include <io.h>

#define JSON_KEY_NAME_1				"request_type"
#define JSON_KEY_NAME_2				"image_operation"
#define JSON_KEY_NAME_3				"image_paras"


DataTransferController* DataTransferController::instance = nullptr;

DataTransferController* DataTransferController::GetInstance()
{
	if (instance == nullptr)
		instance = new DataTransferController();
	return instance;
}

DataTransferController::DataTransferController()
{
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

	if (root[JSON_KEY_NAME_1].isString()) {
		key_name1 = root[JSON_KEY_NAME_1].asString();
	}
	if (root[JSON_KEY_NAME_2].isString()) {
		key_name2 = root[JSON_KEY_NAME_2].asString();
	}
	if (root[JSON_KEY_NAME_3].isString()) {
		key_name3 = root[JSON_KEY_NAME_3].asString();
	}

	if (key_name2 == "Zoom") {
		ImageZoom(key_name3);
	} else if (key_name2 == "Rotate") {
		ImageRotate(key_name3);
	} else if (key_name2 == "Move") {
		ImageMove(key_name3);
	}

	// 模拟再发送给浏览器
	Json::FastWriter writer;
	Json::Value inputjson;
	inputjson[JSON_KEY_NAME_1] = key_name1;
	inputjson[JSON_KEY_NAME_2] = key_name2;
	inputjson[JSON_KEY_NAME_3] = key_name3;


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
	js_data = text;

	return true;
}

bool DataTransferController::ImageZoom(std::string str_rate)
{
	return false;
}

bool DataTransferController::ImageRotate(std::string str_angel)
{
	return false;
}

bool DataTransferController::ImageMove(std::string str_move_position)
{
	return false;
}