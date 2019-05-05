#pragma once

#include <algorithm>
#include <cctype>

#include "include/cef_browser.h"
#include "include/cef_callback.h"
#include "include/cef_frame.h"
#include "include/cef_resource_handler.h"
#include "include/cef_response.h"
#include "include/cef_request.h"
#include "include/cef_scheme.h"
#include "include/wrapper/cef_helpers.h"

#include "json/json.h"
#include <fstream> // ifstream, ifstream::in
#include <io.h>
#include <string>

using namespace std;

#define HANDLER_SCHEME_NAME			"client"
#define HANDLER_DOMAIN_NAME			"resources"
#define HANDLER_POSTDATA_NAME		"postdata"
#define HANDLER_BUFFER_IMAGE_NAME	"buffer_image"
#define HANDLER_LOCAL_IMAGE_NAME	"local_image"


#define URL_POST_DATA				"http://postdata/"
#define URL_LOCAL_IMAGE				"http://local_image/"
#define URL_BUFFER_IMAGE			"http://buffer_image/"
#define JSON_KEY_NAME_1				"func_name"
#define JSON_KEY_NAME_2				"paras_name"

// Implementation of the schema handler for client:// requests.
class ClientXMLRequestResourceHandler : public CefResourceHandler {
public:
	ClientXMLRequestResourceHandler(CefRefPtr<CefFrame> frame) : offset_(0) , frame_(frame){}

	// 使用XMLHttpRequest进行交互：第3步：重写该函数，用于接收JS的post请求
	virtual bool ProcessRequest(CefRefPtr<CefRequest> request,
		CefRefPtr<CefCallback> callback)
		OVERRIDE {
			CEF_REQUIRE_IO_THREAD();
			
			bool handled = false;
			char *buf = NULL;
			std::string sPostData;
			std::wstring wsPostData;

			std::string url = request->GetURL();

			if (url == URL_POST_DATA) {
				ParsePostData(request);
			} else if(url.find(URL_LOCAL_IMAGE) == 0) {
				std::string url_path = URL_LOCAL_IMAGE;
				const std::string& relative_path = url.substr(url_path.length());
				if (LoadBinaryResourceWithLocalImage(relative_path, data_)) {
					//读取图像成功后，需要设置handled，致使返回为true
					handled = true;
					// Set the resulting mime type
					mime_type_ = "image/jpg";//"arraybuffer";//
				}
			} else if(url.find(URL_BUFFER_IMAGE) == 0) {
				if (LoadBinaryResourceWithJSBuffer(request, data_)) {
					//读取图像成功后，需要设置handled，致使返回为true
					handled = true;
					// Set the resulting mime type
					mime_type_ = "image/jpg";//"arraybuffer";//
				}
			}

			if (handled) {
				// Indicate the headers are available.
				callback->Continue();
				return true;
			}

			return false;
	};

	void ParsePostData(CefRefPtr<CefRequest> request)
	{
		CefRefPtr<CefPostData> postData = request->GetPostData();
		if (postData) {
			CefPostData::ElementVector elements;
			postData->GetElements(elements);

			if (elements.size() > 0) {
				std::wstring queryString;
				CefRefPtr<CefPostDataElement> data = elements[0];
				if (data->GetType() == PDE_TYPE_BYTES) {
					const unsigned int length = data->GetBytesCount();
					if (length > 0) {
						char *arraybuffer = new char[length];
						if (arraybuffer) {
							memset(arraybuffer, 0, length);
							data->GetBytes(length, arraybuffer);

							// 解析从浏览器发送过来的Json数据
							Json::Reader reader;
							Json::Value root;
							bool ret = reader.parse(arraybuffer, root, false);
							if (!ret) {
								return ;
							}
							// 获得关键性的参数
							std::string key_name1("");
							std::string key_name2("");
							if (root[JSON_KEY_NAME_1].isString()) {
								key_name1 = root[JSON_KEY_NAME_1].asString();
							}
							if (root[JSON_KEY_NAME_2].isString()) {
								key_name2 = root[JSON_KEY_NAME_2].asString();
							}

							// 模拟再发送给浏览器
							Json::FastWriter writer;
							Json::Value inputjson;
							inputjson[JSON_KEY_NAME_1] = key_name1;
							inputjson[JSON_KEY_NAME_2] = key_name2;

							std::string jsonstr = writer.write(inputjson);

							// 有换行符的json字符串， JS不能处理。
							if (*jsonstr.rbegin() == '\n') {
								jsonstr.erase(jsonstr.end() - 1);
							}

							std::string text("jsSendCount('");
							std::string postfix("')");
							text += jsonstr;
							text += postfix;
							frame_->ExecuteJavaScript(text.c_str(), "", 0);
						}
					}
				}
			}
		}
	}

