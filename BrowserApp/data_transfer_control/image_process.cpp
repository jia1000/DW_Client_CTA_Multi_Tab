#include "stdafx.h"

#include "image_process.h"
#include "io/txt_reader.h"
#include "tools/string_util.h"
#include "io/nii_loader.h"

#include <algorithm>
#include "json/json.h"
#include <fstream> // ifstream, ifstream::in
#include <io.h>

#include "api/studycontextmy.h"
#include "curve_source.h"
#include "data/data_definition.h"
#include "data/data_loader.h"
#include "data_source.h"
#include "encrypt/Utf8String.h"
#include "io/dcm_reader.h"
#include "render_source.h"
#include "render_facade.h"

using namespace DW;
using namespace DW::IMAGE;
using namespace DW::Render;
using namespace DW::IO;

// only once
static DW::IO::IDicomReader* reader = NULL;
static bool is_create_mpr_render = false;
static bool is_create_vr_render  = false;
static bool is_create_cpr_render  = false;

const std::string dicom_files_dir("C:\\ztest2\\dicom_test");
const std::string nii_file_data_path("C:\\ztest2\\1203_0000.nii");
const std::string nii_file_mask_path("C:\\ztest2\\1204_blood.nii");
const std::string nii_curve_data_path("C:\\ztest2\\curve_data_nii_vessel.txt");
const std::string screen_shot_file_path("C:\\ztest2\\screen_shot_temp.bmp");

const std::string series_name_mpr("series1");
const std::string series_name_vr("series1");
const std::string series_name_cpr("series1");



ImageProcessBase::ImageProcessBase(std::string str_paras)
	: m_key3_str_paras(str_paras)
	//, req_type(0)
{
}

ImageProcessBase::~ImageProcessBase()
{
	
}

//void ImageProcessBase::SetKey1_RequestType(std::string str_req_type)
//{
//	if (str_req_type == JSON_VALUE_REQUEST_TYPE_MPR) {
//		req_type = (int)RenderControlType::MPR;
//	} else if (str_req_type == JSON_VALUE_REQUEST_TYPE_VR) {
//		req_type = (int)RenderControlType::VR;
//	} else if (str_req_type == JSON_VALUE_REQUEST_TYPE_CPR) {
//		req_type = (int)RenderControlType::STRAIGHTENED_CPR;
//	}
//}

void ImageProcessBase::SetKey2_ImageOperation(std::string str_opertation) 
{ 
	m_key2_str_opertation = str_opertation; 
}

void ImageProcessBase::SetKey3_ImageOperationParas(std::string str_paras) 
{ 
	m_key3_str_paras = str_paras; 
}

