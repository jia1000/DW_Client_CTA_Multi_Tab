#include "show_buffer.h"
#include "tools/bmp_writer.h"
#include "tools/bitmap_writer.h"

using namespace DW::IMAGE;

void ShowBuffer::InitBufferData(int width, int height, BYTE bits_per_pixel)
{
	buffer_width_ = width;
	buffer_height_ = height;
	bits_per_pixel_ = bits_per_pixel;

	if (buffer_data_){
		delete[] buffer_data_;
	}
	//RGBA通道
	buffer_data_ = new UNITDATASHOW[width * height * bits_per_pixel / 8];
}

void ShowBuffer::SetBufferData(UNITDATASHOW* bytes, int width, int height, BYTE bits_per_pixel)
{
	int old_width = buffer_width_;
	int old_height = buffer_height_;
	int old_components = bits_per_pixel_ / 8;

	buffer_width_ = width;
	buffer_height_ = height;
	bits_per_pixel_ = bits_per_pixel;

	int number_of_components = bits_per_pixel_ / 8;
	//if (old_width * old_height * old_components < width * height * number_of_components){
		delete[] buffer_data_;
		buffer_data_ = new UNITDATASHOW[width * height * number_of_components];
	//}
	memcpy(reinterpret_cast<UNITDATASHOW *>(buffer_data_), bytes, width * height * number_of_components); 
}

UNITDATASHOW* ShowBuffer::GetShowBuffer()
{
	return buffer_data_;
}

void ShowBuffer::DumpBufferToFile(const char* file_path)
{
	// 输出正确的bmp图像
	BitmapWriter::SaveAsBMP(file_path, buffer_data_, buffer_width_, buffer_height_); 
}