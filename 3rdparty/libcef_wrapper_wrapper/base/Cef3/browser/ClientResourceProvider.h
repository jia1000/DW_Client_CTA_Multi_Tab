
#include <string>
#include "include/base/cef_logging.h"
#include "include/cef_stream.h"
#include "include/wrapper/cef_byte_read_handler.h"
#include "include/wrapper/cef_stream_resource_handler.h"
#include "include/wrapper/cef_resource_manager.h"

// Replace all instances of |from| with |to| in |str|.
std::string StringReplace(const std::string& str, const std::string& from,
						  const std::string& to) 
{
	std::string result = str;
	std::string::size_type pos = 0;
	std::string::size_type from_len = from.length();
	std::string::size_type to_len = to.length();
	do {
		pos = result.find(from, pos);
		if (pos != std::string::npos) {
			result.replace(pos, from_len, to);
			pos += to_len;
		}
	} while (pos != std::string::npos);
	return result;
}


// Provider of local file resources.
// url_path_: http://client/
class ClientResourceProvider : public CefResourceManager::Provider {
public:
	explicit ClientResourceProvider(const std::string& url_path)
		: url_path_(url_path) {
			DCHECK(!url_path.empty());
	}

	bool OnRequest(scoped_refptr<CefResourceManager::Request> request) OVERRIDE {
		CEF_REQUIRE_IO_THREAD();

		const std::string& url = request->url();
		if (url.find(url_path_) != 0L) {

			CGLogger::Debug("ClientResourceProvider->OnRequest-> original request url: " + url);

			// Not handled by this provider.
			return false;
		}

		CefRefPtr<CefResourceHandler> handler;
		//url_path_: "http://client"
		const std::string& relative_path = url.substr(url_path_.length());
		if (!relative_path.empty()) {			

			//TCHAR tszModule[MAX_PATH + 1] = { 0 };
			//::GetCurrentDirectory(MAX_PATH, tszModule);

			CefString instancePath = Configurations::getInstance()->GetResourceDir();
			instancePath.FromString(instancePath.ToString() + StringReplace(relative_path, "/", "\\"));

			CGLogger::Debug("ClientResourceProvider->OnRequest-> convert request from [" + url + "] to [" + instancePath.ToString() + "]");
			CGLogger::Info("Request local resources [" + instancePath.ToString() + "]");

			CefRefPtr<CefStreamReader> stream =
				CefStreamReader::CreateForFile(instancePath);

			if (stream.get()) {
				handler = new CefStreamResourceHandler(
					request->mime_type_resolver().Run(url),
					stream);
			}
		}
		else{

			//LOG(ERROR) << "Request local resources with invalid path.";
			CGLogger::Error("Request local resources with invalid path.");

		}

		request->Continue(handler);
		return true;
	}

private:
	std::string url_path_;

	//DISALLOW_COPY_AND_ASSIGN(BinaryResourceProvider);
};
