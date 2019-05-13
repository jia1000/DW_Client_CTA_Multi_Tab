#include "imaging_cpr.h"
#include "renderer_cpr.h"
#include "vector3.h"
#include "render/render_param_cpr.h"
#include "tools/math.h"
#include "processing/image_process.h"
#include "tools/bitmap_writer.h"

using namespace DW::Render;
using namespace DW::CV;

CPRImaging::CPRImaging(string id)
	: IThreedImaging(id)
{
	show_buffer_ = new ShowBuffer();
	show_buffer_->InitBufferData(512, 512, 32);
	bounding_box_ = new BoundingBox();
	rotation_angle_ = 0.0f;
}

CPRImaging::~CPRImaging()
{

}

VolData* CPRImaging::GetData()
{
	return volume_data_;
}

void CPRImaging::SetData(VolData* data)
{
	volume_data_ = data;
	bounding_box_->Update(0,0,0,data->GetSliceWidth(),data->GetSliceHeight(),data->GetSliceCount());
}

ShowBuffer* CPRImaging::GetShowBuffer()
{
	if (renderer_){
		show_buffer_ = renderer_->GetShowBuffer();
	}
	return show_buffer_;
}

void CPRImaging::Render()
{
	if (renderer_){
		renderer_->Render();

		ConvertToHBITMAP();
	}
}

void CPRImaging::ConvertToHBITMAP()
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

void CPRImaging::SetRenderer(IThreedRenderer* renderer)
{
	renderer_ = renderer;
}

void CPRImaging::Zoom(float scale)
{
	if (renderer_){
		RenderParam* param = renderer_->GetRenderParam();
		if (param){

			scale_factor_ *= scale;
			param->SetScale(scale_factor_);

			ConvertToHBITMAP();
		}
	}
}

void CPRImaging::ZoomToFitWindow()
{

}

void CPRImaging::Move(float dx, float dy)
{
	RenderParam* param = renderer_->GetRenderParam();
	if (param){

		offset_x_ += dx;
		offset_y_ += dy;
		param->SetOffset(offset_x_, offset_y_);

		ConvertToHBITMAP();
	}
}

void CPRImaging::Move(float motionVector[3])
{
	// Not applicable
}

void CPRImaging::Rotate(float angle)
{
	if (renderer_){

		RenderParam* param = renderer_->GetRenderParam();
		if (param){

			roll_angle_ += angle;

			ConvertToHBITMAP();

		}
	}
}

void CPRImaging::Rotate3D(Vector3f /*axis*/, float angle)
{
	if (renderer_){
		CPRRenderParam* param = dynamic_cast<CPRRenderParam*>(renderer_->GetRenderParam());
		if (param){

			//if (param->GetRenderMode() == CPRRenderMode::STRETCHED_CPR_VER ||
			//	param->GetRenderMode() == CPRRenderMode::STRETCHED_CPR_HOR){
			float axis[3]={0}, direction[3]={0}, new_direction[3]={0};
			param->GetAxis(axis[0], axis[1], axis[2]);
			param->GetDirection(direction[0], direction[1], direction[2]);

			MathTool::RotateAroundAxis(axis[0], axis[1], axis[2], 
				direction[0], direction[1], direction[2], 
				new_direction[0], new_direction[1], new_direction[2],
				angle);

			param->SetDirection(new_direction[0], new_direction[1], new_direction[2]);
			//}
			//else if (param->GetRenderMode() == CPRRenderMode::STRAIGHTENED_CPR_VER ||
			//	param->GetRenderMode() == CPRRenderMode::STRAIGHTENED_CPR_HOR){

			// 累加旋转角度，用于拉直cpr
			rotation_angle_ += angle;

			param->SetAngle(rotation_angle_);
			//}
			renderer_->Render();

			ConvertToHBITMAP();
		}
	}
}

void CPRImaging::WindowWidthLevel(int width, int level)
{
	if (renderer_){
		CPRRenderParam* param = dynamic_cast<CPRRenderParam*>(renderer_->GetRenderParam());
		if (param){
			param->SetWindowWidthLevel(width, level);

			renderer_->BufferTransform();

			ConvertToHBITMAP();
		}
	}
}

void CPRImaging::WindowWidthLevelOffset(int offset_width, int offset_level)
{
	if (renderer_){
		CPRRenderParam* param = dynamic_cast<CPRRenderParam*>(renderer_->GetRenderParam());
		if (param){
			int width, level;
			param->GetWindowWidthLevel(width, level);
			param->SetWindowWidthLevel(width + offset_width, width + offset_level);

			renderer_->BufferTransform();

			ConvertToHBITMAP();
		}
	}
}