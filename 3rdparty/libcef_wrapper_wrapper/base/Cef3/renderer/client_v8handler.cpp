#include "StdAfx.h"
#include "client_v8handler.h"
#include "encrypt/base64.h"
#include "encrypt/utf8string.h"
#include "encrypt/EncryptUtil.h"

//#define USE_AES

bool V8Handler::Execute( const CefString& name, CefRefPtr<CefV8Value> object, \
							const CefV8ValueList& arguments, CefRefPtr<CefV8Value>& retval, CefString& exception )
{
	CefRefPtr<CefV8Context> context = CefV8Context::GetCurrentContext();

	CefRefPtr<CefProcessMessage> msg = CefProcessMessage::Create( name );

	// Retrieve the argument list object.
	CefRefPtr<CefListValue> args = msg->GetArgumentList();

	// Populate the argument values.
	int _size = arguments.size();

	args->SetSize(_size);

	for (int i = 0; i < _size; i++)
	{
		if (arguments[i]->IsString())
		{
			args->SetString(i, arguments[i]->GetStringValue());
		}
		else if (arguments[i]->IsInt())
		{
			args->SetInt(i, arguments[i]->GetIntValue());
		}
		else if (arguments[i]->IsDouble())
		{
			args->SetDouble(i, arguments[i]->GetDoubleValue());
		}
		else if (arguments[i]->IsBool())
		{
			args->SetBool(i, arguments[i]->GetBoolValue());
		}
		else
		{
			args->SetString(i, arguments[i]->GetStringValue());
		}
	}

	//handle custom functions for js call
	if (msg->GetName() == V8WindowFunciotn::loginFunc){
		_size = arguments.size();

		std::string d1 = args->GetString(0);
		std::string d2 = args->GetString(1);

		retval = CefV8Value::CreateString(d1 + " " + d2);

	}else if(msg->GetName() == V8WindowFunciotn::decodeFunc){
		try{
			_size = arguments.size();
			if (_size > 0){
				//不用转换wstring
				string encodedStr = args->GetString(0);
				string decodedStr = Decode(encodedStr);
				// 转换为wstring: 此步必须 -> 以下转换造成返回js字符串结尾带了特殊字符
				wstring wdecodedStr = StringToWString(decodedStr);
				retval = CefV8Value::CreateString(wdecodedStr);
			}
		}
		catch(std::exception& ex){

			//LOG(ERROR) << "Error occured during decoding. Msg: " << ex.what() << endl;
			CGLogger::Error("Error occured during decoding.", ex.what());

		}

	}else if(msg->GetName() == V8WindowFunciotn::encodeFunc){
		try{
			_size = arguments.size();
			if (_size > 0){
				wstring wdecodedStr = args->GetString(0);

				string encodedStr = Encode(WStringToString(wdecodedStr));
				//// 转换为wstring
				//wstring wencodedStr = StringToWString(encodedStr);
				retval = CefV8Value::CreateString(encodedStr);
			}
		}
		catch(std::exception& ex){

			//LOG(ERROR) << "Error occured during encoding. Msg: " << ex.what() << endl;
			CGLogger::Error("Error occured during encoding.", ex.what());

		}

	}else if(msg->GetName() == V8WindowFunciotn::domTest){

		/*_size = arguments.size();		
		CefRefPtr<CefDOMVisitor> visitor = new SimpleDomVisitor();
		context->GetBrowser()->GetMainFrame()->VisitDOM(visitor);*/
	}

	context->GetBrowser()->SendProcessMessage(PID_BROWSER, msg);

	return true;
}

std::string V8Handler::GenerateKey(const string& str)
{
	return str;
}

std::string V8Handler::Decode(const string& encodedStr)
{
	std::string decodedStr = EncryptDecryptUtils::ExecDecodeDecrypt(encodedStr);
	return decodedStr;
}

std::string V8Handler::Encode(const string& originalStr)
{
	std::string encodedStr = EncryptDecryptUtils::ExecEncryptEncode(originalStr);
	return encodedStr;
}
/////////////////////////////////////////////////////////////////////////////////
