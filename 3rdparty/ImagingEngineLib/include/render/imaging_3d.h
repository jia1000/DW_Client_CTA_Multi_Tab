/*=========================================================================

  Program:   ImagingEngine
  Module:    imaging_3d.h
  author: 	 zhangjian
  Brief:	 表示一个三维图像，输入三维数据，执行具体的渲染过程，并输出显示结果
			 提供用于操作图像的接口，如旋转/放缩/平移/窗宽窗位等

=========================================================================*/
#pragma once

#include "data_definition.h"
//#include "imaging_base.h"
#include "vol_data.h"
#include "renderer_3d.h"
#include "render_param.h"
#include "show_buffer.h"
#include "vector3.h"

using namespace DW::IMAGE;

namespace DW {
	namespace Render{

		class IThreedImaging //: public DWImaging
		{
		public:
			IThreedImaging(string id)
				: image_uid_(id)
				, scale_factor_(1.0f)
				, offset_x_(0.0f)
				, offset_y_(0.0f)
				, roll_angle_(0.0f)
			{

			}
			virtual ~IThreedImaging()
			{
				if (bounding_box_){
					delete bounding_box_;
					bounding_box_ = NULL;
				}
				if (volume_data_){
					delete volume_data_;
					volume_data_ = NULL;
				}
				if (renderer_){
					delete renderer_;
					renderer_ = NULL;
				}
				if (show_buffer_){
					delete show_buffer_;
					show_buffer_ = NULL;
				}
			}
			/// 获取图像类型
			virtual RenderMode GetRenderMode()
			{
				if (renderer_){
					return renderer_->GetRenderMode();
				}
				return RenderMode::UNDEFINED;
			}
			/// 设置原始三维图像数据场 
			virtual void SetData(VolData* data) = 0;
			/// 获取原始三维图像数据场
			virtual VolData* GetData() = 0;
			/// 获取输出显示图像
			virtual ShowBuffer* GetShowBuffer() = 0;
			virtual HBITMAP GetImageBuffer() { return image_buffer_; }
			/// 渲染图像
			virtual void Render() = 0;
			/// 放缩
			virtual void Zoom(float scale) = 0;
			/// 放缩到适应窗口
			virtual void ZoomToFitWindow() = 0;
			/// 平面内移动
			virtual void Move(float dx, float dy) = 0;
			/// Move the camera according to the motion vector we spend
			/// @param motionVector[] motion Vector that determines where and how much camera moves
			virtual void Move(float motionVector[3]) = 0;
			/// 旋转
			virtual void Rotate(float angle) = 0;
			/// 绕着指定中心轴旋转一定角度
			virtual void Rotate3D(Vector3f axis, float angle) = 0;
			/// 窗宽窗位
			virtual void WindowWidthLevel(int width, int level) = 0;
			virtual void WindowWidthLevelOffset(int offset_width, int offset_level) = 0;
			/// 创建renderer
			virtual void SetRenderer(IThreedRenderer* renderer) {renderer_=renderer;}
			/// 获取renderer
			virtual IThreedRenderer* GetRenderer() {return renderer_;}
			/// 获取当前包围盒对象
			BoundingBox* GetBoundingBox() { return bounding_box_; }
			/// 图像ID
			string GetImagingID() {return image_uid_;}

		protected:
			virtual void ConvertToHBITMAP() = 0;

		protected:
			/// 图像ID
			const string image_uid_;
			/// 当前窗口的包围盒
			BoundingBox *bounding_box_;
			/// 图像类型
			RenderMode render_mode_;
			/// 三维数据
			VolData* volume_data_;
			/// 执行渲染的对象
			IThreedRenderer* renderer_;
			/// 用于UI显示的结果
			ShowBuffer* show_buffer_;
			HBITMAP image_buffer_;
			/// 累计的放缩比率
			float scale_factor_;
			/// 累计的偏移量
			float offset_x_;
			float offset_y_;
			/// 累计的平面内旋转角度
			float roll_angle_;

		private:


		};
	}
}