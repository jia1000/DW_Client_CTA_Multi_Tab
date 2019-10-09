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
#include "data_transfer_control/data_transfer_control.h"

#define HANDLER_IAMGE_CONTROLLER				"image_controller"
#define HANDLER_IAMGE_ARRAY_BUFFER_TRANSFER		"image_buffer_transfer"

const static std::string http_image_controller		= std::string("http://") + std::string(HANDLER_IAMGE_CONTROLLER);
const static std::string http_image_buffer_transfer = std::string("http://") + std::string(HANDLER_IAMGE_ARRAY_BUFFER_TRANSFER);

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
			data_ = "";
			
			std::string url = request->GetURL();

            std::string method = request->GetMethod();
			// 和web端联调时，web端会先发送一个Option的请求，然后再发送Get。这里，需要不处理Option请求
            if (method == "OPTIONS")
            {
                handled = true;
                mime_type_ = "application/octet-stream";                
            } 
            else if(url.find(http_image_controller) == 0) 
            {
				if (ParseImageOperationPostData(request, data_)) {
					//读取图像成功后，需要设置handled，致使返回为true
					handled = true;
					// Set the resulting mime type
					mime_type_ = "text";//"image/jpg";					
				} else {
					// 和web端联调时，不会发送postdata
					char *arraybuffer = new char[256];
					if (!DataTransferController::GetInstance()->ParseDcmOperationData(
						arraybuffer, data_)) {
							return false;
					}
					//读取图像成功后，需要设置handled，致使返回为true
					handled = true;
					// Set the resulting mime type
					// 直接读取dcm文件，和web端联调,返回类型需要是application/octet-stream
					mime_type_ = "application/octet-stream";
				}

			} else if(url.find(http_image_buffer_transfer) == 0) {
				if ( HandleArrayBuffrDataFromJS(request, data_)) {
					//读取图像成功后，需要设置handled，致使返回为true
					handled = true;
					// Set the resulting mime type
					mime_type_ = "arraybuffer";
				}
			}  

			if (handled) {
				// Indicate the headers are available.
				callback->Continue();
				return true;
			}

			return false;
	};
	bool ParseImageOperationPostData(CefRefPtr<CefRequest> request, std::string& resource_data)
	{
		CefRefPtr<CefPostData> postData = request->GetPostData();
		if (!postData) {
			return false;
		}
		CefPostData::ElementVector elements;
		postData->GetElements(elements);

		if (elements.size() <= 0) {
			return false;
		}
		std::wstring queryString;
		CefRefPtr<CefPostDataElement> data = elements[0];
		if (data->GetType() != PDE_TYPE_BYTES) {
			return false;
		}
		const unsigned int length = data->GetBytesCount();
		if (length == 0) {
			return false;
		}

		char *arraybuffer = new char[length + 1];
		if (!arraybuffer) {
			return false;
		}

		memset(arraybuffer, 0, length + 1);
		data->GetBytes(length, arraybuffer);

		if (DataTransferController::GetInstance()->ParseImageOperationData(
			arraybuffer, resource_data)) {
				return true;
		}

		return false;
	}

	bool  HandleArrayBuffrDataFromJS(CefRefPtr<CefRequest> request, std::string& resource_data) { 
		CefRefPtr<CefPostData> postData = request->GetPostData();
		if (!postData) {
			return false;
		}

		CefPostData::ElementVector elements;
		postData->GetElements(elements);

		if (elements.size() <= 0) {
			return false;
		}

		std::wstring queryString;
		CefRefPtr<CefPostDataElement> data = elements[0];
		if (data->GetType() != PDE_TYPE_BYTES) {
			return false;
		}

		const unsigned int length = data->GetBytesCount();
		if (length == 0) {
			return false;
		}

		char *arraybuffer = new char[length];
		if (!arraybuffer) {
			return false;
		}

		memset(arraybuffer, 0, length);
		data->GetBytes(length, arraybuffer);
		// 此处需要将arraybuffer数据交给vtk处理
		// to do

		resource_data = "";
		return true;
	}

	virtual void GetResponseHeaders(CefRefPtr<CefResponse> response,
		int64& response_length,
		CefString& redirectUrl) OVERRIDE {
			CEF_REQUIRE_IO_THREAD();

			//DCHECK(!data_.empty());

			response->SetMimeType(mime_type_);
			response->SetStatus(200);

			// 加了这个命令ok
			CefRequest::HeaderMap headers;
			headers.insert(std::make_pair("Access-Control-Allow-Origin", "*"));

			// 和web端联调时，需要添加如下的Header属性-----------------
            // 响应类型
            headers.insert(std::make_pair("Access-Control-Allow-Methods", "*"));
            // 响应头设置
            headers.insert(std::make_pair("Access-Control-Allow-Headers", "content-type,token,id"));
            headers.insert(std::make_pair("Access-Control-Request-Headers", "Origin, X-Requested-With, content-Type, Accept, Authorization"));
            // 和web端联调时，Content Length设置
            headers.insert(std::make_pair("Content-Length", "15302874"));
			//-------------------------------------------------------------------------

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
  
  		if (data_.length() > 0 && offset_ < data_.length()) {
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
