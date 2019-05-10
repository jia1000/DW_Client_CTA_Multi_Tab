#include "imaging_mpr.h"
#include "renderer_mpr.h"
#include "vector3.h"
#include "render/render_param_mpr.h"
#include "processing/image_process.h"
#include "tools/math.h"
#include "tools/bitmap_writer.h"

using namespace DW::Render;
using namespace DW::CV;

MPRImaging::MPRImaging(string id)
	: IThreedImaging(id)
{
	show_buffer_ = new ShowBuffer();
	show_buffer_->InitBufferData(512, 512, 32);
	bounding_box_ = new BoundingBox();
}

MPRImaging::~MPRImaging()
{

}

// 获取原始三维图像数据场
VolData* MPRImaging::GetData()
{
	return volume_data_;
}
// 设置原始三维图像数据场
void MPRImaging::SetData(VolData* data)
{
	volume_data_ = data;
	bounding_box_->Update(0,0,0,data->GetSliceWidth(),data->GetSliceHeight(),data->GetSliceCount());
}
// 获取输出显示图像
ShowBuffer* MPRImaging::GetShowBuffer()
{
	if (renderer_){
		show_buffer_ = renderer_->GetShowBuffer();
	}
	return show_buffer_;
}
// 渲染图像，缓存到show_buffer_
void MPRImaging::Render()
{
	if (renderer_){
		// Call renderer's Render method
		renderer_->Render();

		ConvertToHBITMAP();

	}
}

void MPRImaging::ConvertToHBITMAP()
{
	// Get show buffer
	show_buffer_ = renderer_->GetShowBuffer();
	// Convert to image_buffer HBITMAP
	image_buffer_ = BitmapWriter::ConvertToHBITMAP(show_buffer_->GetShowBuffer(), 
		show_buffer_->GetWidth(), 
		show_buffer_->GetHeight(),
		0);

	/// Apply scale factor
	if (abs(scale_factor_ - 1.0f) > MathTool::kEpsilon){

		HBITMAP hbitmap;
		// User opencv to move image in plane
		ImageZoomProcess processor(image_buffer_, 
			show_buffer_->GetWidth(), 
			show_buffer_->GetHeight(), 
			show_buffer_->GetBitsPerPixel(),
			scale_factor_);

		processor.Excute(hbitmap);

		DeleteObject (image_buffer_);
		// Set it to be the transformed HBITMAP
		image_buffer_ = hbitmap;
	}
	
	if (abs(offset_x_) > MathTool::kEpsilon ||
		abs(offset_y_) > MathTool::kEpsilon){

		HBITMAP hbitmap;
		// User opencv to move image in plane
		ImageMoveProcess processor(image_buffer_, 
			show_buffer_->GetWidth(), 
			show_buffer_->GetHeight(), 
			show_buffer_->GetBitsPerPixel(),
			offset_x_,
			offset_y_);

		processor.Excute(hbitmap);

		DeleteObject (image_buffer_);
		// Set it to be the transformed HBITMAP
		image_buffer_ = hbitmap;
	}

	if (abs(roll_angle_) > MathTool::kEpsilon){

		HBITMAP hbitmap;
		// User opencv to move image in plane
		ImageRotateProcess processor(image_buffer_, 
			show_buffer_->GetWidth(), 
			show_buffer_->GetHeight(), 
			show_buffer_->GetBitsPerPixel(),
			roll_angle_);

		processor.Excute(hbitmap);

		DeleteObject (image_buffer_);
		// Set it to be the transformed HBITMAP
		image_buffer_ = hbitmap;
	}
}

void MPRImaging::SetRenderer(IThreedRenderer* renderer)
{
	renderer_ = renderer;
}

void MPRImaging::Zoom(float scale) 
{
	if (renderer_){
		MPRRenderParam* param = dynamic_cast<MPRRenderParam*>(renderer_->GetRenderParam());
		if (param){

			scale_factor_ *= scale;
			param->SetScale(scale_factor_);

			ConvertToHBITMAP();
		}
	}
}

void MPRImaging::ZoomToFitWindow()
{
	Point3f top_left, bottom_right;
	Point3f display_top_left, display_bottom_right;
	bounding_box_->GetCornerPoints(top_left, bottom_right);

	renderer_->ComputeWorldToDisplay(top_left, display_top_left);
	renderer_->ComputeWorldToDisplay(bottom_right, display_bottom_right);

	// We recalculate taking into account the display
	double display_width, display_height;
	display_width = fabs(display_top_left.x - display_bottom_right.x);
	display_height = fabs(display_top_left.y - display_bottom_right.y);

	/// Adjust the image depending on whether the window is narrower by width or height. 
	/// If we want to see the whole region that we have chosen, we will adjust Be narrower, 
	/// if we adjust through wider image to the other side
	int image_width = renderer_->GetRenderParam()->GetWidth();
	int image_height = renderer_->GetRenderParam()->GetHeight();
	double ratio = min(image_width / display_width, image_height / display_height);
	double margin_rate = 0.01;
	double factor = ratio * (1.0 - margin_rate);

	/// Update zoom factor to camera
	renderer_->GetRenderParam()->GetCamera()->Zoom(factor);
	/// Call render
	renderer_->Render();
}

void MPRImaging::Move(float dx, float dy) 
{
	if (renderer_){

		MPRRenderParam* param = dynamic_cast<MPRRenderParam*>(renderer_->GetRenderParam());
		if (param){

			// Cache offset number to let opencv generate a new image
			offset_x_ += dx;
			offset_y_ += dy;
			param->SetOffset(offset_x_, offset_y_);

			ConvertToHBITMAP();
		}
	}
}

void MPRImaging::Move(float motionVector[3]) 
{
	if (renderer_){
		MPRRenderParam* param = dynamic_cast<MPRRenderParam*>(renderer_->GetRenderParam());
		if (param){
			
			// Set image center to a new position
			param->SetImageCenter(motionVector[0], motionVector[1], motionVector[2]);
			// Call render to recalculate mpr plane according to the new position
			renderer_->Render();

			ConvertToHBITMAP();
		}
	}
}

void MPRImaging::Rotate(float angle) 
{
	if (renderer_){

		MPRRenderParam* param = dynamic_cast<MPRRenderParam*>(renderer_->GetRenderParam());
		if (param){

			// Cache angle number to let opencv generate a new image
			roll_angle_ += angle;

			ConvertToHBITMAP();
		}
	}
}

void MPRImaging::Rotate3D(Vector3f axis, float angle) 
{
	// Not supported
}

void MPRImaging::WindowWidthLevel(int width, int level) 
{
	if (renderer_){
		MPRRenderParam* param = dynamic_cast<MPRRenderParam*>(renderer_->GetRenderParam());
		if (param){
			param->SetWindowWidthLevel(width, level);

			renderer_->BufferTransform();

			ConvertToHBITMAP();
		}
	}
}

void MPRImaging::WindowWidthLevelOffset(int offset_width, int offset_level)
{
	if (renderer_){
		MPRRenderParam* param = dynamic_cast<MPRRenderParam*>(renderer_->GetRenderParam());
		if (param){
			int width, level;
			param->GetWindowWidthLevel(width, level);
			param->SetWindowWidthLevel(width + offset_width, width + offset_level);

			renderer_->BufferTransform();

			ConvertToHBITMAP();
		}
	}
}