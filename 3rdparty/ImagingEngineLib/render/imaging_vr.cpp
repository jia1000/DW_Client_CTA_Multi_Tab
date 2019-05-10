#include "imaging_vr.h"
#include "renderer_raycasting_cpu.h"
#include "vector3.h"
#include "tools/math.h"
#include "processing/image_process.h"
#include "tools/bitmap_writer.h"

using namespace DW::Render;
using namespace DW::CV;

VRImaging::VRImaging(string id)
	: IThreedImaging(id)
{
	show_buffer_ = new ShowBuffer();
	show_buffer_->InitBufferData(512, 512, 32);
	bounding_box_ = new BoundingBox();
}

VRImaging::~VRImaging()
{

}

VolData* VRImaging::GetData()
{
	return volume_data_;
}

void VRImaging::SetData(VolData* data)
{
	volume_data_ = data;
	bounding_box_->Update(0,0,0,data->GetSliceWidth(),data->GetSliceHeight(),data->GetSliceCount());
}

ShowBuffer* VRImaging::GetShowBuffer()
{
	if (renderer_){
		show_buffer_ = renderer_->GetShowBuffer();
	}
	return show_buffer_;
}

void VRImaging::Render()
{
	if (renderer_){
		renderer_->Render();

		ConvertToHBITMAP();
	}
}

void VRImaging::ConvertToHBITMAP()
{
	// Get show buffer
	show_buffer_ = renderer_->GetShowBuffer();
	// Convert to image_buffer HBITMAP
	image_buffer_ = BitmapWriter::ConvertToHBITMAP(show_buffer_->GetShowBuffer(), 
		show_buffer_->GetWidth(), 
		show_buffer_->GetHeight(),
		0);
	
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

void VRImaging::Zoom(float scale)
{
	if (renderer_){
		Camera* camera = renderer_->GetCamera();
		if (camera){
			camera->Zoom(scale);
			renderer_->Render();

			ConvertToHBITMAP();
		}
	}
}

void VRImaging::ZoomToFitWindow()
{
	Point3f top_left, bottom_right;
	Point3f display_top_left, display_bottom_right;
	bounding_box_->GetCornerPoints(top_left, bottom_right);

	renderer_->ComputeWorldToDisplay(top_left, display_top_left);
	renderer_->ComputeWorldToDisplay(bottom_right, display_bottom_right);

	// We Recalculem taking into account the display
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

	ConvertToHBITMAP();
}

void VRImaging::Move(float dx, float dy) 
{
	RenderParam* param = renderer_->GetRenderParam();
	if (param){

		offset_x_ += dx;
		offset_y_ += dy;
		param->SetOffset(offset_x_, offset_y_);

		ConvertToHBITMAP();
	}
}

void VRImaging::Move(float motionVector[3])
{
	if (renderer_){
		vtkCamera *camera = renderer_->GetCamera()->GetVtkCamera();
		if (!camera){
			return;
		}

		double viewFocus[4], viewPoint[3];
		camera->GetFocalPoint(viewFocus);
		camera->GetPosition(viewPoint);
		camera->SetFocalPoint(motionVector[0] + viewFocus[0], motionVector[1] + viewFocus[1], motionVector[2] + viewFocus[2]);
		camera->SetPosition(motionVector[0] + viewPoint[0], motionVector[1] + viewPoint[1], motionVector[2] + viewPoint[2]);
		renderer_->Render();

		ConvertToHBITMAP();
	}
}

void VRImaging::Rotate(float angle)
{
	if (renderer_){
		Camera* camera = renderer_->GetCamera();
		if (camera){
			// Cache angle number to let opencv generate a new image
			roll_angle_ += angle;

			ConvertToHBITMAP();
		}
	}
}

void VRImaging::Rotate3D(Vector3f axis, float angle) 
{
	if (renderer_){
		Camera* camera = renderer_->GetCamera();
		if (camera){
			Vector3f axis_x(1.0f, 0.0f, 0.0f);
			Vector3f axis_y(0.0f, 1.0f, 0.0f);
			Vector3f axis_z(0.0f, 0.0f, 1.0f);
			if (axis == axis_x){
				camera->RotateX(angle);
			}
			else if (axis == axis_y){
				camera->RotateY(angle);
			}
			else if (axis == axis_z){
				//camera->RotateZ(angle);

				// Roll image through opencv
				roll_angle_ += angle;

				ConvertToHBITMAP();

				return;
			}
			else{
				camera->RotateWXYZ(angle, axis[0], axis[1], axis[2]);
			}

			renderer_->Render();

			ConvertToHBITMAP();
		}
	}
}

void VRImaging::WindowWidthLevel(int width, int level) 
{

}

void VRImaging::WindowWidthLevelOffset(int offset_width, int offset_level)
{

}

void VRImaging::SetRenderer(IThreedRenderer* renderer)
{
	renderer_ = renderer;
}