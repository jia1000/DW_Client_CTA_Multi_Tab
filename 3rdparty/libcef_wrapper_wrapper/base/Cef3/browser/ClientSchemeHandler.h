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

#define HANDLER_SCHEME_NAME "client"
#define HANDLER_DOMAIN_NAME "resources"
#define HANDLER_POSTDATA_NAME "postdata"

#define JSON_KEY_NAME_1 "func_name"
#define JSON_KEY_NAME_2 "paras_name"

// Implementation of the schema handler for client:// requests.
class ClientSchemeHandler : public CefResourceHandler {
public:
	ClientSchemeHandler(CefRefPtr<CefFrame> frame) : offset_(0) , frame_(frame){}

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
									return false;
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

			if (handled) {
				// Indicate the headers are available.
				callback->Continue();
				return true;
			}

			return false;
	};

	virtual void GetResponseHeaders(CefRefPtr<CefResponse> response,
		int64& response_length,
		CefString& redirectUrl) OVERRIDE {
			CEF_REQUIRE_IO_THREAD();

			DCHECK(!data_.empty());

			response->SetMimeType(mime_type_);
			response->SetStatus(200);

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

	IMPLEMENT_REFCOUNTING(ClientSchemeHandler);
};



// Implementation of the factory for for creating schema handlers.
class ClientSchemeHandlerFactory : public CefSchemeHandlerFactory {
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
		return new ClientSchemeHandler(frame);
	}

	IMPLEMENT_REFCOUNTING(ClientSchemeHandlerFactory);
};
