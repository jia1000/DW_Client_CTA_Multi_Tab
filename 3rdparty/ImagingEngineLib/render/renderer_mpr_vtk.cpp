/*=========================================================================

  Program:   ImagingEngine
  Module:    renderer_mpr_vtk.cpp
  author: 	 zhangjian
  Brief:	 

=========================================================================*/
#include "render/renderer_mpr_vtk.h"
#include "render/render_param_mpr.h"
#include "tools/vtk_image_data_creator.h"
#include "tools/convert_vtk_image_data_to_rgba.h"
#include "tools/math.h"
#include "tools/timer.h"
#include "tools/logger.h"

using namespace std;
using namespace DW::IMAGE;
using namespace DW::Render;

MPRRendererVtk::MPRRendererVtk()
{
	is_first_render_ = true;
	show_buffer_ = new ShowBuffer();
	vtk_render_window_ = vtkSmartPointer<vtkRenderWindow>::New();
	vtk_image_actor_ = vtkSmartPointer<vtkImageActor>::New();
	vtk_renderer_ =	vtkSmartPointer<vtkRenderer>::New();
	vtk_renderer_->AddActor(vtk_image_actor_);
	vtk_renderer_->SetBackground(0, 0, 0);
	vtk_render_window_->AddRenderer(vtk_renderer_);

	is_off_screen_rendering_ = true;
	vtk_render_window_->SetOffScreenRendering(1);
	
	
	vtk_image_reslice_ = vtkSmartPointer<vtkImageSlabReslice>::New();
	//vtk_image_reslice_->SetOutputDimensionality(2);
	//vtk_image_reslice_->SetResliceAxes(resliceAxes);
	//vtk_image_reslice_->SetInterpolationModeToLinear();

	//得到当前时间
	SYSTEMTIME st;
	::GetLocalTime(&st);
	char char_name[1000] = { 0 };
	printf_s(char_name, "mpr_%04d%02d%02d%02d%02d%02d%03d.bmp", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
	dump_file_name_ = string(char_name);
	if (dump_file_name_.empty()){
		dump_file_name_ = "D:\\mpr_test.bmp";
	}
}
MPRRendererVtk::~MPRRendererVtk()
{

}
void MPRRendererVtk::Render()
{
	if (volume_data_ == NULL) return;

	DoRender();
}
ShowBuffer *MPRRendererVtk::GetShowBuffer()
{
	/// Convert vtkImageData to Showbuffer object
	BufferTransform();

	return show_buffer_;
}
void MPRRendererVtk::SetData(VolData* data)
{
	volume_data_ = data;
	/// Workaround for vtkSmartVolumeMapper bug (https://gitlab.kitware.com/vtk/vtk/issues/17328)
	volume_data_->GetPixelData()->GetVtkImageData()->Modified();
#if VTK_MAJOR_VERSION > 5
	vtk_image_reslice_->SetInputData(volume_data_->GetPixelData()->GetVtkImageData());
	//vtk_image_actor_->SetInputData(volume_data_->GetPixelData()->GetVtkImageData());
#else
	vtk_image_reslice_->SetInput(volume_data_->GetPixelData()->GetVtkImageData());
	//vtk_image_actor_->SetInput(volume_data_->GetPixelData()->GetVtkImageData());
#endif
	is_first_render_ = true;
}

void MPRRendererVtk::DoRender()
{
	Timer::begin("MPR::DoRender");

	MPRRenderParam* param_imp = dynamic_cast<MPRRenderParam *>(render_param_);
	if (param_imp == NULL) return;

	double spacing[3] = {0.0};
	volume_data_->GetPixelData()->GetSpacing(spacing);
	int data_height = volume_data_->GetSliceHeight();
	// 获取图像中心点
	Point3f center_point;
	param_imp->GetImageCenter(center_point);
	// 获取成像平面的x/y方向向量
	Vector3f normal_vector, row_vector, column_vector;
	param_imp->GetPlaneVector(row_vector, column_vector);
	MathTool::Cross(row_vector, column_vector, normal_vector);
	// 因为vtk图像是从左下角开始，转换y轴坐标
	//center_point.y = data_height - 1 - center_point.y;
	double axialElements[16] = {
		row_vector[0], column_vector[0], normal_vector[0], center_point.x * spacing[0],
		row_vector[1], column_vector[1], normal_vector[1], center_point.y * spacing[1],
		row_vector[2], column_vector[2], normal_vector[2], center_point.z * spacing[2],
		0, 0, 0, 1 
	};

	vtkSmartPointer<vtkMatrix4x4> resliceAxes =
		vtkSmartPointer<vtkMatrix4x4>::New();
	resliceAxes->DeepCopy(axialElements);
	//// 应用图像中心点
	//resliceAxes->SetElement(0, 3, center_point.x * spacing[0]);
	//resliceAxes->SetElement(1, 3, center_point.y * spacing[1]);
	//resliceAxes->SetElement(2, 3, center_point.z * spacing[2]);
	vtk_image_reslice_->SetResliceAxes(resliceAxes);
	vtk_image_reslice_->SetSlabThickness(param_imp->GetThickness());

	//// 计算旋转角度和旋转轴
	//Vector3f rotate_axis;
	//float rotate_angle;
	//Vector3f old_normal(0.0f, 0.0f, 1.0f);
	//Vector3f new_normal, row_vector, column_vector;
	//param_imp->GetPlaneVector(row_vector, column_vector);
	//MathTool::Cross(row_vector, column_vector, new_normal);
	//MathTool::ComputeRotationAngleAxis(old_normal, new_normal, rotate_angle, rotate_axis);
	//// 应用行列向量
	//vtkTransform *transform = vtkTransform::New();
	//transform->RotateWXYZ(rotate_angle, rotate_axis[0], rotate_axis[1], rotate_axis[2]);
	//vtk_image_reslice_->SetResliceTransform(transform);

	if (param_imp->GetBlendMode() == BlendMode::MaximumIntensity){
		vtk_image_reslice_->SetBlendMode(VTK_IMAGE_SLAB_MAX);
	}
	else if (param_imp->GetBlendMode() == BlendMode::MinimumIntensity){
		vtk_image_reslice_->SetBlendMode(VTK_IMAGE_SLAB_MIN);
	}
	else {
		vtk_image_reslice_->SetBlendMode(VTK_IMAGE_SLAB_MEAN);
	}
	vtk_image_reslice_->InterpolateOn();
	if (param_imp->GetInterpolationMode() == InterpolationMode::Cubic){
		vtk_image_reslice_->SetInterpolationModeToCubic(); 
	}
	else if (param_imp->GetInterpolationMode() == InterpolationMode::Linear){
		vtk_image_reslice_->SetInterpolationModeToLinear();
	}
	else {
		vtk_image_reslice_->SetInterpolationModeToNearestNeighbor();
	}
	vtk_image_reslice_->SetBackgroundLevel(0);
	vtk_image_reslice_->SetOutputDimensionality(2);
	vtk_image_reslice_->Update();

	output_vtk_image_data_ = vtk_image_reslice_->GetOutput();

	Timer::end("MPR::DoRender");
	CGLogger::Info(Timer::summery());
}

void MPRRendererVtk::BufferTransform()
{
	if (output_vtk_image_data_ && show_buffer_){

		//vtkSmartPointer<vtkImageData> formatted_image_data;

		//vtkTransform *imageTransform = vtkTransform::New();
		//imageTransform->PostMultiply();
		///// Apply scale factor
		//float scale_factor = render_param_->GetScale();
		//imageTransform->Scale(scale_factor, scale_factor, scale_factor);
		///// Apply translation
		//float move_vector[3];
		//render_param_->GetMove(move_vector);
		//imageTransform->Translate(move_vector[0], move_vector[1], move_vector[2]);

		//vtkTransformFilter *transform_filter = vtkTransformFilter::New(); 
		//transform_filter->SetTransform(imageTransform);
		//transform_filter->SetInputData(output_vtk_image_data_);
		//transform_filter->Update();

		//formatted_image_data = (vtkImageData *)transform_filter->GetOutput();
		//if (formatted_image_data == NULL){
		//	return;
		//}
		//原来的方法，用来对比生成的图像
		int width = output_vtk_image_data_->GetDimensions()[0];
		int height = output_vtk_image_data_->GetDimensions()[1];
		// 从8位转换到32位
		int slice = -1;

		int ww, wl;
		(dynamic_cast<MPRRenderParam *>(render_param_))->GetWindowWidthLevel(ww, wl);
		// convert to 32 bits bitmap
		vtkImageData *pTmpImageData = NULL;	
		ConvertVtkImagedataToRGBA* convert = new ConvertVtkImagedataToRGBA();
		if (false == convert->ConvertImageScalarsToRGBA(output_vtk_image_data_, &pTmpImageData, -1, ww, wl))
		{
			return;
		}

		//// 写入磁盘文件
		//vtkSmartPointer<vtkBMPWriter> writer = vtkSmartPointer<vtkBMPWriter>::New();
		//writer->SetFileName(dump_file_name_.c_str());
		//writer->SetInputData(pTmpImageData);
		//writer->Write();


		UNITDATASHOW* pdata = reinterpret_cast<UNITDATASHOW*>(pTmpImageData->GetScalarPointer());
		int number_of_components = pTmpImageData->GetNumberOfScalarComponents();

		show_buffer_->SetBufferData(pdata, width, height, number_of_components * 8);

		//string path = "D:\\mpr_" + to_string(mpr_file_id) + ".bmp";
		//show_buffer_->DumpBufferToFile(path.c_str());
	}
}

void MPRRendererVtk::SetOffScreenRendering(bool flag)
{
	is_off_screen_rendering_ = flag;
}

bool MPRRendererVtk::PickByImagePos(const Vector2i& in_point, Vector3d& out_point)
{
	return false;
}

bool MPRRendererVtk::PickByImagePos(const int& x, const int& y, Vector3d& out_point)
{
	return false;
}

bool MPRRendererVtk::PickBy3DPoint(const Vector3d& in_point, int& x, int& y)
{
	return false;
}

float MPRRendererVtk::CalculateLength(Vector3d&, Vector3d&)
{
	return 0.0;
}

void MPRRendererVtk::GetOrientationTags(Vector3d&, Vector3d&, Vector3d&, Vector3d&)
{

}

void MPRRendererVtk::ComputeWorldToDisplay(Point3f& world_pos, Point3f& display_pos)
{

}

void MPRRendererVtk::ComputeWorldToDisplay(double x, double y, double z, double display_point[3])
{

}

void MPRRendererVtk::SetCameraViewPlane(const OrthogonalPlane &viewPlane)
{
	MPRRenderParam* param_imp = dynamic_cast<MPRRenderParam *>(render_param_);
	if (param_imp == NULL) return;
	
	current_view_plane_ = viewPlane;

	//float rotate_angle = param_imp->GetPlaneVector()
	////convert angle from degree to radian
	//double angleInRads = vtkMath::RadiansFromDegrees(angle);
	//double cosAngle = cos(angleInRads);
	//double sinAngle = sin(angleInRads);
	//// Set the slice orientation
	//vtkSmartPointer<vtkMatrix4x4> resliceAxes = vtkSmartPointer<vtkMatrix4x4>::New();

	//switch (current_view_plane_){
	//case OrthogonalPlane::XYPlane:
	//	double axialElements[16] = {
	//		1, 0, 0, 0,
	//		0, cosAngle,-sinAngle, 0,
	//		0, sinAngle,cosAngle, 0,
	//		0, 0, 0, 1 };
	//	resliceAxes->DeepCopy(axialElements);
	//}


	//vtkCamera *camera = param_imp->GetCamera()->GetVtkCamera();
	//if (camera == NULL) return;
	//// Adjust camera settings According to view
	//camera->SetFocalPoint(0.0, 0.0, 0.0);
	//switch (current_view_plane_)
	//{
	//case OrthogonalPlane::XYPlane:
	//	camera->SetViewUp(0.0, -1.0, 0.0);
	//	camera->SetPosition(0.0, 0.0, -1.0);
	//	break;
	//case OrthogonalPlane::YZPlane:
	//	camera->SetViewUp(0.0, 0.0, 1.0);
	//	camera->SetPosition(1.0, 0.0, 0.0);
	//	break;
	//case OrthogonalPlane::XZPlane:
	//	camera->SetViewUp(0.0, 0.0, 1.0);
	//	camera->SetPosition(0.0, -1.0, 0.0);
	//	break;
	//}
}