	bool LoadBinaryResourceWithJSBuffer(CefRefPtr<CefRequest> request, std::string& resource_data) { 
		CefRefPtr<CefPostData> postData = request->GetPostData();
		if (postData) {
			CefPostData::ElementVector elements;
			postData->GetElements(elements);

			if (elements.size() > 0) {
				std::wstring queryString;
				CefRefPtr<CefPostDataElement> data = elements[0];
				if (data->GetType() == PDE_TYPE_BYTES) {
					const unsigned int length = data->GetBytesCount();
					if (length > 0) {
						char *arraybuffer = new char[length];
						if (arraybuffer) {
							memset(arraybuffer, 0, length);
							data->GetBytes(length, arraybuffer);

							
							// 保存为文件
							{
								FILE* fp;
								fp = fopen("C:\\ztest2\\100.jpg", "wb");
								if (fp) {
									fwrite(arraybuffer, 1, length, fp);
									fclose(fp);
								}
							}

							// 临时，打开一个图像，回传给JS。以后，会回传一个经过处理过的图像，给JS端。
							{
								FILE* fp;
								fp = fopen("C:\\ztest2\\2000.jpg", "rb");
								if (fp) {
									int length = 0;
									//获取图像数据总长度	 
									fseek(fp, 0, SEEK_END);	 
									length=ftell(fp);	 
									rewind(fp);	 
									//根据图像数据长度分配内存buffer	
									char* ImgBuffer=(char*)malloc( length* sizeof(char) );	 
									//将图像数据读入buffer	 
									fread(ImgBuffer, length, 1, fp);	 
									fclose(fp);

									resource_data =   std::string(ImgBuffer, length);

									return true;
								}
							}
							

							
						}
					}
				}
			}
		}
		return false;
	}
	bool LoadBinaryResourceWithLocalImage(const std::string& resource_name, std::string& resource_data) {  
		//sleep 1 second
		//Sleep(10);
		DWORD dwSize;
		//图像数据长度
		int length;
		//文件指针
		FILE* fp;

		//得到当前时间
		SYSTEMTIME st;
		::GetLocalTime(&st);
		//	int milisec = st.wMilliseconds % 285;
		char ss[10];
		//随机
		//sprintf(ss,"%03d",milisec);

		//顺序
		stringstream str;
		str<<resource_name;
		int i;
		str>>i;
		sprintf(ss,"%d", i);

		string path =  "C:\\ztest2\\" + string(ss) + ".jpg";

		//判断文件是否存在
		if(_access(path.c_str(),0) == -1){
			return false;
		}

		fp=fopen(path.c_str(), "rb");
		//获取图像数据总长度	 
		fseek(fp, 0, SEEK_END);	 
		length=ftell(fp);	 
		rewind(fp);	 
		//根据图像数据长度分配内存buffer	
		char* ImgBuffer=(char*)malloc( length* sizeof(char) );	 
		//将图像数据读入buffer	 
		fread(ImgBuffer, length, 1, fp);	 
		fclose(fp);

		dwSize = length;
		resource_data = std::string(ImgBuffer, dwSize);

		//得到当前时间
		SYSTEMTIME st2;
		::GetLocalTime(&st2);
		//char chName[1000] = { 0 };
		//printf_s(chName, "%02d-%02d-%02d.%03d", st2.wHour, st2.wMinute, st2.wSecond, st2.wMilliseconds);

		// 到此，图片已经成功的被读取到内存（buffer）中
		//delete [] buffer;
		free(ImgBuffer);
		return true;
	}
	virtual void GetResponseHeaders(CefRefPtr<CefResponse> response,
		int64& response_length,
		CefString& redirectUrl) OVERRIDE {
			CEF_REQUIRE_IO_THREAD();

			DCHECK(!data_.empty());

			response->SetMimeType(mime_type_);
			response->SetStatus(200);

			// 加了这个命令ok
			CefRequest::HeaderMap headers;
			headers.insert(std::make_pair("Access-Control-Allow-Origin", "*"));
			response->SetHeaderMap(headers);

			// Set the resulting response length
			response_length = data_.length();
	};

	virtual void Cancel() OVERRIDE {
		CEF_REQUIRE_IO_THREAD();
	};

	virtual bool ReadResponse(void* data_out,
		int bytes_to_read,
		int& bytes_read,
		CefRefPtr<CefCallback> callback)
		OVERRIDE 
	{
		CEF_REQUIRE_IO_THREAD();

		bool has_data = false;
  		bytes_read = 0;
  
  		if (offset_ < data_.length()) {
  			// Copy the next block of data into the buffer.
			int transfer_size = std::min<int>(bytes_to_read, static_cast<int>(data_.length() - offset_));
  			memcpy(data_out, data_.c_str() + offset_, transfer_size);
  			offset_ += transfer_size;
  
  			bytes_read = transfer_size;
  			has_data = true;
  		}

		return has_data;
	};

private:
	std::string data_;
	std::string mime_type_;
	size_t offset_;
	CefRefPtr<CefFrame> frame_;

	IMPLEMENT_REFCOUNTING(ClientXMLRequestResourceHandler);
};



// Implementation of the factory for for creating schema handlers.
class ClientXMLRequestSchemeHandlerFactory : public CefSchemeHandlerFactory {
public:
	// Return a new scheme handler instance to handle the request.
	virtual CefRefPtr<CefResourceHandler> Create(CefRefPtr<CefBrowser> browser,
		CefRefPtr<CefFrame> frame,
		const CefString& scheme_name,
		CefRefPtr<CefRequest> request)
		OVERRIDE 
	{
		CEF_REQUIRE_IO_THREAD();
		// 使用XMLHttpRequest进行交互：第2步：需要实现一个CefResourceHandler派生类
		return new ClientXMLRequestResourceHandler(frame);
	}

	IMPLEMENT_REFCOUNTING(ClientXMLRequestSchemeHandlerFactory);
};