bool ImageProcessBase::Excute(std::string& out_image_data)
{
	return true;
}
std::string ImageProcessBase::base64Decode(const char* Data, int DataByte) {
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


std::string ImageProcessBase::base64Encode(const unsigned char* Data, int DataByte) {
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
std::string ImageProcessBase::Mat2Base64(const cv::Mat &img, std::string imgType) {
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


cv::Mat ImageProcessBase::Base2Mat(std::string &base64_data) {
	cv::Mat img;
	std::string s_mat;
	s_mat = base64Decode(base64_data.data(), base64_data.size());
	std::vector<char> base64_img(s_mat.begin(), s_mat.end());
	img = cv::imdecode(base64_img, CV_LOAD_IMAGE_COLOR);
	return img;
}


bool ImageProcessBase::SaveBitmapToFile(HBITMAP hBitmap, LPCWSTR lpFileName)
{
	HDC                hDC;                //设备描述表
	int                iBits;              //当前显示分辨率下每个像素所占字节数
	WORD               wBitCount = 0;      //位图中每个像素所占字节数    
	DWORD              dwPaletteSize=0;    //定义调色板大小
	DWORD              dwBmBitsSize;       //位图中像素字节大小
	DWORD              dwDIBSize;          //位图文件大小
	DWORD              dwWritten;          //写入文件字节数
	BITMAP             Bitmap;             //位图结构
	BITMAPFILEHEADER   bmfHdr;             //位图属性结构   
	BITMAPINFOHEADER   bi;                 //位图文件头结构
	LPBITMAPINFOHEADER lpbi;               //位图信息头结构     指向位图信息头结构
	HANDLE             fh;                 //定义文件句柄
	HANDLE             hDib;               //分配内存句柄
	HANDLE             hPal;               //分配内存句柄
	HANDLE             hOldPal=NULL;       //调色板句柄  

	//计算位图文件每个像素所占字节数   
	hDC = CreateDC(L"DISPLAY",NULL,NULL,NULL);   
	iBits = GetDeviceCaps(hDC, BITSPIXEL) * GetDeviceCaps(hDC, PLANES);   
	DeleteDC(hDC);

	if (iBits <= 1)       
		wBitCount = 1;   
	else if (iBits <= 4)       
		wBitCount = 4;   
	else if (iBits <= 8)       
		wBitCount = 8;   
	else if (iBits <= 24)
		wBitCount = 24;
	else if (iBits<=32)
		wBitCount = 24;


	//计算调色板大小   
	if (wBitCount <= 8) {       
		dwPaletteSize = (1 << wBitCount) *sizeof(RGBQUAD);
	}

	//设置位图信息头结构   
	GetObject(hBitmap, sizeof(BITMAP), (LPSTR)&Bitmap);   
	bi.biSize            = sizeof(BITMAPINFOHEADER);   
	bi.biWidth           = Bitmap.bmWidth;   
	bi.biHeight          = Bitmap.bmHeight;   
	bi.biPlanes          = 1;   
	bi.biBitCount         = wBitCount;       
	bi.biCompression      = BI_RGB;   
	bi.biSizeImage        = 0;   
	bi.biXPelsPerMeter     = 0;   
	bi.biYPelsPerMeter     = 0;   
	bi.biClrUsed         = 0;   
	bi.biClrImportant      = 0;   
	dwBmBitsSize = ((Bitmap.bmWidth *wBitCount+31)/32)* 4*Bitmap.bmHeight ;

	//为位图内容分配内存   
	hDib  = GlobalAlloc(GHND,dwBmBitsSize+dwPaletteSize+sizeof(BITMAPINFOHEADER));   
	lpbi = (LPBITMAPINFOHEADER)GlobalLock(hDib);
	if (lpbi==NULL)
	{
		return false;
	}

	*lpbi = bi;   
	// 处理调色板
	hPal = GetStockObject(DEFAULT_PALETTE);   
	if (hPal)
	{       
		hDC  = GetDC(NULL);       
		hOldPal = ::SelectPalette(hDC, (HPALETTE)hPal, FALSE);       
		RealizePalette(hDC);       
	}   
	// 获取该调色板下新的像素值   
	GetDIBits(hDC, hBitmap, 0, (UINT) Bitmap.bmHeight,       
		(LPSTR)lpbi + sizeof(BITMAPINFOHEADER)+dwPaletteSize,
		(LPBITMAPINFO)lpbi, DIB_RGB_COLORS);   
	//恢复调色板      
	if (hOldPal)       
	{       
		SelectPalette(hDC, (HPALETTE)hOldPal, TRUE);       
		RealizePalette(hDC);       
		ReleaseDC(NULL, hDC);       
	}   
	//创建位图文件       
	fh = CreateFile(lpFileName, GENERIC_WRITE,        
		0, NULL, CREATE_ALWAYS,       
		FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, NULL);   

	if (fh == INVALID_HANDLE_VALUE) {
		return false;
	}

	// 设置位图文件头   
	bmfHdr.bfType = 0x4D42;  // "BM"   
	dwDIBSize    = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER)+ dwPaletteSize + dwBmBitsSize;     
	bmfHdr.bfSize = dwDIBSize;   
	bmfHdr.bfReserved1 = 0;   
	bmfHdr.bfReserved2 = 0;   
	bmfHdr.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) + (DWORD)sizeof(BITMAPINFOHEADER)+ dwPaletteSize;

	// 写入位图文件头   
	WriteFile(fh, (LPSTR)&bmfHdr, sizeof(BITMAPFILEHEADER), &dwWritten, NULL);

	// 写入位图文件其余内容   
	WriteFile(fh, (LPSTR)lpbi, dwDIBSize, &dwWritten, NULL);

	//清除      
	GlobalUnlock(hDib);   
	GlobalFree(hDib);   
	CloseHandle(fh);

	return true;
}

//////////////////////////////////////////////////////////////////////////
ImageMPRProcess::ImageMPRProcess(std::string str_paras)
	: ImageProcessBase(str_paras)
{
	m_wnd_name = "mpr1";
}

ImageMPRProcess::~ImageMPRProcess()
{
}

bool ImageMPRProcess::Excute(std::string& out_image_data)
{
	// 暂时，先从本地读取Dicom文件
	GNC::GCS::StudyContextMy* my = new GNC::GCS::StudyContextMy();
	const std::string path_file("C:\\ztest2\\dicom_test\\413");
	my->ReadDicomFile(path_file);

	// 1.read dcm image from directory
	
	std::string::size_type sz;
	double zoom_scale = std::stod(m_key3_str_paras, &sz);

	if (!reader) {
		reader = new VtkDcmLoader();
		reader->LoadDirectory(dicom_files_dir.c_str());	// only once
		VolData* vol_data = reader->GetData();
		if (vol_data == NULL) return false;
		ImageDataSource::Get()->AddVolData(series_name_mpr, vol_data);
	}

	if (!is_create_mpr_render) {
		// 2.create all image control
		RenderSource::Get()->CreateRenderControl(m_wnd_name, RenderControlType::MPR);	// only once
		RenderFacade::Get()->ChangeSeries(series_name_mpr);	
		//RenderFacade::Get()->SetOrientation(wnd_mpr1_, AXIAL);
		//float pos[3] = { 255.0f, 255.0f, 0};
		RenderFacade::Get()->SetOrientation(m_wnd_name, SAGITTAL);
		RenderFacade::Get()->RenderControl(m_wnd_name);

		is_create_mpr_render = true;
	}

	if (m_key2_str_opertation == JSON_VALUE_IMAGE_OPERATION_ZOOM) {
		RenderFacade::Get()->Zoom(m_wnd_name, zoom_scale);
	} else if (m_key2_str_opertation == JSON_VALUE_IMAGE_OPERATION_ROTATE) {
	} else if (m_key2_str_opertation == JSON_VALUE_IMAGE_OPERATION_MOVE) {
		static int slice_index = 100;
		slice_index += zoom_scale * 10;
		float pos[3] = { slice_index, 255.0f, 189.0f};
		RenderFacade::Get()->MoveTo(m_wnd_name, pos);
	} else if (m_key2_str_opertation == JSON_VALUE_IMAGE_OPERATION_SKIP) {
	}

	// 3.get imaging object through builder. then go render and get show buffer through imaging object
	HBITMAP hBitmap = RenderFacade::Get()->GetImageBuffer(m_wnd_name);
	BITMAP  bitmap ;
	GetObject (hBitmap, sizeof (BITMAP), &bitmap);

	std::wstring ws_screenshot_file = StringToWString(screen_shot_file_path);
	SaveBitmapToFile(hBitmap, ws_screenshot_file.c_str());

	cv::Mat src = cv::imread(screen_shot_file_path.c_str());
	out_image_data = Mat2Base64(src, "bmp");

	return true;
}
//////////////////////////////////////////////////////////////////////////
ImageVRProcess::ImageVRProcess(std::string str_paras)
	: ImageProcessBase(str_paras)
	//, reader(NULL)
{
	m_wnd_name = "vr";
}

ImageVRProcess::~ImageVRProcess()
{
}

bool ImageVRProcess::Excute(std::string& out_image_data)
{
	// 暂时，先从本地读取Dicom文件
	//GNC::GCS::StudyContextMy* my = new GNC::GCS::StudyContextMy();
	//const std::string path_file(""C:\\ztest2\\dicom_test\\413");
	//my->ReadDicomFile(path_file);

	// 1.read dcm image from directory
	
	std::string::size_type sz;
	double zoom_scale = std::stod(m_key3_str_paras, &sz);

	if (!reader) {
		reader = new VtkDcmLoader();
		reader->LoadDirectory(dicom_files_dir.c_str());	// only once

		VolData* vol_data = reader->GetData();
		if (vol_data == NULL) return false;
		ImageDataSource::Get()->AddVolData(series_name_vr, vol_data);
	}

	if (!is_create_vr_render) {
		// 2.create all image control
		RenderSource::Get()->CreateRenderControl(m_wnd_name, RenderControlType::VR);	// only once
		RenderFacade::Get()->ChangeSeries(series_name_vr);	
		//RenderFacade::Get()->SetOrientation(wnd_mpr1_, AXIAL);
		//float pos[3] = { 255.0f, 255.0f, 0};
		RenderFacade::Get()->SetOrientation(m_wnd_name, CORONAL);
		RenderFacade::Get()->RenderControl(m_wnd_name);

		is_create_vr_render = true;
	}

	if (m_key2_str_opertation == JSON_VALUE_IMAGE_OPERATION_ZOOM) {
		RenderFacade::Get()->Zoom(m_wnd_name, zoom_scale);
	} else if (m_key2_str_opertation == JSON_VALUE_IMAGE_OPERATION_ROTATE) {
		float f[3] = { 0.0,1.0,0.0 };
		zoom_scale *= 100;
		RenderFacade::Get()->Rotate(m_wnd_name, zoom_scale, f);
	} else if (m_key2_str_opertation == JSON_VALUE_IMAGE_OPERATION_MOVE) {
		static int slice_index = 100;
		slice_index += zoom_scale * 10;
		float pos[3] = { slice_index, 255.0f, 189.0f};
		RenderFacade::Get()->MoveTo(m_wnd_name, pos);
	} else if (m_key2_str_opertation == JSON_VALUE_IMAGE_OPERATION_SKIP) {
	}

	// 3.get imaging object through builder. then go render and get show buffer through imaging object
	HBITMAP hBitmap = RenderFacade::Get()->GetImageBuffer(m_wnd_name);
	BITMAP  bitmap ;
	GetObject (hBitmap, sizeof (BITMAP), &bitmap);

	std::wstring ws_screenshot_file = StringToWString(screen_shot_file_path);
	SaveBitmapToFile(hBitmap, ws_screenshot_file.c_str());

	cv::Mat src = cv::imread(screen_shot_file_path.c_str());
	out_image_data = Mat2Base64(src, "bmp");

	return true;
}

//////////////////////////////////////////////////////////////////////////
ImageCPRProcess::ImageCPRProcess(std::string str_paras)
	: ImageProcessBase(str_paras)
{
	m_wnd_name = "cpr";
}

ImageCPRProcess::~ImageCPRProcess()
{
}

bool ImageCPRProcess::Excute(std::string& out_image_data)
{	
	// 1.read dcm image from directory

	std::string::size_type sz;
	double zoom_scale = std::stod(m_key3_str_paras, &sz);

	if (!reader) {
		reader = new NiiImageLoader();
		std::vector<const char*> files;
		files.push_back(nii_file_data_path.c_str());
		reader->LoadFiles(files);	// only once
		reader->LoadVolumeMask(nii_file_mask_path.c_str());

		VolData* vol_data = reader->GetData();
		if (vol_data == NULL) return false;
		vol_data->SetDefaultWindowWidthLevel(820, 250);
		ImageDataSource::Get()->AddVolData(series_name_cpr, vol_data);
	}

	if (!is_create_cpr_render) {
		// 2.create all image control
		RenderSource::Get()->CreateRenderControl(m_wnd_name, RenderControlType::STRETECHED_CPR);	// only once
		//////////////////////////////////////////////////////////////////////////
		// move mpr to specified locations
		vector<string> curve_data = ReadTxt(nii_curve_data_path.c_str());
		vector<Point3f> points;
		auto it = curve_data.begin();
		while (it != curve_data.end()){
			vector<string> arr_data = Split(*it, ",");
			if (arr_data.size() >= 3){
				Point3f pnt;
				pnt.x = atoi(arr_data[0].c_str());
				pnt.y = atoi(arr_data[1].c_str());
				pnt.z = atoi(arr_data[2].c_str()) - 1;

				points.push_back(pnt);
			}
			++it;
		}
		curve_id_ = CurveSource::Get()->CreateCurve(series_name_cpr, points);

		Vector3f vx, vy;
		float ix, iy, iz;
		
		RenderFacade::Get()->ChangeSeries(series_name_cpr);
		RenderFacade::Get()->SetCPRCurveID(m_wnd_name, curve_id_);
		RenderFacade::Get()->RenderControl(m_wnd_name);

		is_create_cpr_render = true;
	}

	if (m_key2_str_opertation == JSON_VALUE_IMAGE_OPERATION_ZOOM) {
		RenderFacade::Get()->Zoom(m_wnd_name, zoom_scale);
	} else if (m_key2_str_opertation == JSON_VALUE_IMAGE_OPERATION_ROTATE) {
		float f[3] = { 0.0,1.0,0.0 };
		zoom_scale *= 100;
		RenderFacade::Get()->Rotate(m_wnd_name, zoom_scale, f);
	} else if (m_key2_str_opertation == JSON_VALUE_IMAGE_OPERATION_MOVE) {
		static int slice_index = 100;
		slice_index += zoom_scale * 10;
		float pos[3] = { slice_index, 255.0f, 189.0f};
		RenderFacade::Get()->MoveTo(m_wnd_name, pos);
	} else if (m_key2_str_opertation == JSON_VALUE_IMAGE_OPERATION_SKIP) {
	}

	// 3.get imaging object through builder. then go render and get show buffer through imaging object
	HBITMAP hBitmap = RenderFacade::Get()->GetImageBuffer(m_wnd_name);
	BITMAP  bitmap ;
	GetObject (hBitmap, sizeof (BITMAP), &bitmap);

	std::wstring ws_screenshot_file = StringToWString(screen_shot_file_path);
	SaveBitmapToFile(hBitmap, ws_screenshot_file.c_str());

	cv::Mat src = cv::imread(screen_shot_file_path.c_str());
	out_image_data = Mat2Base64(src, "bmp");
	return true;
}