/*=========================================================================

  Program:   ImagingEngine
  Module:    show_buffer.h
  author: 	 zhangjian
  Brief:	 UI显示结果类，包含UNITDATASHOW指针类型的buffer
			 UNITDATASHOW类型应定义为带符号类型

=========================================================================*/
#pragma once

#include "data_definition.h"

using namespace std;

namespace DW {
	namespace IMAGE{

		///
		// show buffer is associated with one imaging object
		///
		class ShowBuffer
		{
		public:
			// 生成一个指定大小的buffer
			void InitBufferData(int width, int height, BYTE bits_per_pixel);
			// 将字节数据赋值到该buffer中
			void SetBufferData(UNITDATASHOW* bytes, int width, int height, BYTE bits_per_pixel);
			// 获取当前buffer
			UNITDATASHOW* GetShowBuffer();
			// 宽度
			int GetWidth() {return buffer_width_;}
			// 高度
			int GetHeight() {return buffer_height_;}
			// 位数
			BYTE GetBitsPerPixel() {return bits_per_pixel_;}
			// 通道数
			BYTE GetNumberOfComponent() { return bits_per_pixel_ / 8;}
			
			// methods for testing
			void DumpBufferToFile(const char* file_path);

		protected:
			BYTE bits_per_pixel_;				//bits allocated for display 
			UNITDATASHOW* buffer_data_;			//pixel data for display
			int buffer_width_;					//width of the pixel data
			int buffer_height_;					//height of the pixel data
			float x_dpi_;						//indicate how long in mm unit is each pixel in x direction
			float y_dpi_;						//indicate how long in mm unit is each pixel in y direction
			string time_stamp_;					//time stamp uid

		private:


		};
	}
}