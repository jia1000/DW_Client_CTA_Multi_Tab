#include "stdafx.h"

#include "data_transfer_control.h"

#include <algorithm>

#include "json/json.h"
#include <fstream> // ifstream, ifstream::in
#include <io.h>

#define JSON_KEY_REQUEST_TYPE				"request_type"
#define JSON_KEY_IMAGE_OPERATION			"image_operation"
#define JSON_KEY_IMAGE_PARAS				"image_paras"
#define JSON_KEY_IMAGE_DATA					"image_data"

#define JSON_VALUE_IMAGE_OPERATION_ZOOM		"zoom"
#define JSON_VALUE_IMAGE_OPERATION_ROTATE	"rotate"
#define JSON_VALUE_IMAGE_OPERATION_MOVE		"move"


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
	std::string key_name4("");

	
	if (root[JSON_KEY_REQUEST_TYPE].isString()) {
		key_name1 = root[JSON_KEY_REQUEST_TYPE].asString();
	}
	if (root[JSON_KEY_IMAGE_OPERATION].isString()) {
		key_name2 = root[JSON_KEY_IMAGE_OPERATION].asString();
		// 都转为小写字符
		std::transform(key_name2.begin(), key_name2.end(), key_name2.begin(), ::tolower);
	}
	if (root[JSON_KEY_IMAGE_PARAS].isString()) {
		key_name3 = root[JSON_KEY_IMAGE_PARAS].asString();
	}
	if (root[JSON_KEY_IMAGE_DATA].isString()) {
		key_name4 = root[JSON_KEY_IMAGE_DATA].asString();
	}

	std::string out_image_data = "";

	if (key_name2 == JSON_VALUE_IMAGE_OPERATION_ZOOM) {
		ImageZoom(key_name3, key_name4, out_image_data);
	} else if (key_name2 == JSON_VALUE_IMAGE_OPERATION_ROTATE) {
		ImageRotate1(key_name3, key_name4, out_image_data);
	} else if (key_name2 == JSON_VALUE_IMAGE_OPERATION_MOVE) {
		static bool test_flag = true;
		if (test_flag) {
			ImageMove1(key_name3, key_name4, out_image_data);
		} else {
			ImageMove2(key_name3, key_name4, out_image_data);
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

bool DataTransferController::ImageZoom(std::string str_rate, std::string& in_image_data, std::string& out_image_data)
{
	cv::Mat src_image = Base2Mat(in_image_data);
	//cv::imwrite("C:\\ztest2\\a.png", src_image);
	
	std::string::size_type sz;
	double rate = std::stod(str_rate, &sz);

	// 缩放图像
	cv::Mat dst_image;
	cv::resize(src_image, dst_image, cv::Size(), rate, rate);

	out_image_data = Mat2Base64(dst_image, "png");
	//cv::imwrite("C:\\ztest2\\b.png", dst_image);

	return false;
}

bool DataTransferController::ImageRotate1(std::string str_angel, std::string& in_image_data, std::string& out_image_data)
{
	cv::Mat src_image = Base2Mat(in_image_data);

	std::string::size_type sz;
	double rate = std::stod(str_angel, &sz);
	int degree = (int)(rate * 10) % 3;

	// 旋转图像
	double m[6];
	cv::Mat dst_image;
	cv::rotate(src_image, dst_image, degree);

	out_image_data = Mat2Base64(dst_image, "png");

	return false;
}

bool DataTransferController::ImageMove1(std::string str_move_position, std::string& in_image_data, std::string& out_image_data)
{
	cv::Mat src_image = Base2Mat(in_image_data);

	std::string::size_type sz;
	double move_position = std::stod(str_move_position, &sz);

	int dx = move_position * 100;
	int dy = move_position * 100;

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

	out_image_data = Mat2Base64(dst_image, "png");
	return false;
}

bool DataTransferController::ImageMove2(std::string str_move_position, std::string& in_image_data, std::string& out_image_data)
{
	cv::Mat src_image = Base2Mat(in_image_data);

	std::string::size_type sz;
	double move_position = std::stod(str_move_position, &sz);

	int dx = move_position * 100;
	int dy = move_position * 100;

	const int rows = src_image.rows + abs(dy); //输出图像的大小
	const int cols = src_image.cols + abs(dx);

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

	out_image_data = Mat2Base64(dst_image, "png");
	return false;
}

std::string DataTransferController::base64Decode(const char* Data, int DataByte) {
	//解码表
	const char DecodeTable[] =
	{
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		62, // '+'
		0, 0, 0,
		63, // '/'
		52, 53, 54, 55, 56, 57, 58, 59, 60, 61, // '0'-'9'
		0, 0, 0, 0, 0, 0, 0,
		0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12,
		13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, // 'A'-'Z'
		0, 0, 0, 0, 0, 0,
		26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38,
		39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, // 'a'-'z'
	};
	std::string strDecode;
	int nValue;
	int i = 0;
	while (i < DataByte) {
		if (*Data != '\r' && *Data != '\n') {
			nValue = DecodeTable[*Data++] << 18;
			nValue += DecodeTable[*Data++] << 12;
			strDecode += (nValue & 0x00FF0000) >> 16;
			if (*Data != '=') {
				nValue += DecodeTable[*Data++] << 6;
				strDecode += (nValue & 0x0000FF00) >> 8;
				if (*Data != '=') {
					nValue += DecodeTable[*Data++];
					strDecode += nValue & 0x000000FF;
				}
			}
			i += 4;
		}
		else {
			Data++;
			i++;
		}
	}
	return strDecode;
}


std::string DataTransferController::base64Encode(const unsigned char* Data, int DataByte) {
	//编码表
	const char EncodeTable[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
	//返回值
	std::string strEncode;
	unsigned char Tmp[4] = { 0 };
	int LineLength = 0;
	for (int i = 0; i < (int)(DataByte / 3); i++) {
		Tmp[1] = *Data++;
		Tmp[2] = *Data++;
		Tmp[3] = *Data++;
		strEncode += EncodeTable[Tmp[1] >> 2];
		strEncode += EncodeTable[((Tmp[1] << 4) | (Tmp[2] >> 4)) & 0x3F];
		strEncode += EncodeTable[((Tmp[2] << 2) | (Tmp[3] >> 6)) & 0x3F];
		strEncode += EncodeTable[Tmp[3] & 0x3F];
		if (LineLength += 4, LineLength == 76) { strEncode += "\r\n"; LineLength = 0; }
	}
	//对剩余数据进行编码
	int Mod = DataByte % 3;
	if (Mod == 1) {
		Tmp[1] = *Data++;
		strEncode += EncodeTable[(Tmp[1] & 0xFC) >> 2];
		strEncode += EncodeTable[((Tmp[1] & 0x03) << 4)];
		strEncode += "==";
	}
	else if (Mod == 2) {
		Tmp[1] = *Data++;
		Tmp[2] = *Data++;
		strEncode += EncodeTable[(Tmp[1] & 0xFC) >> 2];
		strEncode += EncodeTable[((Tmp[1] & 0x03) << 4) | ((Tmp[2] & 0xF0) >> 4)];
		strEncode += EncodeTable[((Tmp[2] & 0x0F) << 2)];
		strEncode += "=";
	}


	return strEncode;
}

//imgType 包括png bmp jpg jpeg等opencv能够进行编码解码的文件
std::string DataTransferController::Mat2Base64(const cv::Mat &img, std::string imgType) {
	//Mat转base64
	std::string img_data;
	std::vector<uchar> vecImg;
	std::vector<int> vecCompression_params;
	vecCompression_params.push_back(CV_IMWRITE_JPEG_QUALITY);
	vecCompression_params.push_back(90);
	imgType = "." + imgType;
	cv::imencode(imgType, img, vecImg, vecCompression_params);
	img_data = base64Encode(vecImg.data(), vecImg.size());
	return img_data;
}


cv::Mat DataTransferController::Base2Mat(std::string &base64_data) {
	cv::Mat img;
	std::string s_mat;
	s_mat = base64Decode(base64_data.data(), base64_data.size());
	std::vector<char> base64_img(s_mat.begin(), s_mat.end());
	img = cv::imdecode(base64_img, CV_LOAD_IMAGE_COLOR);
	return img;
}
