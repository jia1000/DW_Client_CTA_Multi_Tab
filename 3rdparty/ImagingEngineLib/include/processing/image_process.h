/*=========================================================================

  Program:   ImagingEngine
  Module:    image_process.h
  author: 	 zhangjian
  Brief:	 

=========================================================================*/
#pragma once

#include "opencv2/core/core.hpp"    
#include "opencv2/highgui/highgui.hpp"  
#include "opencv2/imgproc/imgproc.hpp" 
#include "opencv2/opencv.hpp"
#include "data/data_definition.h"

//using namespace DW::IMAGE;

namespace DW {
	namespace CV {
		
		class ImageProcessBase
		{
		public:
			ImageProcessBase(HBITMAP& in_image_data, int width, int height, int bits);
			~ImageProcessBase();

			virtual bool Excute(HBITMAP& out_image_data) = 0;

		protected:	
			/// opencv Mat和Buffer的互转
			void Mat2Buffer(const cv::Mat &img, UNITDATASHOW *output);
			void Buffer2Mat(const UNITDATASHOW *input, int width, int height, int bits, cv::Mat &img);
			/// 
			BOOL ImageProcessBase::HBitmapToMat(HBITMAP& _hBmp, cv::Mat& _mat);
			BOOL ImageProcessBase::MatToHBitmap(cv::Mat& _mat, HBITMAP& _hBmp);

			/// hbitmap convert to IplImage   
			IplImage *HBitmapToIplImage(HBITMAP hBmp);

		protected:
			/// 图像原始数据
			HBITMAP in_image_data_;
			int image_width_;
			int image_height_;
			int bits_per_pixel_;
		};

		//////////////////////////////////////////////////////////////////////////
		/// 放缩图像
		class ImageZoomProcess : public ImageProcessBase
		{
		public:
			ImageZoomProcess(HBITMAP& in_image_data, int width, int height, int bits, float scale);
			~ImageZoomProcess();

			/// 图像缩放后数据
			bool Excute(HBITMAP& out_image_data) override; 

		private:
			/// 缩放倍率
			float scale_;
		};

		//////////////////////////////////////////////////////////////////////////
		/// 选择图像
		class ImageRotateProcess : public ImageProcessBase
		{
		public:
			ImageRotateProcess(HBITMAP& in_image_data, int width, int height, int bits, float angle);
			~ImageRotateProcess();

			bool Excute(HBITMAP& out_image_data) override;

		private:
			/// 旋转角度
			float angle_;
		};

		//////////////////////////////////////////////////////////////////////////
		/// 平移图像
		class ImageMoveProcess : public ImageProcessBase
		{
		public:
			ImageMoveProcess(HBITMAP& in_image_data, int width, int height, int bits, float dx, float dy);
			~ImageMoveProcess();

			bool Excute(HBITMAP& out_image_data) override;

		private:
			/// 平移偏移量x/y
			float offset_x_;
			float offset_y_;
		};
		
	}
}