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

#define HANDLER_SCHEME_NAME "client"
#define HANDLER_DOMAIN_NAME "resources"
#define HANDLER_POSTDATA_NAME "postdata"

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
							}							
							//buffer里存储了arraybuffer的数据
							wchar_t *wc_buffer = new wchar_t[(length + 2) / 2];

							if (wc_buffer) {
								memset(wc_buffer, 0, length + 2);
								memcpy((char*)wc_buffer, arraybuffer, length);
								std::wstring wsPostData2(wc_buffer);
								//::MessageBox(NULL, wsPostData2.c_str(), _T("Postdata"), MB_OK);
								// 遇到有空格的文本时，会出现问题。
								static int count = 1;
								std::wstring text;
								std::wstringstream wss;
								wss << _T("jsSendCount('");
								wss << count;
								//wss << _T("");
								wss << _T(".send_txt:");
								wss << wsPostData2;

								wss << _T("')");
								wss >> text;
								
								frame_->ExecuteJavaScript(text.c_str(), "", 0);
								count++;
							}
						}
					}
				}
			}


			//// 转换为小写
			//for (int i = 0; i < url.size(); i++)
			//	url[i] = std::tolower(url[i]);

			//// 解析相对路径，创建并返回CefStreamResourceHandler
			//std::string baseUrl = HANDLER_SCHEME_NAME + "://" + HANDLER_DOMAIN_NAME;
			//const std::string& relative_path = url.substr(baseUrl.length());
			//if (!relative_path.empty()) {

			//	TCHAR tszModule[MAX_PATH + 1] = { 0 };
			//	::GetCurrentDirectory(MAX_PATH, tszModule);

			//	CefString instancePath = tszModule;
			//	instancePath.FromString(instancePath.ToString() + "/resources/" + relative_path);

			//	CefRefPtr<CefStreamReader> stream =
			//		CefStreamReader::CreateForFile(instancePath);

			//	if (stream.get()) {
			//		handler = new CefStreamResourceHandler(
			//			request->mime_type_resolver().Run(url),
			//			stream);
			//	}
			//}

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
